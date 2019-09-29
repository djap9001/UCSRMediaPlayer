#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include "mplayer_service.hpp"
/*
#include <iostream>
#include <unistd.h>
#include <time.h>
#include "djap_utils/include/mutex.hpp"
#include "djap_utils/include/condition_mutex.hpp"
#include "djap_utils/include/shared_pointer.hpp"
#include "djap_utils/include/thread.hpp"

using std::cout;
using std::endl;

class SharedPointerTester {
public:
    SharedPointerTester() {
        cout << "Shared pointer tester created" << endl;
    }
    ~SharedPointerTester() {
        cout << "Shared pointer tester deleted" << endl;
    }
};

class ThreadTester : public DjapUtils::Thread
{
protected:
    // implement
    void thread_main() {
        cout << "Thread tester started" << endl;
        cout << "Thread tester exitting" << endl;
        return;
    }
    virtual ~ThreadTester() {
        cout << "Thread tester deleted" << endl;
    }
    friend class DjapUtils::SharedPointer<ThreadTester>;
};

void* bthread_main(void* argc) {
// Test exception
    try {
        throw DjapUtils::Exception(std::string("Test exception!"));
    } catch (DjapUtils::Exception &e) {
        cout << "[Bthread] Caught test exception (THIS SHOULD HAPPEN): " << e.error_message() << endl;
    }

    // Test mutex and condition mutex against exceptions
    // TODO more tests once thread has been implemented.
    try {
        DjapUtils::Mutex test_mutex;
        DjapUtils::ConditionMutex test_condition;
        test_mutex.lock();
        test_condition.lock();
        test_condition.unlock();
        test_mutex.unlock();
        test_condition.Signal();    // shouldn't have any issues
        test_condition.Broadcast(); // shouldn't have any issues
    } catch (DjapUtils::Exception &e) {
        cout << "[Bthread] Caught an exception (SOMETHING IS WRONG HERE...): " << e.error_message() << endl;
    }

    // Test shared and weak pointers
    DjapUtils::WeakPointer<SharedPointerTester> weak_test;
    do {
        DjapUtils::SharedPointer<SharedPointerTester> assign_to_here;
        do {
            // Should delete once, TODO more tests later once implementation is complete
            DjapUtils::SharedPointer<SharedPointerTester> ptr1(new SharedPointerTester());
            DjapUtils::SharedPointer<SharedPointerTester> ptr2(ptr1);
            assign_to_here = ptr2;  // quick test assign operator
            weak_test = assign_to_here.to_weak();
        } while(0);
        DjapUtils::SharedPointer<SharedPointerTester> back_from_weak(weak_test);
        cout << "[Bthread] Shared pointer tester should be deleted in a moment..." << endl;
        printf("[Bthread] assign_to_here_raw: %p, back_from_weak: %p\n", assign_to_here.raw_ptr(), back_from_weak.raw_ptr());
    } while(0);
    DjapUtils::SharedPointer<SharedPointerTester> back_from_weak2(weak_test);
    printf("[Bthread] back_from_weak2: %p (should be nullptr now)\n", back_from_weak2.raw_ptr());

    DjapUtils::SharedPointer<ThreadTester> test_thread(DjapUtils::Thread::alloc<ThreadTester>());
    test_thread->start_urgent();
    test_thread->join();
    cout << "[Bthread] Ending.." << endl;
    cout << "bthread main will exit now" << endl;
    return nullptr;
}
*/

DEFINE_int32(port, 8000, "TCP Port of this server");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
             "read/write operations during the last `idle_timeout_s'");
DEFINE_int32(logoff_ms, 2000, "Maximum duration of server's LOGOFF state "
             "(waiting for client to close connection before server stops)");

#include "mplayer_controller.hpp"
#include "djap_utils/include/singleton.hpp"
namespace player_service {
class MplayerEventListener : public MplayerControllerDelegate {
public:
    MplayerEventListener() {

    }

    virtual ~MplayerEventListener() {

    }

    virtual void did_start(const std::string& filename)
    {
        LOG(DEBUG) << "MplayerControllerDelegate::did_start("
                   << filename
                   << ")";
    }
    virtual void did_end(const std::string& error)
    {

    }
    virtual void did_pause()
    {

    }
    virtual void did_resume()
    {

    }
    virtual void did_get_info_track_artist(const std::string& artist)
    {
        LOG(DEBUG) << "MplayerControllerDelegate::did_get_info_track_artist("
                   << artist
                   << ")";
    }
    virtual void did_get_info_track_name(const std::string& name)
    {
        LOG(DEBUG) << "MplayerControllerDelegate::did_get_info_track_name("
                   << name
                   << ")";
    }
    virtual void did_get_info_track_album(const std::string& album)
    {
        LOG(DEBUG) << "MplayerControllerDelegate::did_get_info_track_album("
                   << album
                   << ")";
    }
    virtual void did_get_info_track_progress(uint64_t progress_ms)
    {

    }
    virtual void did_get_info_track_volume(uint64_t volume_percent)
    {

    }
};

void test_mplayer_interface() {
    DjapUtils::SharedPointer<MPlayerController> controller_instance = DjapUtils::Singleton<MPlayerController>::get_instance();
    DjapUtils::SharedPointer<MplayerControllerDelegate> listener(new MplayerEventListener);
    controller_instance->set_delegate(listener);
    controller_instance->set_file(std::string("/home/djap/palvoja.mp3"));
    controller_instance->play();
    sleep(500);
    controller_instance->stop();
}
}

int main(int argc, char** argv) {
    player_service::test_mplayer_interface();
    return 0;
    /*
    // Parse gflags. We recommend you to use gflags as well.
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);

    // Generally you only need one Server.
    brpc::Server server;

    // Instance of your service.
    player_service::MplayerService player_service;

    // Add the service into server. Notice the second parameter, because the
    // service is put on stack, we don't want server to delete it, otherwise
    // use brpc::SERVER_OWNS_SERVICE.
    if (server.AddService(&player_service,
                          brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(ERROR) << "Fail to add service";
        return -1;
    }

    // Start the server.
    brpc::ServerOptions options;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    if (server.Start(FLAGS_port, &options) != 0) {
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }

    // Wait until Ctrl-C is pressed, then Stop() and Join() the server.
    server.RunUntilAskedToQuit();
    return 0;
    */
}
