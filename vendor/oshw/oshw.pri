HEADERS += \
    $$PWD/nicdrv.h \
    $$PWD/oshw.h

SOURCES += \
    $$PWD/nicdrv.c \
    $$PWD/oshw.c

INCLUDEPATH += $$PWD
LIBS+= $$PWD/winmm.lib
LIBS += $$PWD/ws2_32.lib
