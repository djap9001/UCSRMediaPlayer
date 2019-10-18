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
  * C++ read_write lock class
  */
#ifndef _DJAP_UTILS_READ_WRITE_LOCK_HPP_
#define _DJAP_UTILS_READ_WRITE_LOCK_HPP_

#ifdef _USE_BAIDU_THREADS_
#include "djap_utils/include/mutex.hpp"
#include "djap_utils/include/condition_mutex.hpp"
#include "djap_utils/include/shared_pointer.hpp"
#else
#include <pthread.h>
#endif
#include "djap_utils/include/Exception.hpp"

namespace DjapUtils {

class ReadWriteLock {
public:
    ReadWriteLock();
    ~ReadWriteLock();
    void write_lock();
    void write_unlock();
    void read_lock();
    void read_unlock();
protected:

private:
#ifdef _USE_BAIDU_THREADS_
    ConditionMutex _lock_state_mutex;    // bthread does not have working read_write_lock implementation last time I checked.
                                // Somewhat ugly and probably not too optimal but working workaround
    std::list<std::pair<int, SharedPointer<bool> > > _locks;
#else
    pthread_rwlock_t _lock;
#endif
    bool _is_init;
};

} // namespace DjapUtils

#endif // _DJAP_UTILS_READ_WRITE_LOCK_HPP_
