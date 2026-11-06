#ifndef SHRDPTR_H
#define SHRDPTR_H

#include <stdexcept>
#include <cstddef>

template <typename T>
class ShrdPtr {
private:
    T* ptr;
    size_t* referenceCount;

    template <typename U> friend class ShrdPtr;

    void Release() {
        if (referenceCount != nullptr) {
            (*referenceCount)--;
            if (*referenceCount == 0) {
                delete ptr;
                delete referenceCount;
            }
        }
        ptr = nullptr;
        referenceCount = nullptr;
    }

public:
    explicit ShrdPtr(T* p = nullptr) {
        ptr = p;
        if (ptr != nullptr) {
            referenceCount = new size_t(1);
        } else {
            referenceCount = nullptr;
        }
    }

    ~ShrdPtr() {
        Release();
    }

    ShrdPtr(const ShrdPtr& other) {
        ptr = other.ptr;
        referenceCount = other.referenceCount;
        if (referenceCount != nullptr) {
            (*referenceCount)++;
        }
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if (ptr != other.ptr) {
            Release();
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            if (referenceCount != nullptr) {
                (*referenceCount)++;
            }
        }
        return *this;
    }

    ShrdPtr(ShrdPtr&& other) noexcept {
        ptr = other.ptr;
        referenceCount = other.referenceCount;
        other.ptr = nullptr;
        other.referenceCount = nullptr;
    }

    ShrdPtr& operator=(ShrdPtr&& other) noexcept {
        if (this != &other) {
            Release();
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            other.ptr = nullptr;
            other.referenceCount = nullptr;
        }
        return *this;
    }

    template <typename U>
    ShrdPtr(const ShrdPtr<U>& other) {
        ptr = static_cast<T*>(other.ptr);
        referenceCount = other.referenceCount;
        if (referenceCount != nullptr) {
            (*referenceCount)++;
        }
    }

    template <typename U>
    ShrdPtr(ShrdPtr<U>&& other) noexcept {
        ptr = static_cast<T*>(other.ptr);
        referenceCount = other.referenceCount;
        other.ptr = nullptr;
        other.referenceCount = nullptr;
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

    size_t GetCount() const {
        return (referenceCount != nullptr) ? *referenceCount : 0;
    }

    T* get() const {
        return ptr;
    }

    void reset(T* p = nullptr) {
        Release();
        ptr = p;
        if (ptr != nullptr) {
            referenceCount = new size_t(1);
        } else {
            referenceCount = nullptr;
        }
    }

    bool operator==(const ShrdPtr& other) const { return ptr == other.ptr; }
    bool operator!=(const ShrdPtr& other) const { return ptr != other.ptr; }
    bool operator==(std::nullptr_t) const { return ptr == nullptr; }
    bool operator!=(std::nullptr_t) const { return ptr != nullptr; }
    explicit operator bool() const { return ptr != nullptr; }

    template <typename U>
    ShrdPtr& operator=(const ShrdPtr<U>& other) {
        if (ptr != other.ptr) {
            Release();
            ptr = static_cast<T*>(other.ptr);
            referenceCount = other.referenceCount;
            if (referenceCount != nullptr) {
                (*referenceCount)++;
            }
        }
        return *this;
    }
};

#endif