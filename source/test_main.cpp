#include <iostream>
#include "exceptions.hpp"

bool testBitSequence();
bool testArraySequence();
bool testListSequence();
bool testMapReduce();

int main() {
    try {
        bool allPassed = true;
        allPassed = testBitSequence() && allPassed;
        allPassed = testArraySequence() && allPassed;
        allPassed = testListSequence() && allPassed;
        allPassed = testMapReduce() && allPassed;

        if (allPassed) {
            std::cout << "\n=== Все тесты успешно пройдены! ===" << std::endl;
            return 0;
        } else {
            std::cerr << "\n=== Тесты не пройдены ===" << std::endl;
            return 1;
        }
    } catch (const IException& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Стандартное исключение: " << e.what() << std::endl;
        return 1;
    }
}