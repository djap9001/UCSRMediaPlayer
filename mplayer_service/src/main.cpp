#include <iostream>
#include <unistd.h>
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
        sleep(1);
        cout << "Thread tester exitting" << endl;
        return;
    }
    virtual ~ThreadTester() {
        cout << "Thread tester deleted" << endl;
    }
    friend class DjapUtils::SharedPointer<ThreadTester>;
};

int main(int argc, char** argv) {
    // Test exception
    try {
        throw DjapUtils::Exception(std::string("Test exception!"));
    } catch (DjapUtils::Exception &e) {
        cout << "Caught test exception (THIS SHOULD HAPPEN): " << e.error_message() << endl;
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
        cout << "Caught an exception (SOMETHING IS WRONG HERE...): " << e.error_message() << endl;
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
        cout << "Shared pointer tester should be deleted in a moment..." << endl;
        printf("assign_to_here_raw: %p, back_from_weak: %p\n", assign_to_here.raw_ptr(), back_from_weak.raw_ptr());
    } while(0);
    DjapUtils::SharedPointer<SharedPointerTester> back_from_weak2(weak_test);
    printf("back_from_weak2: %p (should be nullptr now)\n", back_from_weak2.raw_ptr());

    DjapUtils::SharedPointer<ThreadTester> test_thread(DjapUtils::Thread::alloc<ThreadTester>());
    test_thread->Start();
    test_thread->Join();
    cout << "Ending.." << endl;
    return 0;
}
