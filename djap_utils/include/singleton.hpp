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
  * Singleton instance
  */
#ifndef _DJAP_UTILS_SINGLETON_
#define _DJAP_UTILS_SINGLETON_

#include "shared_pointer.hpp"
#include "mutex.hpp"
#include "mutex_guard.hpp"

namespace DjapUtils {

template <class SingletonType>
class Singleton {
public:
    static SharedPointer<SingletonType> get_instance()
    {
        MutexGuard guard(_instance_lock);
        if (nullptr == _instance.raw_ptr()) {
            _instance = new SingletonType();
        }
        return _instance;
    }
private:
    static SharedPointer<SingletonType> _instance;
    static Mutex _instance_lock;
};
template <class SingletonType>
SharedPointer<SingletonType> Singleton<SingletonType>::_instance;
template <class SingletonType>
Mutex Singleton<SingletonType>::_instance_lock;

}

#endif // _DJAP_UTILS_SINGLETON_
