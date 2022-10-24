HEADERS += \
    $$PWD/nicdrv.h \
    $$PWD/oshw.h

SOURCES += \
    $$PWD/nicdrv.c \
    $$PWD/oshw.c

INCLUDEPATH += $$PWD

contains(QT_ARCH,i386){
LIBS+= $$PWD/x86/winmm.lib
LIBS += $$PWD/x86/ws2_32.lib
}

contains(QT_ARCH,x86_64){
LIBS+= $$PWD/winmm.lib
LIBS += $$PWD/ws2_32.lib
}
