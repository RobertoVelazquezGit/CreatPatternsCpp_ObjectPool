
#include <iostream>
#include "ObjectPool.h"

class Test {
    Test() {
        std::cout << "Test constructor\n";
    }

public:
    void Foo() {
        std::cout << "Test::Foo()\n";
    }

    friend class TestAllocator;  // Because of Test has a private constructor
};

class TestAllocator {
public:
    Test* operator()() {
        std::cout << "TestAllocator::Create()\n";
        return new Test{};
    }

    void operator()(Test* p) {
        std::cout << "TestAllocator::Destroy()\n";
        delete p;
    }

    void Reset() {
        std::cout << "TestAllocator::Reset()\n";
    }
};

// ------------------------ Main ------------------------
int main() {
    std::cout << "=== Using ObjectPool<int> ===\n";
    using IntPool = ObjectPool<int, 3>; // Default allocator <-- it is here where the private static vector is created

    auto p1 = IntPool::Acquire(); // new
    auto p2 = IntPool::Acquire(); // new
    auto p3 = IntPool::Acquire(); // new

    auto p4 = IntPool::Acquire(); // pool full -> nullptr
    IntPool::Release(p1);         // free p1
    auto p5 = IntPool::Acquire(); // reuse p1
    IntPool::Destroy();           // free all

    std::cout << "\n=== Using ObjectPool<Test, 2, TestAllocator> ===\n";
    using TestPool = ObjectPool<Test, 2, TestAllocator>;

    Test* t1 = TestPool::Acquire(); // create new Test
    Test* t2 = TestPool::Acquire(); // create new Test
    Test* t3 = TestPool::Acquire(); // pool full -> nullptr

    if (t1) t1->Foo();
    TestPool::Release(t1);
    Test* t4 = TestPool::Acquire(); // reuse t1
    TestPool::Destroy();           // free all
}

// m_PooledObjects for int and Test live in separate pools.
// Each type has its own isolated pool (correct for this design).
// Int and Test objects do not share the static array.