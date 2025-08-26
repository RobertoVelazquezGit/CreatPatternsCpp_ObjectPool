#pragma once

#include <vector>
#include <memory>  // For std::unique_ptr

class SharedObject;

class ObjectPool {
public:
    // Retrieves an object from the pool
    static SharedObject* AcquireObject();

    // Returns an object back to the pool
    static void ReleaseObject(SharedObject* pSO);

    // Frees all memory and clears the pool
    static void Shutdown();

    ObjectPool() = delete;
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

private:
    // Vector holding all pooled objects
    inline static std::vector<std::unique_ptr<SharedObject>> m_PooledObjects{};
};

