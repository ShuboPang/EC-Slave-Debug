HEADERS += \
    $$PWD/include/ethercatbase.h \
    $$PWD/include/ethercatcoe.h \
    $$PWD/include/ethercatconfig.h \
    $$PWD/include/ethercatconfiglist.h \
    $$PWD/include/ethercatdc.h \
    $$PWD/include/ethercatfoe.h \
    $$PWD/include/ethercatmain.h \
    $$PWD/include/ethercatprint.h \
    $$PWD/include/ethercatsoe.h \
    $$PWD/include/ethercattype.h


#LIBS+=  $$PWD/LIB/win32/libsoem.lib

INCLUDEPATH+=$$PWD/include/

SOURCES += \
    $$PWD/src/ethercatbase.c \
    $$PWD/src/ethercatcoe.c \
    $$PWD/src/ethercatconfig.c \
    $$PWD/src/ethercatdc.c \
    $$PWD/src/ethercatfoe.c \
    $$PWD/src/ethercatmain.c \
    $$PWD/src/ethercatprint.c \
    $$PWD/src/ethercatsoe.c
