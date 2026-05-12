#include "BitSequence.hpp"
#include "exceptions.hpp"
#include "test_common.hpp"
#include <iostream>

bool testBitSequenceConstruction()
{
    std::cout << "  Проверка конструкторов и Get" << std::endl;
    Bit bits[] = {Bit::one, Bit::zero, Bit::one};
    BitSequence seq(bits, 3);
    TEST_ASSERT(seq.GetLength() == 3, "Длина не 3");
    TEST_ASSERT(seq.Get(0) == Bit::one, "бит 0 не 1");
    TEST_ASSERT(seq.Get(1) == Bit::zero, "бит 1 не 0");
    TEST_ASSERT(seq.Get(2) == Bit::one, "бит 2 не 1");
    printBitSequence(&seq, "  bitSeq");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testBitSequenceAppend()
{
    std::cout << "  Проверка Append (mutable)" << std::endl;
    Bit bits[] = {Bit::one, Bit::zero};
    BitSequence seq(bits, 2);
    seq.Append(Bit::one);
    TEST_ASSERT(seq.GetLength() == 3, "длина не 3");
    TEST_ASSERT(seq.Get(2) == Bit::one, "добавленный бит не 1");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testBitSequencePrepend()
{
    std::cout << "  Проверка Prepend (immutable)" << std::endl;
    Bit bits[] = {Bit::zero, Bit::one};
    BitSequence orig(bits, 2);
    Sequence<Bit> *newSeq = orig.Prepend(Bit::one);
    TEST_ASSERT(orig.GetLength() == 2, "исходная длина изменилась");
    TEST_ASSERT(newSeq->GetLength() == 3, "новая длина не 3");
    TEST_ASSERT(newSeq->Get(0) == Bit::one, "первый бит не 1");
    delete newSeq;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testBitSequenceInsertAt()
{
    std::cout << "  Проверка InsertAt (immutable)" << std::endl;
    Bit bits[] = {Bit::one, Bit::one, Bit::zero};
    BitSequence orig(bits, 3);
    Sequence<Bit> *newSeq = orig.InsertAt(Bit::zero, 1);
    TEST_ASSERT(orig.GetLength() == 3, "исходная длина изменилась");
    TEST_ASSERT(newSeq->GetLength() == 4, "новая длина не 4");
    TEST_ASSERT(newSeq->Get(1) == Bit::zero, "бит на позиции 1 не 0");
    delete newSeq;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testBitSequenceConcat()
{
    std::cout << "  Проверка Concat (immutable)" << std::endl;
    Bit a[] = {Bit::one, Bit::zero};
    Bit b[] = {Bit::one, Bit::one};
    BitSequence seqA(a, 2);
    BitSequence seqB(b, 2);
    Sequence<Bit> *concat = seqA.Concat(&seqB);
    TEST_ASSERT(concat->GetLength() == 4, "длина результата не 4");
    TEST_ASSERT(concat->Get(0) == Bit::one, "0-й бит не 1");
    TEST_ASSERT(concat->Get(2) == Bit::one, "2-й бит не 1");
    delete concat;
    std::cout << "    OK" << std::endl;
    return true;
}

bool testBitSequence()
{
    std::cout << "\n=== Тестирование BitSequence ===" << std::endl;
    auto tests = DynamicArray{
        testBitSequenceConstruction,
        testBitSequenceAppend,
        testBitSequencePrepend,
        testBitSequenceInsertAt,
        testBitSequenceConcat};
    return std::ranges::all_of(tests, [](auto f)
                               { return f(); });
}
int main()
{
    try
    {
        if (testBitSequence())
        {
            std::cout << "\n=== Тесты BitSequence пройдены ===" << std::endl;
            return 0;
        }
        else
        {
            std::cerr << "\n=== Тесты BitSequence не пройдены ===" << std::endl;
            return 1;
        }
    }
    catch (const IException &e)
    {
        std::cerr << "Исключение: " << e.what() << std::endl;
        return 1;
    }
}