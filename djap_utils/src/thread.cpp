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

#include "djap_utils/include/thread.hpp"
namespace DjapUtils {
#ifdef _USE_BAIDU_THREADS_
void Thread::start_urgent() {
    if (nullptr == _weak_ref) {
        throw Exception(std::string("This thread is not properly allocated, always use alloc() template function to create new instances of thread classes!"));
    }
    if (true == _started) {
        throw Exception(std::string("Thread can be started only once, create a new one."));
    }
    _thread_retainer->copy_from(_weak_ref); // create a strong reference
    _started = true;
    if (0 != bthread_start_urgent(&_thread_handle, nullptr, &Thread::thread_starter, this)) {
        delete _thread_retainer;
        _thread_retainer = nullptr;
        throw Exception(std::string("Failed to create the thread!"));
    }
}
#endif // _USE_BAIDU_THREADS_

void Thread::start() {
    if (nullptr == _weak_ref) {
        throw Exception(std::string("This thread is not properly allocated, always use alloc() template function to create new instances of thread classes!"));
    }
    if (true == _started) {
        throw Exception(std::string("Thread can be started only once, create a new one."));
    }
    _thread_retainer->copy_from(_weak_ref); // create a strong reference
    _started = true;
#ifdef _USE_BAIDU_THREADS_
    if (0 != bthread_start_background(&_thread_handle, nullptr, &Thread::thread_starter, this)) {
#else
    if (0 != pthread_create(&_thread_handle, nullptr, &Thread::thread_starter, this)) {
#endif // _USE_BAIDU_THREADS_
        delete _thread_retainer;
        _thread_retainer = nullptr;
        throw Exception(std::string("Failed to create the thread!"));
    }
}

void Thread::join() {
#ifdef _USE_BAIDU_THREADS_
    bthread_join(_thread_handle, nullptr);
#else
    pthread_join(_thread_handle, nullptr);
#endif // _USE_BAIDU_THREADS_
}

void Thread::detach() {
#ifdef _USE_BAIDU_THREADS_
    // all bthreads all detached (but still joinable) by default, there's no detach() method for them
#else
    pthread_detach(_thread_handle);
#endif // _USE_BAIDU_THREADS_
}

std::string Thread::thread_id() {
    // TODO ID of the calling thread
    return std::string("");
}

// Due to thread object management the constructor and destructor are protected. Call alloc() to allocate an instance of thread
Thread::Thread() {
    _weak_ref = nullptr;
    _thread_retainer = nullptr;
    _started = false;
}

Thread::~Thread() {
    if (nullptr != _weak_ref) {
        delete _weak_ref;
        // _thread_retainer is managed in Start() and ThreadStarter()
    }
}

void* Thread::thread_starter(void* instance_pointer) {
    // handles management of the thread object's life time and calls the thread's main function that is implemented by subclass
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

}   // namespace DjapUtils
