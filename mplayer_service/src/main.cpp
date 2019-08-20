#include <iostream>
#include "djap_utils/include/mutex.hpp"
#include "djap_utils/include/condition_mutex.hpp"
#include "djap_utils/include/shared_pointer.hpp"

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
    do {
        // Should delete once, TODO more tests later once implementation is complete
        DjapUtils::SharedPointer<SharedPointerTester> ptr1(new SharedPointerTester());
        DjapUtils::SharedPointer<SharedPointerTester> ptr2(ptr1);
    } while(0);

    return 0;
}
