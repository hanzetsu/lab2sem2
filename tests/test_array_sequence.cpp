#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "exceptions.hpp"
#include "test_common.hpp"
#include <iostream>
#include <algorithm>
#include <ranges>
#include <functional>
#include "DynamicArray.hpp"

bool testMutableArraySequenceBasic() {
    std::cout << "  Проверка базовых методов MutableArraySequence" << std::endl;
    int data[] = {5, 10, 15};
    MutableArraySequence<int> seq(data, 3);
    TEST_ASSERT(seq.GetLength() == 3, "длина не 3");
    TEST_ASSERT(seq.GetFirst() == 5, "GetFirst не 5");
    TEST_ASSERT(seq.GetLast() == 15, "GetLast не 15");
    TEST_ASSERT(seq.Get(1) == 10, "Get(1) не 10");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMutableArraySequenceAppendPrependInsert() {
    std::cout << "  Проверка Append, Prepend, InsertAt MutableArraySequence" << std::endl;
    int data[] = {20, 30};
    MutableArraySequence<int> seq(data, 2);
    seq.Append(40);
    seq.Prepend(10);
    seq.InsertAt(25, 2);
    TEST_ASSERT(seq.GetLength() == 5, "длина не 5");
    TEST_ASSERT(seq.Get(0) == 10, "эл.0 не 10");
    TEST_ASSERT(seq.Get(1) == 20, "эл.1 не 20");
    TEST_ASSERT(seq.Get(2) == 25, "эл.2 не 25");
    TEST_ASSERT(seq.Get(3) == 30, "эл.3 не 30");
    TEST_ASSERT(seq.Get(4) == 40, "эл.4 не 40");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMutableArraySequenceGetSubsequence() {
    std::cout << "  Проверка GetSubsequence MutableArraySequence" << std::endl;
    int data[] = {0, 1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 6);
    Sequence<int>* sub = seq.GetSubsequence(2, 4);
    TEST_ASSERT(sub->GetLength() == 3, "длина подпоследовательности не 3");
    TEST_ASSERT(sub->Get(0) == 2, "sub[0] не 2");
    TEST_ASSERT(sub->Get(2) == 4, "sub[2] не 4");
    delete sub;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMutableArraySequenceConcat() {
    std::cout << "  Проверка Concat MutableArraySequence (возвращает новый объект)" << std::endl;
    int a[] = {1, 2};
    int b[] = {3, 4};
    MutableArraySequence<int> seqA(a, 2);
    MutableArraySequence<int> seqB(b, 2);
    Sequence<int>* concat = seqA.Concat(&seqB);
    TEST_ASSERT(concat->GetLength() == 4, "длина результата не 4");
    TEST_ASSERT(concat->Get(0) == 1, "concat[0] не 1");
    TEST_ASSERT(concat->Get(2) == 3, "concat[2] не 3");
    TEST_ASSERT(seqA.GetLength() == 2, "seqA изменилась");
    delete concat;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMutableArraySequence() {
    std::cout << "\n=== Тестирование MutableArraySequence ===" << std::endl;
    DynamicArray Container { testMutableArraySequenceGetSubsequence,testMutableArraySequenceAppendPrependInsert,
    testMutableArraySequenceConcat};
    int i;
    return std::all_of(Container.begin(), Container.end(), [&i](auto b){ 
        std::cout << i++ << std::endl;
        return b();});
}

bool testImmutableArraySequenceBasic() {
    std::cout << "  Проверка базовых методов ImmutableArraySequence" << std::endl;
    int data[] = {7, 8, 9};
    ImmutableArraySequence<int> seq(data, 3);
    TEST_ASSERT(seq.GetLength() == 3, "длина не 3");
    TEST_ASSERT(seq.GetFirst() == 7, "GetFirst не 7");
    TEST_ASSERT(seq.GetLast() == 9, "GetLast не 9");
    TEST_ASSERT(seq.Get(1) == 8, "Get(1) не 8");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testImmutableArraySequenceAppend() {
    std::cout << "  Проверка Append ImmutableArraySequence (возвращает новый объект)" << std::endl;
    int data[] = {1, 2};
    ImmutableArraySequence<int> orig(data, 2);
    Sequence<int>* newSeq = orig.Append(3);
    TEST_ASSERT(orig.GetLength() == 2, "исходная изменилась");
    TEST_ASSERT(newSeq->GetLength() == 3, "новая длина не 3");
    TEST_ASSERT(newSeq->Get(2) == 3, "добавленный элемент не 3");
    delete newSeq;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testImmutableArraySequencePrepend() {
    std::cout << "  Проверка Prepend ImmutableArraySequence (возвращает новый объект)" << std::endl;
    int data[] = {2, 3};
    ImmutableArraySequence<int> orig(data, 2);
    Sequence<int>* newSeq = orig.Prepend(1);
    TEST_ASSERT(orig.GetLength() == 2, "исходная изменилась");
    TEST_ASSERT(newSeq->GetLength() == 3, "новая длина не 3");
    TEST_ASSERT(newSeq->Get(0) == 1, "первый элемент не 1");
    delete newSeq;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testImmutableArraySequenceInsertAt() {
    std::cout << "  Проверка InsertAt ImmutableArraySequence (возвращает новый объект)" << std::endl;
    int data[] = {1, 3, 4};
    ImmutableArraySequence<int> orig(data, 3);
    Sequence<int>* newSeq = orig.InsertAt(2, 1);
    TEST_ASSERT(orig.GetLength() == 3, "исходная изменилась");
    TEST_ASSERT(newSeq->GetLength() == 4, "новая длина не 4");
    TEST_ASSERT(newSeq->Get(1) == 2, "вставленный элемент не 2");
    delete newSeq;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testImmutableArraySequence() {
    std::cout << "\n=== Тестирование ImmutableArraySequence ===" << std::endl;
    auto tests = DynamicArray {testImmutableArraySequenceAppend,
    testImmutableArraySequencePrepend, testImmutableArraySequenceInsertAt};
    return std::ranges::all_of(tests, [](auto b){return b();});
}

bool testArraySequence() {
    return testMutableArraySequence();
    // if (!testMutableArraySequence()) return false;
    // if (!testImmutableArraySequence()) return false;
    // return true;
}
int main() {
    try {
        if (testArraySequence()) {
            std::cout << "\n=== Тесты ArraySequence пройдены ===" << std::endl;
            return 0;
        } else {
            std::cerr << "\n=== Тесты ArraySequence не пройдены ===" << std::endl;
            return 1;
        }
    } catch (const IException& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
        return 1;
    }
}