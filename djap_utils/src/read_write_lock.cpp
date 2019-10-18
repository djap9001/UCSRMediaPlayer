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

#include "djap_utils/include/read_write_lock.hpp"
#include "djap_utils/include/mutex_guard.hpp"

namespace DjapUtils {

ReadWriteLock::ReadWriteLock() {
    _is_init = false;
#ifdef _USE_BAIDU_THREADS_
    _is_init = true;
#else
    if (0 == pthread_rwlock_init(&_lock, nullptr)) {
        _is_init = true;
    }
#endif
}

ReadWriteLock::~ReadWriteLock() {
#ifdef _USE_BAIDU_THREADS_

#else
    if (_is_init) {
        pthread_rwlock_destroy(&_lock);
    }
#endif
}

void ReadWriteLock::write_lock() {
#ifdef _USE_BAIDU_THREADS_
    SharedPointer<bool> permit_to_proceed(new bool);
    *permit_to_proceed.raw_ptr() = false;
    _lock_state_mutex.lock();
    _locks.push_back(std::make_pair(-1, permit_to_proceed));
    if (_locks.size() == 1) {
        // Only one trying to obtain, get it right away.
        *permit_to_proceed.raw_ptr() = true;
    }

    // Wait untill we can gain write lock
    while (false == *permit_to_proceed.raw_ptr()) {
        _lock_state_mutex.Wait();
    }
    _lock_state_mutex.unlock();
#else
    if (0 != pthread_rwlock_wrlock(&_lock)) {
        throw Exception(std::string("Gaining write lock failed!"));
    }
#endif
}

void ReadWriteLock::write_unlock() {
#ifdef _USE_BAIDU_THREADS_
    {
        MutexGuard lock(_lock_state_mutex);
        if (_locks.size() == 0 || _locks.begin()->first > 0) {
            // either this is not locked or not write locked, error
            throw Exception(std::string("Releasing write lock failed!"));
        }
        _locks.erase(_locks.begin());
        if (_locks.size() > 0) {
            // Give next in queue a permit to proceed
            *_locks.begin()->second.raw_ptr() = true;
        }
    }
    _lock_state_mutex.Broadcast();  // notify all of possible changes in permit situation
#else
    if (0 != pthread_rwlock_unlock(&_lock)) {
        throw Exception(std::string("Releasing write lock failed!"));
    }
#endif
}

void ReadWriteLock::read_lock() {
#ifdef _USE_BAIDU_THREADS_
    SharedPointer<bool> permit_to_proceed;
    _lock_state_mutex.lock();
    if (_locks.size() > 0) {
        if (_locks.back().first > 0) {
            // Join the most recent read batch
            _locks.back().first++;  // Increase lock holder count in this batch
            permit_to_proceed = _locks.back().second;
        } else {
            // a pending or active write lock, create a new batch and wait
            permit_to_proceed = SharedPointer<bool>(new bool);
            *permit_to_proceed.raw_ptr() = false;
            _locks.push_back(std::make_pair(1, permit_to_proceed));
        }
    } else {
        // Only one trying to obtain, create new batch and get permit right away.
        permit_to_proceed = SharedPointer<bool>(new bool);
        *permit_to_proceed.raw_ptr() = true;
        _locks.push_back(std::make_pair(1, permit_to_proceed));
    }
    // Wait untill we can gain write lock
    while (false == *permit_to_proceed.raw_ptr()) {
        _lock_state_mutex.Wait();
    }
    _lock_state_mutex.unlock();
#else
    if (0 != pthread_rwlock_rdlock(&_lock)) {
        throw Exception(std::string("Gaining read lock failed!"));
    }
#endif
}

void ReadWriteLock::read_unlock() {
#ifdef _USE_BAIDU_THREADS_
    {
        MutexGuard lock(_lock_state_mutex);
        if (_locks.size() == 0 || _locks.begin()->first < 0) {
            // either this is not locked or not read locked, error
            throw Exception(std::string("Releasing read lock failed!"));
        }
        _locks.begin()->first--;    // Reduce lock holder count in this batch
        if (_locks.begin()->first == 0) {
            // Was the last one in current batch, erase the batch
            _locks.erase(_locks.begin());
            if (_locks.size() > 0) {
                // Give next in queue a permit to proceed
                *_locks.begin()->second.raw_ptr() = true;
            }
        }
    }
    _lock_state_mutex.Broadcast();  // notify all of possible changes in permit situation
#else
    if (0 != pthread_rwlock_unlock(&_lock)) {
        throw Exception(std::string("Releasing read lock failed!"));
    }
#endif
}

} // namespace DjapUtils
