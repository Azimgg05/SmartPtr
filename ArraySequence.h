#ifndef ARRAYSEQUENCE_H
#define ARRAYSEQUENCE_H

#include <stdexcept>
#include <utility>
#include "Sequence.h"

template <typename T>
class ArraySequence : public Sequence<T> {
private:
    T* items;
    int count;
    int capacity;

public:
    ArraySequence(){
        items = nullptr;
        count = 0;
        capacity = 0;
    }
    ~ArraySequence() override {
        delete[] items;
    }

    ArraySequence(const ArraySequence& other){
        count = other.count;
        capacity = other.capacity;
        items = new T[capacity];
            for (int i = 0; i < count; ++i) {
                items[i] = other.items[i];
            }
        }


    ArraySequence& operator=(const ArraySequence& other) {
        if (this != &other) {
            delete[] items;
            count = other.count;
            capacity = other.capacity;
            items = new T[capacity];
            for (int i = 0; i < count; ++i) {
                items[i] = other.items[i];
            }
        }
        return *this;
    }

    ArraySequence(ArraySequence&& other) noexcept {
        items = other.items;
        count = other.count;
        capacity = other.capacity;
        other.items = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    ArraySequence& operator=(ArraySequence&& other) noexcept {
        if (this != &other) {
            delete[] items;
            items = other.items;
            count = other.count;
            capacity = other.capacity;
            other.items = nullptr;
            other.count = 0;
            other.capacity = 0;
        }
        return *this;
    }


    const T& Get(int index) const override {
        if (index < 0 || index >= count) {
            throw std::out_of_range("ArraySequence::Get: индекс вне диапозона");
        }
        return items[index];
    }

    T& Get(int index) override {
        if (index < 0 || index >= count) {
            throw std::out_of_range("ArraySequence::Get: индекс вне диапозона");
        }
        return items[index];
    }

    int GetLength() const override {
        return count;
    }

    void Append(const T& item) override {
        if (count == capacity) {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            T* newItems = new T[newCapacity];

            for (int i = 0; i < count; ++i) {
                newItems[i] = std::move(items[i]);
            }

            delete[] items;
            items = newItems;
            capacity = newCapacity;
        }

        items[count] = item;
        ++count;
    }
};

#endif