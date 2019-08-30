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

    /*
     * Construct function, use this to allocate new instances of threads
     * Example:
     * User has created a class called MyWorkerThread that is a subclass of Thread.
     * To allocate and run an instance of MyWorkerThread:
     * void run_my_worker(bool wait_for_it) {
     *      SharedPointer<MyWorkerThread> t(Thread::alloc<MyWorkerThread>());
     *      // TODO any additional initialization here (if you need to pass some parameters for MyWorkerThread, implement your own interface for it and call it here)
     *      t->start();  // starts the execution of MyWorkerThread, you can detach and discard t at this point if you're not interested in Joinining the thread,
     *                   // Thread can delete it self when it stops running if there are no references to it elsewhere.
     *      if (true == wait_for_it) {
     *          t->join();
     *      } else {
     *          t->detach();
     *      }
     * }

     */
    template <typename ThreadSubclass>
    static SharedPointer<ThreadSubclass> alloc() {
        SharedPointer<ThreadSubclass> ret(new ThreadSubclass());
        // TODO new WeakPointer with ret as parameter, assign to _weak_ref
        ret->_weak_ref = new WeakPointer<ThreadSubclass>(ret.to_weak());
        ret->_thread_retainer = new SharedPointer<ThreadSubclass>();
        return ret;
    }
#ifdef _USE_BAIDU_THREADS_
    void start_urgent();
#endif // _USE_BAIDU_THREADS_
    void start();                   // Launch the thread
    void join();                    // Wait for the thread to finish
    void detach();                  // Detach the thread, not going to join() it.
    static std::string thread_id();  // Unique ID for the calling thread

protected:
    virtual void thread_main() = 0; // Override to implement the thread's main function here
    Thread();                       // Due to thread object management the constructor and destructor are protected. Call alloc() to allocate an instance of thread
    virtual ~Thread();              // Due to thread object management the constructor and destructor are protected, only shared pointer is allowed to delete the object.
private:
    static void* thread_starter(void* instance_pointer); // moves the execution to thread_main();
#ifdef _USE_BAIDU_THREADS_
    bthread_t _thread_handle;
#else
    pthread_t _thread_handle;
#endif // _USE_BAIDU_THREADS_
    SharedPointerBase* _thread_retainer;    // keeps the object alive as long as thread is running.
    SharedPointerBase* _weak_ref;           // allow Start() to create a strong reference, user don't need to pass it in.
    bool _started;
};

} // namespace DjapUtils

#endif // _DJAP_UTILS_THREAD_HPP_
