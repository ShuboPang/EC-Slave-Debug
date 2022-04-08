#ifndef ETHERCATMASTER_H
#define ETHERCATMASTER_H

#include <QObject>
#include <QStringList>

#include "ethercattype.h"
#include "ethercatmain.h"
#include "ethercatconfig.h"
#include "ethercatdc.h"
#include "ethercatprint.h"
#include "ethercatcoe.h"
#include "ethercatbase.h"

#include "axismotion.h"


class EthercatMaster:public QObject
{
    Q_OBJECT
public:
    EthercatMaster();

    ///
    /// \brief scanNetwork      扫描网卡
    /// \return
    ///
    Q_INVOKABLE QStringList scanNetwork();

    ///
    /// \brief init             初始化网卡
    /// \param network_id
    /// \return
    ///
    Q_INVOKABLE qint32 init(quint32 network_id);


    Q_INVOKABLE void ecClose();
    ///
    /// \brief getSlaveCount    获取从站数量
    /// \return
    ///
    Q_INVOKABLE quint32 getSlaveCount();

    ///
    /// \brief getSlaveNameList
    /// \return
    ///
    Q_INVOKABLE QStringList getSlaveNameList();

    ///
    /// \brief getSlaveState    获取从站状态
    /// \param slave
    /// \return
    ///
    Q_INVOKABLE quint32 getSlaveState(quint32 slave_id);


    Q_INVOKABLE quint32 getSlaveIslost(quint32 slave_id);

    ///
    /// \brief getSlaveInfo     获取从站信息
    /// \param slave
    /// \return
    ///
    Q_INVOKABLE QString getSlaveInfo(quint32 slave_id);


    ///
    /// \brief readSdo
    /// \param slave_id
    /// \param main_index
    /// \param sub_index
    /// \param size
    /// \return
    ///
    Q_INVOKABLE qint32 readSdo(quint32 slave_id,quint32 main_index,quint32 sub_index,quint32 size);

    ///
    /// \brief writeSdo
    /// \param slave_id
    /// \param main_index
    /// \param sub_index
    /// \param size
    /// \param value
    /// \return
    ///
    Q_INVOKABLE qint32 writeSdo(quint32 slave_id,quint32 main_index,quint32 sub_index,quint32 size,qint32 value);

    Q_INVOKABLE qint32 getLastWorkCounter();

    Q_INVOKABLE qint32 writeAlias(quint32 slave_id,quint32 alias_id);

    Q_INVOKABLE QString readFile(const QString& path);

    Q_INVOKABLE qint32 writeFile(const QString& path,const QString& text);

    Q_INVOKABLE QString readSII(quint32 slave_id);



    Q_INVOKABLE void clearServoAlarm(quint32 slave_id, quint32 sub_id);

    Q_INVOKABLE qint32 getServoAlarm(quint32 slave_id,quint32 sub_id);

    Q_INVOKABLE void servoOn(quint32 slave_id, quint32 sub_id, bool state);

    Q_INVOKABLE qint32 getServoPos(quint32 slave_id,quint32 sub_id);

    Q_INVOKABLE qint32 getServoOn(quint32 slave_id,quint32 sub_id);

    Q_INVOKABLE qint32 getServoTorque(quint32 slave_id,quint32 sub_id);

    Q_INVOKABLE qint32 getServoCmdPos(quint32 slave_id,quint32 sub_id);

    Q_INVOKABLE qint32 getServoVelocity(quint32 slave_id,quint32 sub_id);

    Q_INVOKABLE qint32 setAxisJog(quint32 slave_id,quint32 sub_id,qint32 speed);

    Q_INVOKABLE qint32 setAxisStop(quint32 slave_id,quint32 sub_id);

    Q_INVOKABLE float getFirmwareUpdateValue(){
        return firmware_update_value;
    }

    Q_INVOKABLE qint32 updateSlaveFirm(quint32 slave_id, const QString path);

private:
    AxisMotion* getMotionAxis(quint32 slave_id,quint32 sub_id);

private:
    boolean printSDO = FALSE;
    boolean printMAP = FALSE;
    char usdo[128];
    char hstr[1024];

    int wkc;

    float firmware_update_value;
};

#endif // ETHERCATMASTER_H
