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
  * Base class and control object for custom shared and weak pointer implementations.
  */

#ifndef _DJAP_UTILS_SHARED_POINTER_BASE_
#define _DJAP_UTILS_SHARED_POINTER_BASE_

#include "djap_utils/include/mutex.hpp"

namespace DjapUtils {

class ManagedPointerControlObject {
public:
    ManagedPointerControlObject(void* managed_pointer);
    ~ManagedPointerControlObject();

    void add_ref_strong();
    void add_ref_weak();

    // if *managed_pointer_out != NULL on return, strong references reached 0, cast and delete *managed_pointer_out.
    // if return == true, both strong and weak references are at 0, delete the control object
    bool deref_strong(void** managed_pointer_out);
    bool deref_weak();
    void* managed_pointer();
private:
    uint32_t _strong_reference_count;
    uint32_t _weak_reference_count;
    void* _managed_pointer;
    Mutex _managed_pointer_lock;
};

class SharedPointerBase {
protected:
    virtual void deref() = 0;
    virtual void add_ref() = 0;
};

}

#endif
