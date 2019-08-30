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
  * C++ mutex class with suppport for pthread_mutex or bthread_mutex
  * as underlying mutex implementation
  */
#ifndef _DJAP_UTILS_MUTEX_HPP_
#define _DJAP_UTILS_MUTEX_HPP_
// TODO add bthread support and required macros
// TODO add read/write lock support
#ifdef _USE_BAIDU_THREADS_
#include "bthread/bthread.h"
#else
#include <pthread.h>
#endif
#include "djap_utils/include/Exception.hpp"

namespace DjapUtils {

class Mutex {
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
protected:
#ifdef _USE_BAIDU_THREADS_
    bthread_mutex_t* underlying_mutex();
#else
    pthread_mutex_t* underlying_mutex();
#endif
private:
#ifdef _USE_BAIDU_THREADS_
    bthread_mutex_t _mutex;
#else
    pthread_mutex_t _mutex;
#endif
    bool _is_init;
};

} // namespace DjapUtils

#endif // _DJAP_UTILS_MUTEX_HPP_
