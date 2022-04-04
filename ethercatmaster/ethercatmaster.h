#ifndef ETHERCATMASTER_H
#define ETHERCATMASTER_H

#include <QObject>
#include <QStringList>

#include "ethercatmain.h"
#include "ethercatconfig.h"
#include "ethercatdc.h"
#include "ethercatprint.h"
#include "ethercatcoe.h"
#include "ethercatbase.h"


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

    ///
    /// \brief getSlaveCount    获取从站数量
    /// \return
    ///
    Q_INVOKABLE quint32 getSlaveCount();

    ///
    /// \brief getSlaveState    获取从站状态
    /// \param slave
    /// \return
    ///
    Q_INVOKABLE quint32 getSlaveState(quint32 slave_id);

    ///
    /// \brief getSlaveInfo     获取从站信息
    /// \param slave
    /// \return
    ///
    Q_INVOKABLE QString getSlaveInfo(quint32 slave_id);

private:
    char IOmap[4096];
    ec_ODlistt ODlist;
    ec_OElistt OElist;
    boolean printSDO = FALSE;
    boolean printMAP = FALSE;
    char usdo[128];
    char hstr[1024];
};

#endif // ETHERCATMASTER_H
