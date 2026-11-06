#include <iostream>
#include <chrono>
#include <utility>
#include <windows.h>
#include <iomanip>

#include "UnqPtr.h"
#include "ShrdPtr.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const int N = 200000;
    long long checksum = 0;

    std::cout << "=== НАГРУЗОЧНОЕ ТЕСТИРОВАНИЕ (" << N << " итераций) ===\n\n";

    auto start = std::chrono::steady_clock::now();
    int** rawPointers = new int*[N];
    for (int i = 0; i < N; ++i) {
        rawPointers[i] = new int(i % 10);
    }
    auto end = std::chrono::steady_clock::now();
    auto rawCreate = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    for (int i = 0; i < N; ++i) { checksum += *rawPointers[i]; }

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        delete rawPointers[i];
    }
    delete[] rawPointers;
    end = std::chrono::steady_clock::now();
    auto rawDestroy = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    start = std::chrono::steady_clock::now();
    UnqPtr<int>* uniquePointers = new UnqPtr<int>[N];
    for (int i = 0; i < N; ++i) {
        uniquePointers[i].reset(new int(i % 10));
    }
    end = std::chrono::steady_clock::now();
    auto uniqueCreate = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    for (int i = 0; i < N; ++i) { checksum += *uniquePointers[i]; }

    start = std::chrono::steady_clock::now();
    delete[] uniquePointers;
    end = std::chrono::steady_clock::now();
    auto uniqueDestroy = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    UnqPtr<int> moveSource(new int(10));
    UnqPtr<int> moveDestination;
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        moveDestination = std::move(moveSource);
        moveSource = std::move(moveDestination);
    }
    end = std::chrono::steady_clock::now();
    auto uniqueMove = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    checksum += *moveSource;

    start = std::chrono::steady_clock::now();
    ShrdPtr<int>* sharedPointers = new ShrdPtr<int>[N];
    for (int i = 0; i < N; ++i) {
        sharedPointers[i].reset(new int(i % 10));
    }
    end = std::chrono::steady_clock::now();
    auto sharedCreate = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    for (int i = 0; i < N; ++i) { checksum += *sharedPointers[i]; }

    start = std::chrono::steady_clock::now();
    delete[] sharedPointers;
    end = std::chrono::steady_clock::now();
    auto sharedDestroy = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    ShrdPtr<int> sharedSource(new int(20));
    ShrdPtr<int>* sharedCopies = new ShrdPtr<int>[N];
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        sharedCopies[i] = sharedSource;
    }
    end = std::chrono::steady_clock::now();
    auto sharedCopy = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    delete[] sharedCopies;

    std::cout << std::left << std::setw(30) << "Операция" << "Время (микросекунды)\n";
    std::cout << std::string(55, '-') << "\n";
    std::cout << std::setw(30) << "Raw pointer creation:" << rawCreate << "\n";
    std::cout << std::setw(30) << "Raw pointer destruction:" << rawDestroy << "\n";
    std::cout << std::setw(30) << "UnqPtr creation:" << uniqueCreate << "\n";
    std::cout << std::setw(30) << "UnqPtr destruction:" << uniqueDestroy << "\n";
    std::cout << std::setw(30) << "UnqPtr move:" << uniqueMove << "\n";
    std::cout << std::setw(30) << "ShrdPtr creation:" << sharedCreate << "\n";
    std::cout << std::setw(30) << "ShrdPtr destruction:" << sharedDestroy << "\n";
    std::cout << std::setw(30) << "ShrdPtr copy:" << sharedCopy << "\n";
    std::cout << "\nChecksum: " << checksum << "\n";

    return 0;
}