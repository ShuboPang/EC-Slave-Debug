#include "ethercatmaster.h"
#include "scanhardware/scanhardware.h"
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMap>
#include <QApplication>
#include <QCoreApplication>
#include <chrono>

#include "ethercatservobase.h"


#define EC_TIMEOUTMON 500

OSAL_THREAD_HANDLE thread1 = NULL;
UINT mmResult = 0;//< 定时器
int expectedWKC;
boolean needlf;
volatile int wkc;
volatile int rtcnt;
boolean inOP;
uint8 currentgroup = 0;
char IOmap[4096];
int16_t* IOmap16;
ec_ODlistt ODlist;
ec_OElistt OElist;


static QMap<int,const _EthercatSlaveConfig*> ethercat_servo_map;

static QMap<int,QVector<AxisMotion*>> ethercat_servo_axis_map;

QVector<int> time_;

auto msec = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
auto end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

/* most basic RT thread for process data, just does IO transfer */
void CALLBACK RTthread(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1,  DWORD_PTR dw2)
{
//    msec = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//    time_.append(msec - end);
//    end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ec_send_processdata();
    wkc = ec_receive_processdata(EC_TIMEOUTRET);
    rtcnt++;
    /* do RT control stuff here */
    for(int i = 1;i<=ec_slavecount;i++){
        if(ethercat_servo_map.contains(i)){
            const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(i);
            if(slave_config!=NULL){
                if(ethercat_servo_axis_map.contains(i)){
                    QVector<AxisMotion*> axis_motion = ethercat_servo_axis_map.value(i);
                    for(int j = 0;j<axis_motion.length();j++){
                        axis_motion.at(j)->CycleRun();
                        slave_config->cycle_run(&(ec_slave[i]),axis_motion.at(j),j);
                    }
                }
            }
        }
    }
}

