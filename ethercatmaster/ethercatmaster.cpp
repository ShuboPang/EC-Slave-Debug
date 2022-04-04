#include "ethercatmaster.h"
#include "scanhardware/scanhardware.h"
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

extern boolean EcatError;


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
    QString name = ScanHardware::GetNetworkName(network_id);
    if(!name.size()){
        return -1;
    }
    char str[1028];
    strcpy(str,name.toStdString().data());
    qint32 ret = ec_init(str);
    if(ret){
        qDebug("ec_init %s",str);
        if(ec_config(false, &IOmap) > 0){
            qDebug("find and auto-config slaves");
            ec_configdc();
            while(EcatError) qDebug("%s", ec_elist2string());
            qDebug("%d slaves found and configured.\n",ec_slavecount);
            qint32 expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
             printf("Calculated workcounter %d\n", expectedWKC);
             /* wait for all slaves to reach SAFE_OP state */
             ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 3);
             if (ec_slave[0].state != EC_STATE_SAFE_OP )
             {
                qDebug("Not all slaves reached safe operational state.\n");
                ec_readstate();
                for(quint32 i = 1; i<=ec_slavecount ; i++)
                {
                   if(ec_slave[i].state != EC_STATE_SAFE_OP)
                   {
                      qDebug("Slave %d State=%2x StatusCode=%4x : %s\n",
                         i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                   }
                }
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

quint32 EthercatMaster::getSlaveCount(){
    return ec_slavecount;
}

quint32 EthercatMaster::getSlaveState(quint32 slave_id){
    return ec_slave[slave_id].state;
}

QString EthercatMaster::getSlaveInfo(quint32 slave_id){
    QJsonObject info;
    ec_slavet* slave = &ec_slave[slave_id];
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
    info.insert("Obytes",(int)slave->Obytes);
    info.insert("name",slave->name);
    return QString(QJsonDocument(info).toJson());
}

qint32 EthercatMaster::readSdo(quint32 slave_id,quint32 main_index,quint32 sub_index,quint32 size){
    if(size == 0){
        size = 1;
    }
    int32 rdl = size;
    int8_t u8 = 0;
    int16_t u16 = 0;
    int32_t u32 = 0;
    if(size == 1){
        ec_SDOread(slave_id, main_index, sub_index, FALSE, &rdl, &u8, EC_TIMEOUTRXM);
        return u8;
    }
    else if(size == 1){
        ec_SDOread(slave_id, main_index, sub_index, FALSE, &rdl, &u16, EC_TIMEOUTRXM);
        return u16;
    }
    else if(size == 1){
        ec_SDOread(slave_id, main_index, sub_index, FALSE, &rdl, &u32, EC_TIMEOUTRXM);
        return u32;
    }
    return 0;
}

qint32 EthercatMaster::writeSdo(quint32 slave_id,quint32 main_index,quint32 sub_index,quint32 size,qint32 value){
    if(size == 0){
        size = 1;
    }
    int32 rdl = size;
    int8_t u8 = value;
    int16_t u16 = value;
    int32_t u32 = value;
    if(size == 1){
        ec_SDOwrite(slave_id, main_index, sub_index, FALSE, rdl, &u8, EC_TIMEOUTRXM);
        return u8;
    }
    else if(size == 1){
        ec_SDOwrite(slave_id, main_index, sub_index, FALSE, rdl, &u16, EC_TIMEOUTRXM);
        return u16;
    }
    else if(size == 1){
        ec_SDOwrite(slave_id, main_index, sub_index, FALSE, rdl, &u32, EC_TIMEOUTRXM);
        return u32;
    }
    return 0;
}
