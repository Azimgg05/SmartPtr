cmake_minimum_required(VERSION 3.20) # Версии 4.1 еще не существует, 3.20 — оптимальная
project(SmartPointersLab)

set(CMAKE_CXX_STANDARD 17)

# 1. Первая программа: только модульные тесты (точка входа в tests.cpp)
add_executable(run_tests
        tests.cpp
        SmartPtrTest.cpp
        SmartPtrTest.h
        UnqPtr.h
        UnqPtrArr.h
        ShrdPtrArr.h
        ArraySequence.h
)

# 2. Вторая программа: только замеры скорости (точка входа в Benchmark.cpp)
add_executable(run_benchmark
        Benchmark.cpp
        UnqPtr.h
        UnqPtrArr.h
        ShrdPtrArr.h
        ArraySequence.h
)