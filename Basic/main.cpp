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

	ObjectPool::Shutdown();
	return 0;
}

