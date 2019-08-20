#include "djap_utils/include/shared_pointer_base.hpp"
#include "djap_utils/include/mutex_guard.hpp"

namespace DjapUtils {

ManagedPointerControlObject::ManagedPointerControlObject(void* managed_pointer) {
    _managed_pointer = managed_pointer;
    _strong_reference_count = 0;
    _weak_reference_count = 0;
}

ManagedPointerControlObject::~ManagedPointerControlObject() {

}

void ManagedPointerControlObject::add_ref_strong() {
    MutexGuard guard(_managed_pointer_lock);
    _strong_reference_count++;
}

void ManagedPointerControlObject::add_ref_weak() {
    MutexGuard guard(_managed_pointer_lock);
    _weak_reference_count++;
}

bool ManagedPointerControlObject::deref_strong(void** managed_pointer_out) {
    MutexGuard guard(_managed_pointer_lock);
    _strong_reference_count--;
    if (0 == _strong_reference_count) {
        *managed_pointer_out = _managed_pointer;    // set out pointer to let caller to know it should be deleted now.
        _managed_pointer = nullptr;
        if (0 == _weak_reference_count) {
            return true;    // delete the control object.
        }
    }
    return false;
}

bool ManagedPointerControlObject::deref_weak() {
    MutexGuard guard(_managed_pointer_lock);
    _weak_reference_count--;
    if ((0 == _weak_reference_count) && (0 == _strong_reference_count)) {
        return true;    // delete the control object.
    }
    return false;
}

void* ManagedPointerControlObject::managed_pointer() {
    // Not gaining the lock since only SharedPointers should call this.
    // If there are SharedPointer instances around to call this, then the _managed_pointer will not be altered
    return _managed_pointer;
}

}
