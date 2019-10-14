TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ../main_service/src/main.cpp \
    ../main_service/proto/main_service.pb.cc

HEADERS += \
    ../djap_utils/include/condition_mutex.hpp \
    ../djap_utils/include/Exception.hpp \
    ../djap_utils/include/mutex.hpp \
    ../djap_utils/include/mutex_guard.hpp \
    ../djap_utils/include/shared_pointer.hpp \
    ../djap_utils/include/shared_pointer_base.hpp \
    ../djap_utils/include/singleton.hpp \
    ../djap_utils/include/thread.hpp \
    ../djap_utils/include/weak_pointer.hpp \
    ../player_interface/player_service.pb.h \
    ../main_service/proto/main_service.pb.h

INCLUDEPATH += \
    ../ \
    ../player_interface \
    ../third_party/brpc/include \
    ../main_service/include \
    ../main_service/proto


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

DISTFILES += \
    ../main_service/proto/main_service.proto