//DWORD WINAPI ecatcheck( LPVOID lpParam )
OSAL_THREAD_FUNC ecatcheck(void *lpParam)
{
    int slave;

    while(1)
    {
        if( inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {
            if (needlf)
            {
                needlf = FALSE;
                qDebug("\n");
            }
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();
            for (slave = 1; slave <= ec_slavecount; slave++)
            {
                if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
                {
                    ec_group[currentgroup].docheckstate = TRUE;
                    if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                    {
                        qDebug("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                        ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                        ec_writestate(slave);
                    }
                    else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                    {
                        qDebug("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                        ec_slave[slave].state = EC_STATE_OPERATIONAL;
                        ec_writestate(slave);
                  }
                    else if(ec_slave[slave].state > 0)
                    {
                        if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                        {
                            ec_slave[slave].islost = FALSE;
                            qDebug("MESSAGE : slave %d reconfigured\n",slave);
                     }
                    }
                  else if(!ec_slave[slave].islost)
                    {
                        /* re-check state */
                        ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                        if (!ec_slave[slave].state)
                        {
                            ec_slave[slave].islost = TRUE;
                            qDebug("ERROR : slave %d lost\n",slave);
                     }
                    }
                }
                if (ec_slave[slave].islost)
                {
                    if(!ec_slave[slave].state)
                    {
                        if (ec_recover_slave(slave, EC_TIMEOUTMON))
                        {
                            ec_slave[slave].islost = FALSE;
                            qDebug("MESSAGE : slave %d recovered\n",slave);
                     }
                    }
                    else
                    {
                        ec_slave[slave].islost = FALSE;
                        qDebug("MESSAGE : slave %d found\n",slave);
                  }
                }
            }
            if(!ec_group[currentgroup].docheckstate)
                qDebug("OK : all slaves resumed OPERATIONAL.\n");
        }
        osal_usleep(10000);
    }
}

EthercatMaster::EthercatMaster()
{

}

QStringList EthercatMaster::scanNetwork(){
    return ScanHardware::ScanNetworkCard();
}


///
/// \brief EthercatMaster::init
/// \param network_id
/// \return     0：正常  -1：网卡不存在  -2：无法初始化网卡 -3：没有找到从站设备
///
qint32 EthercatMaster::init(quint32 network_id){
    ecClose();
    int i, j, oloop, iloop, wkc_count, chk, slc;
    needlf = FALSE;
    inOP = FALSE;

    ethercat_servo_map.clear();

    QString name = ScanHardware::GetNetworkName(network_id);
    if(!name.size()){
        return -1;
    }
    /* create thread to handle slave error handling in OP */
    osal_thread_create(&thread1, 128000, &ecatcheck, (void*) &ctime);
    char str[1028];
    strcpy(str,name.toStdString().data());
    qint32 ret = ec_init(str);
    if(ret){
        qDebug("ec_init %s",str);
        if(ec_config_init(false) > 0){

            qDebug("find and auto-config slaves");

            while(EcatError) qDebug("%s", ec_elist2string());
            qDebug("%d slaves found and configured.\n",ec_slavecount);
            if((ec_slavecount >= 1))
            {
                for(slc = 1; slc <= ec_slavecount; slc++)
                {
//                    // beckhoff EL7031, using ec_slave[].name is not very reliable
//                    if((ec_slave[slc].eep_man == 0x00000002) && (ec_slave[slc].eep_id == 0x1b773052))
//                    {
//                        printf("Found %s at position %d\n", ec_slave[slc].name, slc);
//                        // link slave specific setup to preop->safeop hook
////                        ec_slave[slc].PO2SOconfig = &EL7031setup;
//                    }
//                    // Copley Controls EAP, using ec_slave[].name is not very reliable
//                    else if((ec_slave[slc].eep_man == 0x000000ab) && (ec_slave[slc].eep_id == 0x00000380))
//                    {
//                        printf("Found %s at position %d\n", ec_slave[slc].name, slc);
//                        // link slave specific setup to preop->safeop hook
////                        ec_slave[slc].PO2SOconfig = &AEPsetup;
//                    }

//                    // Copley Controls yk2405, using ec_slave[].name is not very reliable
//                    else if((ec_slave[slc].eep_man == 0x00000994) && (ec_slave[slc].eep_id == 0x00002000))
//                    {
//                        printf("Found %s at position %d\n", ec_slave[slc].name, slc);
//                        // link slave specific setup to preop->safeop hook
////                        ec_slave[slc].PO2SOconfig = &Yk2405Setup;
//                    }
//                    else if((ec_slave[slc].eep_man == 0x00000766) && (ec_slave[slc].eep_id == 0x0000501))
//                    {
//                        printf("Found %s at position %d\n", ec_slave[slc].name, slc);
//                        // link slave specific setup to preop->safeop hook
////                        ec_slave[slc].PO2SOconfig = &HC2000setup;
//                    }

                    /// 添加从站和伺服对应关系
                    if(ethercatservo_szhc.is_surport(&(ec_slave[slc]))){
                        ethercat_servo_map.insert(slc,&ethercatservo_szhc);
                    }
                    if(ethercatservo_single.is_surport(&(ec_slave[slc]))){
                        ethercat_servo_map.insert(slc,&ethercatservo_single);
                    }

                    // 根据从站类型配置pdo
                    if(ethercat_servo_map.contains(slc)){
                        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slc);
                        if(slave_config!=NULL){
                            ec_slave[slc].PO2SOconfig = slave_config->setup_pdo_config;
                            QVector<AxisMotion*> axis ;
                            for(int k = 0;k<slave_config->axis_num();k++){
                                AxisMotion* axis_motion = new AxisMotion;
                                axis.push_back(axis_motion);
                            }
                            ethercat_servo_axis_map.insert(slc,axis);

                        }
                    }
                    printf("find slave %d : man = %x  id = %x \n",slc,ec_slave[slc].eep_man,ec_slave[slc].eep_id);
                }
            }

            ec_config_map(&IOmap);
            ec_configdc();
            IOmap16 = (int16_t*)IOmap;
            qDebug("Slaves mapped, state to SAFE_OP.\n");
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

            oloop = ec_slave[0].Obytes;
            if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
            if (oloop > 8) oloop = 8;
            iloop = ec_slave[0].Ibytes;
            if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
            if (iloop > 8) iloop = 8;

            printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

            qDebug("Request operational state for all slaves\n");
            expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            qDebug("Calculated workcounter %d\n", expectedWKC);

            ec_slave[0].state = EC_STATE_OPERATIONAL;
            /* send one valid process data to make outputs in slaves happy*/
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);

            /* start RT thread as periodic MM timer */
            mmResult = timeSetEvent(1, 0, RTthread, 0, TIME_PERIODIC);

            /* request OP state for all slaves */
            ec_writestate(0);

            for(int j = 1;j<=ec_slavecount;j++){
                ec_slave[j].state = EC_STATE_OPERATIONAL;
                ec_writestate(j);
                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
            }

            chk = 40;
            /* wait for all slaves to reach OP state */
            do
            {
                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
                qApp->processEvents();
            }
            while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
            if (ec_slave[0].state == EC_STATE_OPERATIONAL )
            {
                for(int j = 1;j<=ec_slavecount;j++){
                    if(ethercat_servo_map.contains(j)){
                        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(j);
                        if(slave_config!=NULL){
                            slave_config->setup_config(&(ec_slave[j]));
                        }
                    }
                }
                qDebug("Operational state reached for all slaves.%d  %d  %d \n",ec_slave[0].state,ec_slave[1].state,ec_slave[2].state);
//                printf("Operational state reached for all slaves.\n");
            }
            else{
                qDebug("Not all slaves reached operational state. %d  %d  %d\n",ec_slave[0].state,ec_slave[1].state,ec_slave[2].state);
            }
            return 0;
        }
        else{
            qDebug("Not find and auto-config slaves");
            return -3;
        }
    }
    return -2;
}

Q_INVOKABLE void EthercatMaster::ecClose(){
    if(thread1){
        CloseHandle(thread1);
    }
    if(mmResult){
        timeKillEvent(mmResult);
    }
    QMap<int,QVector<AxisMotion*>>::const_iterator  it = ethercat_servo_axis_map.begin();
    while(it != ethercat_servo_axis_map.end()){
        QVector<AxisMotion*> axis_motion = it.value();
        for(int i = 0;i<axis_motion.length();i++){
            delete  axis_motion.at(i);
        }
        axis_motion.clear();
        ++it;
    }
    ethercat_servo_axis_map.clear();
    ec_close();
}

QStringList EthercatMaster::getSlaveNameList(){
    QStringList str;
    quint32 count = getSlaveCount();
    for(quint32 i =0;i<count;i++){
        ec_slavet* slave = &ec_slave[i+1];
        str.append(QString::number(i)+":"+slave->name);
    }
    return str;
}

quint32 EthercatMaster::getSlaveCount(){

#ifdef SLAVE_TEST
    return 8;
#endif
    return ec_slavecount;
}


quint32 EthercatMaster::getSlaveState(quint32 slave_id){
    ec_readstate();
    return ec_slave[slave_id+1].state;
}

quint32 EthercatMaster::getSlaveIslost(quint32 slave_id){
    ec_readstate();
    if(!ec_slave[slave_id+1].state){
        ec_slave[slave_id+1].islost = 1;
    }
    else{
        ec_slave[slave_id+1].islost = 0;
    }
    return ec_slave[slave_id+1].islost;
}

QString EthercatMaster::getSlaveInfo(quint32 slave_id){
    ec_readstate();
    QJsonObject info;
    ec_slavet* slave = &ec_slave[slave_id+1];

#ifdef SLAVE_TEST
    slave->configadr = 4097+slave_id;
    slave->state = 0x02;
    slave->aliasadr = slave_id;
    slave->eep_man  = 0x092b;
    slave->eep_id = 0x501;
    sprintf(slave->name,"%s","Ethercat Device");
#endif

    info.insert("ec_state",slave->state);
    info.insert("ALstatuscode",slave->ALstatuscode);
    info.insert("configadr",slave->configadr);
    info.insert("aliasadr",slave->aliasadr);
    info.insert("eep_man",(int)slave->eep_man);
    info.insert("eep_id",(int)slave->eep_id);
    info.insert("eep_rev",(int)slave->eep_rev);

    info.insert("Itype",slave->Itype);
    info.insert("Dtype",slave->Dtype);
    info.insert("Obits",slave->Obits);
    info.insert("Ibits",slave->Ibits);
    info.insert("Ibytes",(int)slave->Ibytes);
    info.insert("Ostartbit",slave->Ostartbit);

//    info.insert("inputs",&(slave->inputs[0]));
//    info.insert("outputs",&(slave->outputs[0]));

    info.insert("Obytes",(int)slave->Obytes);
    info.insert("name",slave->name);
    info.insert("SIIindex",slave->SIIindex);
    info.insert("parent",slave->parent);
    info.insert("parentport",slave->parentport);
    info.insert("entryport",slave->entryport);
    info.insert("eep_8byte",slave->eep_8byte);
    info.insert("eep_pdi",slave->eep_pdi);
    info.insert("group",slave->group);
    info.insert("islost",slave->islost);
    return QString(QJsonDocument(info).toJson());
}

qint32 EthercatMaster::getLastWorkCounter(){
    return wkc;
}

qint32 EthercatMaster::readSdo(quint32 slave_id,quint32 main_index,quint32 sub_index,quint32 size){
    int32 rdl = size;
    int8_t u8 = 0;
    int16_t u16 = 0;
    int32_t u32 = 0;
    wkc = 0;
    ++slave_id;
    if(size == 0){
        rdl = sizeof (u8);
        wkc = ec_SDOread(slave_id, main_index, sub_index, FALSE, &rdl, &u8, EC_TIMEOUTRXM);
//        qDebug("readSdo8 %d %d %d %d %d %d",slave_id,main_index,sub_index,rdl,u8,wkc);
        return u8;
    }
    else if(size == 1){
        rdl = sizeof (u16);
        wkc = ec_SDOread(slave_id, main_index, sub_index, FALSE, &rdl, &u16, EC_TIMEOUTRXM);
//        qDebug("readSdo16 %d %d %d %d %d %d",slave_id,main_index,sub_index,rdl,u16,wkc);
        return u16;
    }
    else if(size == 2){
        rdl = sizeof (u32);
        wkc = ec_SDOread(slave_id, main_index, sub_index, FALSE, &rdl, &u32, EC_TIMEOUTRXM);
//        qDebug("readSdo32 %d %d %d %d %d %d",slave_id,main_index,sub_index,rdl,u32,wkc);
        return u32;
    }
    return 0;
}

qint32 EthercatMaster::writeSdo(quint32 slave_id,quint32 main_index,quint32 sub_index,quint32 size,qint32 value){
    int32 rdl = size;
    int8_t u8 = value;
    int16_t u16 = value;
    int32_t u32 = value;
    wkc = 0;
    ++slave_id;
    if(size == 0){
        rdl = sizeof (u8);
        wkc = ec_SDOwrite(slave_id, main_index, sub_index, FALSE, rdl, &u8, EC_TIMEOUTRXM);
        qDebug("writeSdo8 %d %d %d %d %d %d",slave_id,main_index,sub_index,rdl,u8,wkc);
        return u8;
    }
    else if(size == 1){
        rdl = sizeof (u16);
        wkc = ec_SDOwrite(slave_id, main_index, sub_index, FALSE, rdl, &u16, EC_TIMEOUTRXM);
        qDebug("writeSdo16 %d %d %d %d %d %d",slave_id,main_index,sub_index,rdl,u16,wkc);
        return u16;
    }
    else if(size == 2){
        rdl = sizeof (u32);
        wkc = ec_SDOwrite(slave_id, main_index, sub_index, FALSE, rdl, &u32, EC_TIMEOUTRXM);
        qDebug("writeSdo32 %d %d %d %d %d %d",slave_id,main_index,sub_index,rdl,u32,wkc);
        return u32;
    }
    return 0;
}

qint32 EthercatMaster::writeAlias(quint32 slave_id,quint32 alias_id){
    ++slave_id;
//    wkc = 0;
//    ec_slavet* slave = &ec_slave[slave_id];
//    slave->aliasadr = alias_id;
//    wkc = ec_writealias(slave_id);

//    qDebug("writeAlias slave_id = %d ,alias_id = %d ,ret = %d ",slave_id,alias_id,wkc);

    ec_writealias2(slave_id,alias_id);
    return wkc;
}

QString EthercatMaster::readFile(const QString& path){
    QString str = path;
    str = str.mid(8);
    QFile file(str);
    str="";
    if(file.open(QFile::ReadOnly)){
        str = file.readAll();
    }
    else{
        qDebug()<<path<<"readFile open failed";
    }
    return str.toUtf8();
}

qint32 EthercatMaster::writeFile(const QString& path,const QString& text){
    QString str = path;
    str = str.mid(8);
    QFile file(str);
    str="";
    if(file.open(QFile::ReadWrite)){
        file.resize(0);
        file.write(text.toUtf8());
    }
    else{
        qDebug()<<path<<"writeFile open failed";
    }
    return 0;
}

QString EthercatMaster::readSII(quint32 slave_id){
    char out[1024]={0};
    memset(out,0,sizeof (out));
    ec_read_sii_infomation(slave_id+1,out);
    return QString(out);
}


qint32 EthercatMaster::updateSlaveFirm(quint32 slave_id,const QString path){
    ++slave_id;
    int offset = 0;
    int buffSize = 2048;


    QString str = path;
    str = str.mid(8);
    QFile file(str);
    QByteArray byte_array;
    if(file.open(QFile::ReadOnly)){
        byte_array = file.readAll();
    }
    else{
        qDebug()<<path<<"readFile open failed";
        return -1;
    }

    uint8_t value[5] = {3,0,0,0,0};
    uint8_t data[2048] = {0};
    uint8_t data_addr[4] = {0};
    int32 rdl = sizeof(value);

    while (1) {
        qDebug()<<"Wait to enter update start:"<<byte_array.length();

        ec_SDOwrite(slave_id, 0x5000, 1, FALSE, sizeof (value), value, EC_TIMEOUTRXM);

        value[0] = 0;
        ec_SDOread(slave_id, 0x5000, 1, FALSE, &rdl, value, EC_TIMEOUTRXM);

        if(value[0]  == 3){
            while (1) {
                value[0] = 0x55;
                value[1] = 0;
                ec_SDOwrite(slave_id, 0x5000, 1, FALSE, sizeof (value), value, EC_TIMEOUTRXM);
                int sec_count = byte_array.length() / (64 * 1024) + 1;
                qDebug()<<"use blcok count:"<<sec_count;
                for (int i = 0; i < sec_count;++i) {
                    value[0] = 0x56;
                    value[1] = 0;
                    value[2] = i;
                    value[3] = 1;
                    ec_SDOwrite(slave_id, 0x5000, 1, FALSE, sizeof (value), value, EC_TIMEOUTRXM);
                }
                ec_SDOread(slave_id, 0x5000, 1, FALSE, &rdl, value, EC_TIMEOUTRXM);
                if (value[0] == 0) {
                    break;
                }
                qApp->processEvents();
            }
            qDebug() << "Entered update process!";
            break;
        }
        qApp->processEvents();
    }
    int sent_count = (byte_array.length() - offset) / buffSize;
    int last_sent = (byte_array.length() - offset) % buffSize;
    char *fp = byte_array.data() + offset;
    qDebug()<<"sent info:"<<sent_count<<" mod:"<<last_sent;
    for (int i = 0; i < sent_count; ++i) {
        qDebug()<<"-------"<<i<<"/"<<sent_count;
        ec_SDOread(slave_id, 0x5000, 1, FALSE, &rdl, value, EC_TIMEOUTRXM);
        qApp->processEvents();
        if (value[0] == 0) {
            data_addr[0] = i & 0xFF;
            data_addr[1] = (i >> 8) & 0xFF;
//            m.sdoDownload(&data_addr);
            ec_SDOwrite(slave_id, 0x5000, 2, FALSE, sizeof (data_addr), data_addr, EC_TIMEOUTRXM);      //< 写数据地址

             memcpy(data, fp,buffSize);
//            m.sdoDownload(&data);
            ec_SDOwrite(slave_id, 0x5001, 0, FALSE, sizeof (data), data, EC_TIMEOUTRXM);      //< 写数据


//            m.sdoDownload(&write_can_sent_flag);
            value[0] = 1;
            value[1] = 0;
            ec_SDOwrite(slave_id, 0x5000, 1, FALSE, sizeof (value), value, EC_TIMEOUTRXM);

            fp += buffSize;

        }
        else
        {
            //          --i;
        }
    }
    qDebug()<<"+++++++"<<last_sent<<"/"<<sent_count;
    ec_SDOread(slave_id, 0x5000, 1, FALSE, &rdl, value, EC_TIMEOUTRXM);
    if (last_sent && value[0] == 0) {
        data_addr[0] = sent_count & 0xFF;
        data_addr[1] = (sent_count >> 8) & 0xFF;
        ec_SDOwrite(slave_id, 0x5000, 2, FALSE, sizeof (data_addr), data_addr, EC_TIMEOUTRXM);      //< 写数据地址

        memcpy(data, fp,last_sent);
        ec_SDOwrite(slave_id, 0x5001, 0, FALSE, last_sent, data, EC_TIMEOUTRXM);      //< 写数据

        value[0] = 1;
        value[1] = 0;
        ec_SDOwrite(slave_id, 0x5000, 1, FALSE, sizeof (value), value, EC_TIMEOUTRXM);
        qDebug()<<"******"<<last_sent<<"/"<<sent_count;
    }
    else
    {
        //      --i;
    }

    value[0] = 2;
    value[1] = 0;
    ec_SDOwrite(slave_id, 0x5000, 1, FALSE, sizeof (value), value, EC_TIMEOUTRXM);
    qDebug()<<"update finished";
    return 0;
}

void EthercatMaster::clearServoAlarm(quint32 slave_id,quint32 sub_id){
     ++slave_id;
    ec_slavet* slave = &ec_slave[slave_id];
    if(ethercat_servo_map.contains(slave_id)){
        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slave_id);
        if(slave_config!=NULL){
            slave_config->clear_alarm(slave,sub_id);
        }
    }
}

