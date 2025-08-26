
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "ObjectPool.h"

// Class with private constructor
class Test {
    Test() {
        std::cout << "Test constructor\n";
    }
public:
    void Foo(int threadId) {
        std::cout << "Test::Foo() from thread " << threadId << "\n";
    }
    friend class TestAllocator;
};

// Custom allocator for Test
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

void ThreadTask(int id) {
    using TestPool = ObjectPool<Test, 3, TestAllocator>;  // All threads use the same static pool

    Test* obj = TestPool::Acquire();  // Acquire a shared object from the pool
    if (obj) {
        obj->Foo(id);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate some work
        std::cout << "Thread " << id << " releasing Test object\n";
        TestPool::Release(obj);  // Return the object to the pool
    }
    else {
        std::cout << "Thread " << id << " could not acquire Test object (pool full)\n";
    }
}

int main() {
    std::cout << "=== Multithreaded ObjectPool<Test, 3, TestAllocator> ===\n";

    using TestPool = ObjectPool<Test, 3, TestAllocator>;  // Alias for readability

    std::vector<std::thread> threads;

    for (int i = 0; i < 6; ++i) {
        threads.emplace_back(ThreadTask, i);  // Launch more threads than pool capacity
    }

    for (auto& t : threads) {
        t.join();  // Wait for all threads to finish
    }

    // The object pool is shared between all threads via static inline members.
    // That's why we only need to call Destroy() once — after all threads are done.
    TestPool::Destroy();  // Clean up the shared pool (safe now)

    // ObjectPool<Test, 3, TestAllocator>::Destroy();  // Same as above, shown for learning

    return 0;
}
