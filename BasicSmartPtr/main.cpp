#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "ObjectPool.h"
#include "SharedObject.h"
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto s1 = ObjectPool::AcquireObject();
	s1->MethodA();
	s1->MethodB();

	auto s2 = ObjectPool::AcquireObject();
	s2->MethodA();
	s2->MethodB();

	ObjectPool::ReleaseObject(s1);
	auto s3 = ObjectPool::AcquireObject();
	s3->MethodA();
	s3->MethodB();

	/**
	* Even not calling shutdown there is not memory leak
	* static std::vector<std::unique_ptr<SharedObject>> m_PooledObjects{}
	* is created when used for the first time,
	* is destroyed after main and before process ends.
	* 
	*/
	ObjectPool::Shutdown();
	return 0;
}

