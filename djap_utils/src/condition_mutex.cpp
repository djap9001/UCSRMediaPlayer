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
  * C++ condition class with suppport for pthread_cond or bthread_cond
  * as underlying condition implementation.
  * This class subclasses Mutex, which will be used or all wait calls since
  * for now this should be enough for all my use cases.
  */

#include "djap_utils/include/condition_mutex.hpp"

namespace DjapUtils {
ConditionMutex::ConditionMutex() : Mutex()
{
    _is_cond_init = (0 == pthread_cond_init(&_cond, nullptr));
}

ConditionMutex::~ConditionMutex() {
    if (true == _is_cond_init) {
        pthread_cond_destroy(&_cond);
    }
}

void ConditionMutex::Wait() {
    if (false == _is_cond_init) {
        throw Exception(std::string("Trying to wait un-initialized condition"));
    }
    int err = pthread_cond_wait(&_cond, underlying_mutex());
    if (0 != err) {
        // TODO need to be more detailed than this about the failure?
        throw Exception(std::string("pthread_cond_wait failed with error:") + std::to_string(err));
    }
}

void ConditionMutex::Signal() {
    if (false == _is_cond_init) {
        throw Exception(std::string("Trying to signal un-initialized condition"));
    }

    int err = pthread_cond_signal(&_cond);
    if (0 != err) {
        // TODO need to be more detailed than this about the failure?
        throw Exception(std::string("pthread_cond_signal failed with error:") + std::to_string(err));
    }
}

void ConditionMutex::Broadcast() {
    if (false == _is_cond_init) {
        throw Exception(std::string("Trying to broadcast un-initialized condition"));
    }
    int err = pthread_cond_broadcast(&_cond);
    if (0 != err) {
        // TODO need to be more detailed than this about the failure?
        throw Exception(std::string("pthread_cond_broadcast failed with error:") + std::to_string(err));
    }
}
}
