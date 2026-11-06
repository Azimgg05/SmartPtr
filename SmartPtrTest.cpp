#include "SmartPtrTest.h"
#include <cassert>
#include <utility>
#include <iostream>
#include <stdexcept>

#include "UnqPtr.h"
#include "ShrdPtr.h"
#include "UnqPtrArr.h"
#include "ShrdPtrArr.h"
#include "ArraySequence.h"

class TestObject {
private:
    int value{0};
public:
    static int destroyed;
    explicit TestObject(int newValue = 0) : value(newValue) {}
    ~TestObject() { ++destroyed; }
    int GetValue() const { return value; }
    void SetValue(int newValue) { value = newValue; }
};

int TestObject::destroyed = 0;

void TestUnqPtr() {
    TestObject::destroyed = 0;
    {
        UnqPtr<TestObject> empty;
        assert(empty.get() == nullptr);
        assert(!empty);

        UnqPtr<TestObject> first(new TestObject(10));
        assert(first.get() != nullptr);
        assert(first);
        assert(first->GetValue() == 10);
        first->SetValue(11);
        assert(first->GetValue() == 11);

        UnqPtr<TestObject> moved(std::move(first));
        assert(first.get() == nullptr);
        assert(moved->GetValue() == 11);

        moved.reset(new TestObject(30));
        assert(moved->GetValue() == 30);
        assert(TestObject::destroyed == 1);

        moved.reset(nullptr);
        assert(moved.get() == nullptr);
        assert(TestObject::destroyed == 2);
    }
    assert(TestObject::destroyed == 2);
}

void TestShrdPtr() {
    TestObject::destroyed = 0;
    {
        ShrdPtr<TestObject> empty;
        assert(empty.get() == nullptr);

        ShrdPtr<TestObject> first(new TestObject(40));
        {
            ShrdPtr<TestObject> copy(first);
            ShrdPtr<TestObject> moved(std::move(copy));
            assert(copy.get() == nullptr);

            moved.reset();
        }
        assert(TestObject::destroyed == 0);
    }
    assert(TestObject::destroyed == 1);
}

void TestUnqArr() {
    TestObject::destroyed = 0;
    {
        UnqArr<TestObject> arr(new TestObject[3]);
        arr[0].SetValue(1);
        arr[1].SetValue(2);
        arr[2].SetValue(3);

        assert(arr[1].GetValue() == 2);

        UnqArr<TestObject> moved(std::move(arr));
        assert(arr.get() == nullptr);
        assert(moved[2].GetValue() == 3);

        moved.reset(nullptr);
        assert(TestObject::destroyed == 3);
    }
    assert(TestObject::destroyed == 3);
}

void TestShrdArr() {
    TestObject::destroyed = 0;
    {
        ShrdArr<TestObject> arr1(new TestObject[2]);
        assert(arr1.use_count() == 1);

        {
            ShrdArr<TestObject> arr2 = arr1;
            assert(arr1.use_count() == 2);
            arr2[0].SetValue(42);
            assert(arr1[0].GetValue() == 42);
        }
        assert(arr1.use_count() == 1);
        assert(TestObject::destroyed == 0);
    }
    assert(TestObject::destroyed == 2);
}

void TestSequence() {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 10);
    assert(seq.Get(2) == 30);

    bool caught = false;
    try {
        seq.Get(999);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
}