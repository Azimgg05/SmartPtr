#ifndef UNQ_ARR_H
#define UNQ_ARR_H

#include <cstddef>

template <typename T>
class UnqArr {
private:
    T* ptr;

public:
    explicit UnqArr(T* p = nullptr) {
    ptr = p;
    }

    ~UnqArr() {
        delete[] ptr;
    }

    UnqArr(const UnqArr&) = delete;
    UnqArr& operator=(const UnqArr&) = delete;

    UnqArr(UnqArr&& other) noexcept {
        ptr =other.ptr;
        other.ptr = nullptr;
    }



    UnqArr& operator=(UnqArr&& other) noexcept {
        if (this != &other) {
            delete[] ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator[](std::size_t index) const {
        return ptr[index];
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    T* get() const {
        return ptr;
    }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        if (ptr != p) {
            delete[] ptr;
            ptr = p;
        }
    }
};

#endif