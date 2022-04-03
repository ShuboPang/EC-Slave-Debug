#include "scanhardware.h"
#include "vendor/soem/include/ethercatmain.h"
#include <QDebug>


QStringList ScanHardware::network_card_name_list;


ScanHardware::ScanHardware()
{

}

QStringList ScanHardware::ScanNetworkCard(){
    qDebug("ScanNetworkCard  start!");
    QStringList network_card_list;
    network_card_name_list.clear();
    ec_adaptert * adapter = NULL;
    adapter = ec_find_adapters ();
    while (adapter != NULL)
    {
       qDebug ("Description : %s, Device to use for wpcap: %s\n", adapter->desc,adapter->name);
       network_card_list.append(adapter->desc);
       network_card_name_list.append(adapter->name);
       adapter = adapter->next;
    }
    qDebug("ScanNetworkCard!");
    return network_card_list;
}
