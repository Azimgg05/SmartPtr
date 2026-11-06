#ifndef UNQPTR_H
#define UNQPTR_H

#include <stdexcept>

template <typename T>
class UnqPtr {
private:
    T* ptr;

public:
    explicit UnqPtr(T* p = nullptr) {
        ptr = p;
    }

    ~UnqPtr() {
        delete ptr;
    }

    UnqPtr(const UnqPtr& other) = delete;
    UnqPtr& operator=(const UnqPtr& other) = delete;

    UnqPtr(UnqPtr&& other) noexcept {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    template <typename U>
    UnqPtr(UnqPtr<U>&& other) noexcept {
        ptr = static_cast<T*>(other.Release());
    }

    template <typename U>
    UnqPtr& operator=(UnqPtr<U>&& other) noexcept {
        delete ptr;
        ptr = static_cast<T*>(other.Release());
        return *this;
    }


    T& operator*() {
        if (ptr == nullptr) {
            throw std::runtime_error("Разыменование nullptr!");
        }
        return *ptr;
    }
    T* operator->() {
        if (ptr == nullptr) {
            throw std::runtime_error("Доступ через nullptr!");
        }
        return ptr;
    }

    T* Release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    T* get() const {
        return ptr;
    }

    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    bool operator==(const UnqPtr& other) const {return ptr == other.ptr;}
    bool operator!=(const UnqPtr& other) const {return ptr != other.ptr;}
    bool operator==(std::nullptr_t) const {return ptr == nullptr;}
    bool operator!=(std::nullptr_t) const {return ptr != nullptr;}
    explicit operator bool() const {return ptr != nullptr;}
};

#endif