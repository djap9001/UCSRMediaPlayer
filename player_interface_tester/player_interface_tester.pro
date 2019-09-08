TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    ../player_interface/player_service.pb.cc

HEADERS += \
    ../player_interface/player_service.pb.h

INCLUDEPATH += \
    ../player_interface \
    ../third_party/brpc/include

unix:!macx: LIBS += -L$$PWD/../third_party/brpc/lib/ -lbrpc

INCLUDEPATH += $$PWD/../third_party/brpc/include
DEPENDPATH += $$PWD/../third_party/brpc/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../third_party/brpc/lib/libbrpc.a

unix:!macx: LIBS += -L$$PWD/../third_party/brpc/lib/ -lgflags

INCLUDEPATH += $$PWD/../third_party/brpc/include
DEPENDPATH += $$PWD/../third_party/brpc/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../third_party/brpc/lib/libgflags.a

unix:!macx: LIBS += -L$$PWD/../third_party/brpc/lib/ -lleveldb

INCLUDEPATH += $$PWD/../third_party/brpc/include
DEPENDPATH += $$PWD/../third_party/brpc/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../third_party/brpc/lib/libleveldb.a

unix:!macx: LIBS += -L$$PWD/../third_party/brpc/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../third_party/brpc/include
DEPENDPATH += $$PWD/../third_party/brpc/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../third_party/brpc/lib/libprotobuf.a

LIBS += \
    -lpthread -lssl -lcrypto -lz -ldl
