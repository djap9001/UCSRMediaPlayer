TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../mplayer_service/src/main.cpp \
    ../mplayer_service/src/mplayer_controller.cpp \
    ../mplayer_service/src/mplayer_service.cpp \
    ../djap_utils/src/condition_mutex.cpp \
    ../djap_utils/src/Exception.cpp \
    ../djap_utils/src/mutex.cpp \
    ../djap_utils/src/mutex_guard.cpp \
    ../djap_utils/src/shared_pointer_base.cpp \
    ../djap_utils/src/thread.cpp \
    ../player_interface/player_service.pb.cc

HEADERS += \
    ../mplayer_service/include/mplayer_controller.hpp \
    ../mplayer_service/include/mplayer_service.hpp \
    ../djap_utils/include/condition_mutex.hpp \
    ../djap_utils/include/Exception.hpp \
    ../djap_utils/include/mutex.hpp \
    ../djap_utils/include/mutex_guard.hpp \
    ../djap_utils/include/shared_pointer.hpp \
    ../djap_utils/include/shared_pointer_base.hpp \
    ../djap_utils/include/singleton.hpp \
    ../djap_utils/include/thread.hpp \
    ../djap_utils/include/weak_pointer.hpp \
    ../player_interface/player_service.pb.h

INCLUDEPATH += \
    ../ \
    ../player_interface \
    ../third_party/brpc/include \
    ../mplayer_service/include


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