qint32 EthercatMaster::getServoAlarm(quint32 slave_id,quint32 sub_id){
    ++slave_id;
    ec_slavet* slave = &ec_slave[slave_id];
    if(ethercat_servo_map.contains(slave_id)){
        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slave_id);
        if(slave_config!=NULL){
            return slave_config->get_servo_alarm(slave,sub_id);
        }
    }
    return 0;
}

void EthercatMaster::servoOn(quint32 slave_id,quint32 sub_id,bool state){
     ++slave_id;
    ec_slavet* slave = &ec_slave[slave_id];
    if(ethercat_servo_map.contains(slave_id)){
        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slave_id);
        if(slave_config!=NULL){
            slave_config->servo_on(slave,sub_id,state);
        }
    }
}


qint32 EthercatMaster::getServoPos(quint32 slave_id,quint32 sub_id){
     ++slave_id;
    ec_slavet* slave = &ec_slave[slave_id];
    if(ethercat_servo_map.contains(slave_id)){
        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slave_id);
        if(slave_config!=NULL){
            return slave_config->get_servo_pos(slave,sub_id);
        }
    }
    return 0;
}

qint32 EthercatMaster::getServoOn(quint32 slave_id,quint32 sub_id){
    ++slave_id;
    ec_slavet* slave = &ec_slave[slave_id];
    if(ethercat_servo_map.contains(slave_id)){
        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slave_id);
        if(slave_config!=NULL){
            return slave_config->get_servo_on(slave,sub_id);
        }
    }
    return 0;
}


