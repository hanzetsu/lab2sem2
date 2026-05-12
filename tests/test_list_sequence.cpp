#include "MutableListSequence.hpp"
#include "ImmutableListSequence.hpp"
#include "exceptions.hpp"
#include "test_common.hpp"
#include <iostream>

bool testMutableListSequenceBasic() {
    std::cout << "  Проверка базовых методов MutableListSequence" << std::endl;
    int data[] = {11, 22, 33};
    MutableListSequence<int> seq(data, 3);
    TEST_ASSERT(seq.GetLength() == 3, "длина не 3");
    TEST_ASSERT(seq.GetFirst() == 11, "GetFirst не 11");
    TEST_ASSERT(seq.GetLast() == 33, "GetLast не 33");
    TEST_ASSERT(seq.Get(1) == 22, "Get(1) не 22");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMutableListSequenceAppendPrependInsert() {
    std::cout << "  Проверка Append, Prepend, InsertAt MutableListSequence" << std::endl;
    int data[] = {20, 30};
    MutableListSequence<int> seq(data, 2);
    seq.Append(40);
    seq.Prepend(10);
    seq.InsertAt(25, 2);
    TEST_ASSERT(seq.GetLength() == 5, "длина не 5");
    TEST_ASSERT(seq.Get(0) == 10, "эл.0 не 10");
    TEST_ASSERT(seq.Get(2) == 25, "эл.2 не 25");
    TEST_ASSERT(seq.Get(4) == 40, "эл.4 не 40");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMutableListSequence() {
    std::cout << "\n=== Тестирование MutableListSequence ===" << std::endl;
    if (!testMutableListSequenceBasic()) return false;
    if (!testMutableListSequenceAppendPrependInsert()) return false;
    return true;
}

bool testImmutableListSequenceBasic() {
    std::cout << "  Проверка базовых методов ImmutableListSequence" << std::endl;
    int data[] = {5, 6, 7};
    ImmutableListSequence<int> seq(data, 3);
    TEST_ASSERT(seq.GetLength() == 3, "длина не 3");
    TEST_ASSERT(seq.GetFirst() == 5, "GetFirst не 5");
    TEST_ASSERT(seq.GetLast() == 7, "GetLast не 7");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testImmutableListSequenceAppend() {
    std::cout << "  Проверка Append ImmutableListSequence (возвращает новый объект)" << std::endl;
    int data[] = {1, 2};
    ImmutableListSequence<int> orig(data, 2);
    Sequence<int>* newSeq = orig.Append(3);
    TEST_ASSERT(orig.GetLength() == 2, "исходная изменилась");
    TEST_ASSERT(newSeq->GetLength() == 3, "новая длина не 3");
    TEST_ASSERT(newSeq->Get(2) == 3, "добавленный элемент не 3");
    delete newSeq;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testImmutableListSequence() {
    std::cout << "\n=== Тестирование ImmutableListSequence ===" << std::endl;
    if (!testImmutableListSequenceBasic()) return false;
    if (!testImmutableListSequenceAppend()) return false;
    return true;
}

bool testListSequence() {
    if (!testMutableListSequence()) return false;
    if (!testImmutableListSequence()) return false;
    return true;
}

int main() {
    try {
        if (testListSequence()) {
            std::cout << "\n=== Тесты ListSequence пройдены ===" << std::endl;
            return 0;
        } else {
            std::cerr << "\n=== Тесты ListSequence не пройдены ===" << std::endl;
            return 1;
        }
    } catch (const IException& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
        return 1;
    }
}