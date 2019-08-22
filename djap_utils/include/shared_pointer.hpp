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
  * Shared pointer using DjapUtils:Mutex for control object locking
  */
#ifndef _DJAP_UTILS_SHARED_POINTER_
#define _DJAP_UTILS_SHARED_POINTER_

#include "djap_utils/include/shared_pointer_base.hpp"
#include "djap_utils/include/weak_pointer.hpp"

namespace DjapUtils {

template <class PointerType>
class SharedPointer : public SharedPointerBase {
public:
    SharedPointer() {
        _control_object = new ManagedPointerControlObject(nullptr);
        _control_object->add_ref_strong();
    }

    SharedPointer(PointerType* raw_ptr) {
        _control_object = new ManagedPointerControlObject(raw_ptr);
        _control_object->add_ref_strong();
    }

    SharedPointer(const SharedPointer<PointerType>& other) {
        _control_object = other._control_object;
        _control_object->add_ref_strong();
    }

    SharedPointer<PointerType>& operator= (const SharedPointer<PointerType>& other) {
        copy_from(&other);
        return *this;
    }

    SharedPointer(const WeakPointer<PointerType>& other) {
        _control_object = other._control_object;
        _control_object->add_ref_strong();
    }

    SharedPointer<PointerType>& operator= (const WeakPointer<PointerType>& other) {
        copy_from(&other);
        return *this;
    }

    PointerType* operator-> () {
        return static_cast<PointerType*>(_control_object->managed_pointer());
    }

    ~SharedPointer() {
        deref();
    }

    PointerType* raw_ptr() {
        return static_cast<PointerType*>(_control_object->managed_pointer());
    }

    WeakPointer<PointerType> to_weak() {
        WeakPointer<PointerType> weak;
        weak.copy_from(this);
        return weak;
    }

protected:
    virtual void deref() {
        void* managed_pointer_out = nullptr;
        bool delete_control = _control_object->deref_strong(&managed_pointer_out);
        if (true == delete_control) {
            delete _control_object;
        }
        if (nullptr != managed_pointer_out) {
            delete static_cast<PointerType*>(managed_pointer_out);
        }
    }

    virtual void add_ref() {
        _control_object->add_ref_strong();
    }

    virtual void set_control_object(ManagedPointerControlObject* control) {
        if (nullptr == control) {
            control = new ManagedPointerControlObject(nullptr);
        }
        control->add_ref_strong();
        deref();
        _control_object = control;
    }
};

}

#endif // _DJAP_UTILS_SHARED_POINTER_
