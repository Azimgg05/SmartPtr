
#include <cstddef>

template <typename T>
class ShrdArr {
private:
    T* ptr;
    std::size_t* referenceCount;

    void Release() {
        if (referenceCount != nullptr) {
            (*referenceCount)--;
            if (*referenceCount == 0) {
                delete[] ptr;
                delete referenceCount;
            }
        }
    }

public:
    explicit ShrdArr(T* p = nullptr){
        ptr = p;
        referenceCount = nullptr;
        if (ptr != nullptr) {
            referenceCount = new std::size_t(1);
        }
    }

    ~ShrdArr() {
        Release();
    }

    ShrdArr(const ShrdArr& other){
        ptr = other.ptr;
        referenceCount = other.referenceCount;
        if (referenceCount!= nullptr){
            (*referenceCount)++;
        }
    }

    ShrdArr& operator=(const ShrdArr& other){
        if (this != &other) {
            Release();
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            if (referenceCount != nullptr) {
                (*referenceCount)++;
            }
        }
        return *this;
    }

    ShrdArr(ShrdArr&& other) noexcept {
        ptr = other.ptr;
        referenceCount = other.referenceCount;
        other.ptr = nullptr;
        other.referenceCount = nullptr;
    }

    ShrdArr& operator=(ShrdArr&& other) noexcept {
        if (this != &other) {
            Release();
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            other.ptr = nullptr;
            other.referenceCount = nullptr;
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

    std::size_t use_count() const {
        if (referenceCount == nullptr) {
            return 0;
        }
        return *referenceCount;
    }

    void reset(T* p = nullptr) {
        if (ptr == p) {
            return;
        }
        Release();
        ptr = p;
        referenceCount = nullptr;
        if (ptr != nullptr) {
            referenceCount = new std::size_t(1);
        }
    }
};

