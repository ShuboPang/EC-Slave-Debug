HEADERS += \
    $$PWD/Include/Packet32.h \
    $$PWD/Include/Win32-Extensions.h \
    $$PWD/Include/bittypes.h \
    $$PWD/Include/ip6_misc.h \
    $$PWD/Include/pcap-bpf.h \
    $$PWD/Include/pcap-namedb.h \
    $$PWD/Include/pcap-stdinc.h \
    $$PWD/Include/pcap.h \
    $$PWD/Include/pcap/bluetooth.h \
    $$PWD/Include/pcap/bpf.h \
    $$PWD/Include/pcap/namedb.h \
    $$PWD/Include/pcap/pcap.h \
    $$PWD/Include/pcap/sll.h \
    $$PWD/Include/pcap/usb.h \
    $$PWD/Include/pcap/vlan.h \
    $$PWD/Include/remote-ext.h

contains(QT_ARCH,i386){
LIBS +=  $$PWD/Lib/Packet.lib
LIBS +=  $$PWD/Lib/wpcap.lib
}

contains(QT_ARCH,x86_64){
LIBS +=  $$PWD/Lib/x64/Packet.lib
LIBS +=  $$PWD/Lib/x64/wpcap.lib
}

INCLUDEPATH+=$$PWD/include/
