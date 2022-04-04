#ifndef SCANHARDWARE_H
#define SCANHARDWARE_H


#include <QStringList>

class ScanHardware
{
public:
    ScanHardware();

    static QStringList ScanNetworkCard();

    static QString GetNetworkName(quint32 index);

private:
    static QStringList network_card_name_list;

};

#endif // SCANHARDWARE_H
