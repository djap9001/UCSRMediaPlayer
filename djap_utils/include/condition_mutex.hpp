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

#include "djap_utils/include/mutex.hpp"

namespace DjapUtils {

class ConditionMutex : public Mutex{
public:
    ConditionMutex();
    ~ConditionMutex();
    void Wait();
    /*
     * Timed wait on condition,
     * Return true if condition received a signal within given timeout
     * Return false if wait timed out
     */
    bool timed_wait(uint32_t wait_max_milliseconds);
    void Signal();
    void Broadcast();

private:
#ifdef _USE_BAIDU_THREADS_
    bthread_cond_t _cond;
#else
    pthread_cond_t _cond;
#endif
    bool _is_cond_init;

  };

  } // namespace DjapUtils
