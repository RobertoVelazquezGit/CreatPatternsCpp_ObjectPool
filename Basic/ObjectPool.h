#pragma once
#include <vector>
class SharedObject;  // Forward declaration

// Monostate
class ObjectPool
{
	// Private constructor, class can not be instantiated
	ObjectPool() = default;
	// Non copyable idiom
	ObjectPool(const ObjectPool&) = delete;
	ObjectPool& operator=(const ObjectPool&) = delete;

	inline static std::vector<SharedObject*> m_PooledObjects{};
public:

	static SharedObject* AcquireObject();
	static void ReleaseObject(SharedObject* pSO);
	static void Shutdown();  // Free memory
};

