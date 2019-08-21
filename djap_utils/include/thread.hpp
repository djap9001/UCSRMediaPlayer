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
#ifndef _DJAP_UTILS_THREAD_HPP_
#define _DJAP_UTILS_THREAD_HPP_
// TODO add bthread support and required macros

#include <pthread.h>
#include "djap_utils/include/Exception.hpp"
#include "djap_utils/include/shared_pointer.hpp"

namespace DjapUtils {

class Thread {
public:
    template <typename ThreadSubclass>
    static SharedPointer<ThreadSubclass> alloc() {
        SharedPointer<ThreadSubclass> ret(new ThreadSubclass());
        // TODO new WeakPointer with ret as parameter, assign to _weak_ref

        return ret;
    }

    void Start() {
    }
    void Join() {
    }
    static std::string ThreadID()
    {
        // TODO ID of the calling thread
        return std::string("");
    }
protected:
    virtual void thread_main() = 0;
    Thread()    // Due to thread object management the constructor and destructor are protected. Call alloc() to allocate an instance of thread
    {
        _weak_ref = nullptr;
        _thread_retainer = nullptr;
    }
    virtual ~Thread()   // Due to thread object management the constructor and destructor are protected, only shared pointer is allowed to delete the object.
    {
        if (nullptr != _weak_ref) {
            delete _weak_ref;
            // _thread_retainer is managed in Start() and ThreadStarter()
        }
    }
private:
    static void* ThreadStarter(SharedPointerBase* instance_pointer) {
        return nullptr;
    }
    pthread_t _thread_handle;
    SharedPointerBase* _thread_retainer;    // keeps the object alive as long as thread is running.
    SharedPointerBase* _weak_ref;           // allow Start() to create a strong reference, user don't need to pass it in.
};

} // namespace DjapUtils

#endif // _DJAP_UTILS_THREAD_HPP_
