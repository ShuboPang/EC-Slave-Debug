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


private:
    boolean printSDO = FALSE;
    boolean printMAP = FALSE;
    char usdo[128];
    char hstr[1024];

    int wkc;
};

#endif // ETHERCATMASTER_H
