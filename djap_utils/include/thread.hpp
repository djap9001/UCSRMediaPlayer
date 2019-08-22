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
        ret->_weak_ref = new WeakPointer<ThreadSubclass>(ret.to_weak());
        ret->_thread_retainer = new SharedPointer<ThreadSubclass>();
        return ret;
    }

    void Start() {
        if (nullptr == _weak_ref) {
            throw Exception(std::string("This thread is not properly allocated, always use alloc() template function to create new instances of thread classes!"));
        }
        if (true == _started) {
            throw Exception(std::string("Thread can be started only once, create a new one."));
        }
        _thread_retainer->copy_from(_weak_ref); // create a strong reference
        _started = true;
        if (0 != pthread_create(&_thread_handle, nullptr, &Thread::ThreadStarter, this)) {
            delete _thread_retainer;
            _thread_retainer = nullptr;
            throw Exception(std::string("Failed to create the thread!"));
        }
    }

    void Join() {
        pthread_join(_thread_handle, nullptr);
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
        _started = false;
    }
    virtual ~Thread()   // Due to thread object management the constructor and destructor are protected, only shared pointer is allowed to delete the object.
    {
        if (nullptr != _weak_ref) {
            delete _weak_ref;
            // _thread_retainer is managed in Start() and ThreadStarter()
        }
    }
private:
    static void* ThreadStarter(void* instance_pointer) {
        Thread* thread_ptr = static_cast<Thread*>(instance_pointer);
        try {
            thread_ptr->thread_main();
        } catch (std::exception& e) {
            printf("Caught an exception from thread\n");
        }
        SharedPointerBase* destruct_ptr = thread_ptr->_thread_retainer;
        thread_ptr->_thread_retainer = nullptr;
        delete destruct_ptr;
        return nullptr;
    }
    pthread_t _thread_handle;
    SharedPointerBase* _thread_retainer;    // keeps the object alive as long as thread is running.
    SharedPointerBase* _weak_ref;           // allow Start() to create a strong reference, user don't need to pass it in.
    bool _started;
};

} // namespace DjapUtils

#endif // _DJAP_UTILS_THREAD_HPP_
