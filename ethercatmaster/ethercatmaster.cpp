#include "ethercatmaster.h"
#include "scanhardware/scanhardware.h"
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#define EC_TIMEOUTMON 500

OSAL_THREAD_HANDLE thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
volatile int rtcnt;
boolean inOP;
uint8 currentgroup = 0;
char IOmap[4096];
ec_ODlistt ODlist;
ec_OElistt OElist;

/* most basic RT thread for process data, just does IO transfer */
void CALLBACK RTthread(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1,  DWORD_PTR dw2)
{
    IOmap[6]++;
    ec_send_processdata();
    wkc = ec_receive_processdata(EC_TIMEOUTRET);
    rtcnt++;
    /* do RT control stuff here */
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
    int i, j, oloop, iloop, wkc_count, chk, slc;
    UINT mmResult;

    needlf = FALSE;
    inOP = FALSE;

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
                    // beckhoff EL7031, using ec_slave[].name is not very reliable
                    if((ec_slave[slc].eep_man == 0x00000002) && (ec_slave[slc].eep_id == 0x1b773052))
                    {
                        printf("Found %s at position %d\n", ec_slave[slc].name, slc);
                        // link slave specific setup to preop->safeop hook
//                        ec_slave[slc].PO2SOconfig = &EL7031setup;
                    }
                    // Copley Controls EAP, using ec_slave[].name is not very reliable
                    else if((ec_slave[slc].eep_man == 0x000000ab) && (ec_slave[slc].eep_id == 0x00000380))
                    {
                        printf("Found %s at position %d\n", ec_slave[slc].name, slc);
                        // link slave specific setup to preop->safeop hook
//                        ec_slave[slc].PO2SOconfig = &AEPsetup;
                    }

                    // Copley Controls yk2405, using ec_slave[].name is not very reliable
                    else if((ec_slave[slc].eep_man == 0x00000994) && (ec_slave[slc].eep_id == 0x00002000))
                    {
                        printf("Found %s at position %d\n", ec_slave[slc].name, slc);
                        // link slave specific setup to preop->safeop hook
//                        ec_slave[slc].PO2SOconfig = &Yk2405Setup;
                    }

                    printf("find slave %d : man = %x  id = %x \n",slc,ec_slave[slc].eep_man,ec_slave[slc].eep_id);
                }
            }

            ec_config_map(&IOmap);
            ec_configdc();


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
            chk = 40;
            /* wait for all slaves to reach OP state */
            do
            {
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            }
            while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
            if (ec_slave[0].state == EC_STATE_OPERATIONAL )
            {
                qDebug("Operational state reached for all slaves.\n");
//                printf("Operational state reached for all slaves.\n");
//                wkc_count = 0;
//                inOP = TRUE;


//                /* cyclic loop, reads data from RT thread */
//                for(i = 1; i <= 500; i++)
//                {
//                    if(wkc >= expectedWKC)
//                    {
//                        printf("Processdata cycle %4d, WKC %d , O:", rtcnt, wkc);

//                        for(j = 0 ; j < oloop; j++)
//                        {
//                            printf(" %2.2x", *(ec_slave[0].outputs + j));
//                        }

//                        printf(" I:");
//                        for(j = 0 ; j < iloop; j++)
//                        {
//                            printf(" %2.2x", *(ec_slave[0].inputs + j));
//                        }
//                        printf(" T:%lld\r",ec_DCtime);
//                        needlf = TRUE;
//                    }
//                    osal_usleep(50000);

//                }
//                inOP = FALSE;
            }
            else{
                qDebug("Not all slaves reached operational state.\n");
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
    return ec_slave[slave_id+1].state;
}

QString EthercatMaster::getSlaveInfo(quint32 slave_id){
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
