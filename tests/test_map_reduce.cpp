#include "MutableArraySequence.hpp"
#include "test_common.hpp"
#include <iostream>

bool testMapReduce() {
    std::cout << "\n=== Тестирование map и reduce ===" << std::endl;
    std::cout << "  Проверка map и reduce (MutableArraySequence<int>)" << std::endl;
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    MutableArraySequence<int>* mapped = seq.map<int>([](int x) { return x * 2; });
    TEST_ASSERT(mapped->GetLength() == 4, "длина map-результата не 4");
    TEST_ASSERT(mapped->Get(0) == 2, "map[0] не 2");
    TEST_ASSERT(mapped->Get(3) == 8, "map[3] не 8");
    printSequence(mapped, "    map(*2)");

    int sum = seq.reduce<int>([](int a, int b) { return a + b; }, 0);
    TEST_ASSERT(sum == 10, "сумма не 10");
    std::cout << "    reduce sum = " << sum << std::endl;

    double product = seq.reduce<double>([](double a, int b) { return a * b; }, 1.0);
    TEST_ASSERT(product == 24.0, "произведение не 24");
    std::cout << "    reduce product = " << product << std::endl;

    delete mapped;
    std::cout << "    OK" << std::endl;
    return true;
}
int main() {
    try {
        if (testMapReduce()) {
            std::cout << "\n=== Тесты MapReduce пройдены ===" << std::endl;
            return 0;
        } else {
            std::cerr << "\n=== Тесты MapReduce не пройдены ===" << std::endl;
            return 1;
        }
    } catch (const IException& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
        return 1;
    }
}