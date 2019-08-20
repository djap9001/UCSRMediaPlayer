/*
 * Copyright 2019 Väinö Lappi
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * C++ threading class with suppport for pthread or bthread
  * as underlying thread implementation
  */
#ifndef _DJAP_UTILS_MUTEX_HPP_
#define _DJAP_UTILS_MUTEX_HPP_
// TODO add bthread support and required macros

#include <pthread.h>
#include "djap_utils/include/Exception.hpp"

namespace DjapUtils {

class Thread {
public:
    virtual static
    void Start();
    void Join();
    static std::string ThreadID(); // ID of the calling thread
protected:
    virtual void thread_main() {
        return;
    }
    Thread();   // Due to thread object management the constructor and destructor are protected, implementing instance should introduce a static construct
                // Function that returns a shared pointer to the thread object.
    ~Thread();  // Due to thread object management the constructor and destructor are protected, only shared pointer is allowed to delete the object.
private:
    static void* ThreadStarter(SharedPointerBase* instance_pointer)
    pthread_t _thread_handle;
    SharedPointerBase* _thread_retainer; // keeps the object alive as long as thread is running.
};

} // namespace DjapUtils

#endif // _DJAP_UTILS_MUTEX_HPP_
