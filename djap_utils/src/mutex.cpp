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
#include "djap_utils/include/mutex.hpp"
namespace DjapUtils {

Mutex::Mutex() {
    _is_init = false;
#ifdef _USE_BAIDU_THREADS_
    if (0 == bthread_mutex_init(&_mutex, nullptr)) {
        _is_init = true;
    }
#else
    if (0 == pthread_mutex_init(&_mutex, nullptr)) {
        _is_init = true;
    }
#endif
}

Mutex::~Mutex() {
    if (true == _is_init) {
#ifdef _USE_BAIDU_THREADS_
        bthread_mutex_destroy(&_mutex);
#else
        pthread_mutex_destroy(&_mutex);
#endif
    }
}

void Mutex::lock() {
    if (false == _is_init) {
        throw Exception(std::string("Trying to lock uninitialized mutex!"));
    }
#ifdef _USE_BAIDU_THREADS_
    if (0 != bthread_mutex_lock(&_mutex)) {
        throw Exception(std::string("Locking mutex failed!"));
    }
#else
    if (0 != pthread_mutex_lock(&_mutex)) {
        throw Exception(std::string("Locking mutex failed!"));
    }
#endif
}

void Mutex::unlock() {
    if (false == _is_init) {
        throw Exception(std::string("Trying to unlock uninitialized mutex!"));
    }
#ifdef _USE_BAIDU_THREADS_
    if (0 != bthread_mutex_unlock(&_mutex)) {
        throw Exception(std::string("Unlocking mutex failed!"));
    }
#else
    if (0 != pthread_mutex_unlock(&_mutex)) {
        throw Exception(std::string("Unlocking mutex failed!"));
    }
#endif
}
#ifdef _USE_BAIDU_THREADS_
bthread_mutex_t* Mutex::underlying_mutex() {
#else
pthread_mutex_t* Mutex::underlying_mutex() {
#endif
    return &_mutex;
}

} // namespace DjapUtils
