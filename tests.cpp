#include <iostream>
#include <windows.h>
#include "SmartPtrTest.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "Запуск модульных тестов с проверкой утечек...\n";

    TestUnqPtr();
    TestShrdPtr();
    TestUnqArr();
    TestShrdArr();
    TestSequence();

    std::cout << "Все тесты успешно пройдены! Утечек памяти не обнаружено.\n";
    return 0;
}