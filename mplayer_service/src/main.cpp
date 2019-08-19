#include <iostream>
#include "djap_utils/include/mutex.hpp"
#include "djap_utils/include/condition_mutex.hpp"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    try {
        throw DjapUtils::Exception(std::string("Test exception!"));
    } catch (DjapUtils::Exception &e) {
        cout << "Caught test exception (THIS SHOULD HAPPEN): " << e.error_message() << endl;
    }
    try {
        DjapUtils::Mutex test_mutex;
        DjapUtils::ConditionMutex test_condition;   // TODO test for wait once thread has been implemented.
        test_mutex.lock();
        test_condition.lock();
        cout << "Hello world!" << endl;
        test_condition.unlock();
        test_mutex.unlock();
        test_condition.Signal();    // shouldn't have any issues
        test_condition.Broadcast(); // shouldn't have any issues
    } catch (DjapUtils::Exception &e) {
        cout << "Caught an exception (SOMETHING IS WRONG HERE...): " << e.error_message() << endl;
    }
    return 0;
}
