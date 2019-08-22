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
  * Weak pointer using DjapUtils:Mutex for control object locking
  */
#ifndef _DJAP_UTILS_WEAK_POINTER_
#define _DJAP_UTILS_WEAK_POINTER_

#include "djap_utils/include/shared_pointer_base.hpp"

namespace DjapUtils {
template <class PointerType> class SharedPointer;

template <class PointerType>
class WeakPointer : public SharedPointerBase {
public:
    WeakPointer() {
        _control_object = new ManagedPointerControlObject(nullptr);
        _control_object->add_ref_weak();
    }

    WeakPointer(const WeakPointer<PointerType>& other) {
        _control_object = other._control_object;
        _control_object->add_ref_weak();
    }

    WeakPointer<PointerType>& operator= (const WeakPointer<PointerType>& other) {
        copy_from(&other);
        return *this;
    }

    ~WeakPointer() {
        deref();
    }

protected:
    virtual void deref() {
        bool delete_control = _control_object->deref_weak();
        if (true == delete_control) {
            delete _control_object;
        }
    }

    virtual void add_ref() {
        _control_object->add_ref_weak();
    }

    virtual void set_control_object(ManagedPointerControlObject* control) {
        if (nullptr == control) {
            control = new ManagedPointerControlObject(nullptr);
        }
        control->add_ref_weak();
        deref();
        _control_object = control;
    }
    friend class SharedPointer<PointerType>;
};

}
#endif // _DJAP_UTILS_WEAK_POINTER_
