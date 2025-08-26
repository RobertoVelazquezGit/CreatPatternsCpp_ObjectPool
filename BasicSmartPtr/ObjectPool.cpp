#include "ObjectPool.h"
#include "SharedObject.h"

#include <iostream>

SharedObject* ObjectPool::AcquireObject() {
    // Look for an unused object in the pool
    for (auto& so : m_PooledObjects) {
        if (!so->IsUsed()) {
            std::cout << "[POOL] Reusing an existing object\n";
            so->SetUsedState(true);
            so->Reset();
            return so.get();  // Return raw pointer
        }
    }

    // No reusable object found, create a new one
    std::cout << "[POOL] Creating a new object\n";
    auto newSO = std::make_unique<SharedObject>();
    newSO->SetUsedState(true);

    SharedObject* rawPtr = newSO.get();  // Save raw pointer for return
    m_PooledObjects.push_back(std::move(newSO));  // Transfer ownership to the pool
    return rawPtr;
}

void ObjectPool::ReleaseObject(SharedObject* pSO) {
    for (auto& so : m_PooledObjects) {
        // Match by pointer and ensure it's marked as used
        if (so.get() == pSO && so->IsUsed()) {
            so->SetUsedState(false);
            // Optionally: so->Reset();
        }
    }
}

void ObjectPool::Shutdown() {
    std::cout << "[POOL] Releasing all pooled objects...\n";
    m_PooledObjects.clear();  // All unique_ptrs are destroyed and memory is freed
    /**
    * unique_ptr gets destroyed when:
    *   comes out of the block
    *   container is destroyed clear(), erase(), etc ...
    */
}
