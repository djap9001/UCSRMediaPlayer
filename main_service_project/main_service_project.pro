TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ../main_service/src/main.cpp \
    ../main_service/proto/main_service.pb.cc \
    ../djap_utils/src/condition_mutex.cpp \
    ../djap_utils/src/Exception.cpp \
    ../djap_utils/src/mutex.cpp \
    ../djap_utils/src/read_write_lock.cpp \
    ../djap_utils/src/mutex_guard.cpp \
    ../djap_utils/src/shared_pointer_base.cpp \
    ../djap_utils/src/thread.cpp \
    ../main_service/src/static_http_content_service.cpp

HEADERS += \
    ../djap_utils/include/condition_mutex.hpp \
    ../djap_utils/include/Exception.hpp \
    ../djap_utils/include/mutex.hpp \
    ../djap_utils/include/read_write_lock.hpp \
    ../djap_utils/include/mutex_guard.hpp \
    ../djap_utils/include/shared_pointer.hpp \
    ../djap_utils/include/shared_pointer_base.hpp \
    ../djap_utils/include/singleton.hpp \
    ../djap_utils/include/thread.hpp \
    ../djap_utils/include/weak_pointer.hpp \
    ../player_interface/player_service.pb.h \
    ../main_service/proto/main_service.pb.h \
    ../main_service/include/static_http_content_service.hpp

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
