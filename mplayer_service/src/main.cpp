#include <stdio.h>
#include "djap_utils/include/mutex.hpp"

int main(int argc, char** argv) {
    DjapUtils::Mutex test_mutex;
    test_mutex.lock();
    printf("Hello wold!\n");
    test_mutex.unlock();
    return 0;
}