qint32 EthercatMaster::getServoCmdPos(quint32 slave_id,quint32 sub_id){
    ++slave_id;
    ec_slavet* slave = &ec_slave[slave_id];
    if(ethercat_servo_map.contains(slave_id)){
        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slave_id);
        if(slave_config!=NULL){
            return slave_config->get_servo_cmd_pos(slave,sub_id);
        }
    }
    return 0;
}

qint32 EthercatMaster::getServoTorque(quint32 slave_id,quint32 sub_id){
    ++slave_id;
    ec_slavet* slave = &ec_slave[slave_id];
    if(ethercat_servo_map.contains(slave_id)){
        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slave_id);
        if(slave_config!=NULL){
            return slave_config->get_servo_torque(slave,sub_id);
        }
    }
    return 0;
}

qint32 EthercatMaster::getServoVelocity(quint32 slave_id,quint32 sub_id){
    ++slave_id;
    ec_slavet* slave = &ec_slave[slave_id];
    if(ethercat_servo_map.contains(slave_id)){
        const _EthercatSlaveConfig* slave_config = ethercat_servo_map.value(slave_id);
        if(slave_config!=NULL){
            return slave_config->get_servo_velocity(slave,sub_id);
        }
    }
    return 0;
}


AxisMotion* EthercatMaster::getMotionAxis(quint32 slave_id,quint32 sub_id){
    ++slave_id;
    if(ethercat_servo_axis_map.contains(slave_id)){
        QVector<AxisMotion*> axis_motion = ethercat_servo_axis_map.value(slave_id);
        if(sub_id < axis_motion.length()){
            return axis_motion.at(sub_id);
        }
    }
    return NULL;
}


qint32 EthercatMaster::setAxisJog(quint32 slave_id,quint32 sub_id,qint32 speed){
    AxisMotion*  axis = getMotionAxis(slave_id,sub_id);
    if(axis == NULL){
        return -1;
    }
    axis->SetAxisMotionJog(speed);
    return 0;
}

qint32 EthercatMaster::setAxisStop(quint32 slave_id,quint32 sub_id){
    AxisMotion*  axis = getMotionAxis(slave_id,sub_id);
    if(axis == NULL){
        return -1;
    }
    axis->SetAxisStop();
    return 0;
}


