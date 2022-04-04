QT += quick
QT += widgets
QT += core

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    EC-Slave-Debug_zh_HK.ts
CONFIG += lrelease
CONFIG += embed_translations


include("ethercatmaster/ethercatmaster.pri")
include("scanhardware/scanhardware.pri")
include("vendor/soem/soem.pri")
include("vendor/wpcap/wpcap.pri")
include("vendor/osal/osal.pri")
include("vendor/oshw/oshw.pri")
include("data/data.pri")




# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
