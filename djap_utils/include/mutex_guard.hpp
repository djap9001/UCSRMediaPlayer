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
  * C++ mutex guard class with suppport for auto-unlocking mutex on return
  */
#ifndef _DJAP_UTILS_MUTEX_GUARD_HPP_
#define _DJAP_UTILS_MUTEX_GUARD_HPP_

#include "djap_utils/include/mutex.hpp"

namespace DjapUtils {

class MutexGuard {
public:
    MutexGuard(Mutex& guard_mutex);
    ~MutexGuard();
private:
    Mutex& _guard_mutex;
};

} // namespace DjapUtils

#endif // _DJAP_UTILS_MUTEX_GUARD_HPP_
