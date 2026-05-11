#include <iostream>
#include <stdexcept>
#include <cassert>
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "MutableListSequence.hpp"
#include "ImmutableListSequence.hpp"
#include "BitSequence.hpp"
#include "Bit.hpp"


using namespace std;

template <typename T>
void printSequence(Sequence<T>* seq, const string& title)
{
    cout << title << " [";
    for (int i = 0; i < seq->GetLength(); ++i)
    {
        if (i > 0) cout << ", ";
        cout << seq->Get(i);
    }
    cout << "]" << endl;
}

void printBitSequence(Sequence<Bit>* seq, const string& title)
{
    cout << title << " [";
    for (int i = 0; i < seq->GetLength(); ++i)
    {
        if (i > 0) cout << ", ";
        cout << (seq->Get(i) == Bit::one ? "1" : "0");
    }
    cout << "]" << endl;
}


// Тесты для DynamicArray
void testDynamicArrayConstructionFromArray()
{
    cout << "  Проверка конструктора от массива и методов Get/GetSize" << endl;
    int src[] = {10, 20, 30};
    DynamicArray<int> arr(src, 3);
    assert(arr.GetSize() == 3);
    assert(arr.Get(0) == 10);
    assert(arr.Get(1) == 20);
    assert(arr.Get(2) == 30);
    cout << "    OK" << endl;
}

void testDynamicArraySet()
{
    cout << "  Проверка метода Set" << endl;
    int src[] = {1, 2, 3};
    DynamicArray<int> arr(src, 3);
    arr.Set(1, 99);
    assert(arr.Get(1) == 99);
    cout << "    OK" << endl;
}

void testDynamicArrayResizeLarger()
{
    cout << "  Проверка Resize (увеличение)" << endl;
    int src[] = {5, 6, 7};
    DynamicArray<int> arr(src, 3);
    arr.Resize(5);
    assert(arr.GetSize() == 5);
    assert(arr.Get(0) == 5);
    assert(arr.Get(1) == 6);
    assert(arr.Get(2) == 7);
    arr.Set(3, 8);
    arr.Set(4, 9);
    assert(arr.Get(3) == 8);
    assert(arr.Get(4) == 9);
    cout << "    OK" << endl;
}

void testDynamicArrayResizeSmaller()
{
    cout << "  Проверка Resize (уменьшение)" << endl;
    int src[] = {1, 2, 3, 4, 5};
    DynamicArray<int> arr(src, 5);
    arr.Resize(3);
    assert(arr.GetSize() == 3);
    assert(arr.Get(0) == 1);
    assert(arr.Get(1) == 2);
    assert(arr.Get(2) == 3);
    cout << "    OK" << endl;
}

void testDynamicArrayExceptionOutOfRange()
{
    cout << "  Проверка исключения при выходе за границы" << endl;
    int src[] = {1, 2, 3};
    DynamicArray<int> arr(src, 3);
    try {
        arr.Get(5);
        assert(false && "Исключение не выброшено");
    } catch (const out_of_range& e) {
        cout << "    Исключение перехвачено: " << e.what() << endl;
    }
    try {
        arr.Set(5, 0);
        assert(false);
    } catch (const out_of_range& e) {
        cout << "    Исключение перехвачено: " << e.what() << endl;
    }
    cout << "    OK" << endl;
}

void testDynamicArray()
{
    cout << "\n=== Тестирование DynamicArray ===" << endl;
    testDynamicArrayConstructionFromArray();
    testDynamicArraySet();
    testDynamicArrayResizeLarger();
    testDynamicArrayResizeSmaller();
    testDynamicArrayExceptionOutOfRange();
}

// Тесты для LinkedList
void testLinkedListConstructionAndGetters()
{
    cout << "  Проверка конструктора от массива, GetFirst, GetLast, GetLength" << endl;
    int items[] = {10, 20, 30, 40};
    LinkedList<int> list(items, 4);
    assert(list.GetLength() == 4);
    assert(list.GetFirst() == 10);
    assert(list.GetLast() == 40);
    cout << "    OK" << endl;
}

void testLinkedListAppendPrepend()
{
    cout << "  Проверка Append и Prepend" << endl;
    int items[] = {20, 30};
    LinkedList<int> list(items, 2);
    list.Append(40);
    list.Prepend(10);
    assert(list.GetLength() == 4);
    assert(list.Get(0) == 10);
    assert(list.Get(1) == 20);
    assert(list.Get(2) == 30);
    assert(list.Get(3) == 40);
    cout << "    OK" << endl;
}

void testLinkedListInsertAt()
{
    cout << "  Проверка InsertAt" << endl;
    int items[] = {1, 2, 4, 5};
    LinkedList<int> list(items, 4);
    list.InsertAt(3, 2);   // вставка в середину
    assert(list.GetLength() == 5);
    assert(list.Get(2) == 3);
    list.InsertAt(0, 0);   // вставка в начало
    assert(list.Get(0) == 0);
    list.InsertAt(6, list.GetLength()); // вставка в конец
    assert(list.Get(list.GetLength() - 1) == 6);
    cout << "    OK" << endl;
}

void testLinkedListGetSubList()
{
    cout << "  Проверка GetSubList" << endl;
    int items[] = {0, 1, 2, 3, 4, 5};
    LinkedList<int> list(items, 6);
    LinkedList<int>* sub = list.GetSubList(1, 4);
    assert(sub->GetLength() == 4); // индексы 1,2,3,4
    assert(sub->Get(0) == 1);
    assert(sub->Get(3) == 4);
    delete sub;
    cout << "    OK" << endl;
}

void testLinkedListConcat()
{
    cout << "  Проверка Concat (разрушающая)" << endl;
    int a[] = {1, 2};
    int b[] = {3, 4};
    LinkedList<int> listA(a, 2);
    LinkedList<int> listB(b, 2);
    listA.Concat(listB);
    assert(listA.GetLength() == 4);
    assert(listA.Get(2) == 3);
    assert(listA.Get(3) == 4);
    assert(listB.GetLength() == 0); // listB должен стать пустым
    cout << "    OK" << endl;
}

void testLinkedListExceptionEmpty()
{
    cout << "  Проверка исключения при обращении к пустому списку" << endl;
    LinkedList<int> empty;
    try {
        empty.GetFirst();
        assert(false);
    } catch (const out_of_range& e) {
        cout << "    Исключение перехвачено: " << e.what() << endl;
    }
    cout << "    OK" << endl;
}

void testLinkedList()
{
    cout << "\n=== Тестирование LinkedList ===" << endl;
    testLinkedListConstructionAndGetters();
    testLinkedListAppendPrepend();
    testLinkedListInsertAt();
    testLinkedListGetSubList();
    testLinkedListConcat();
    testLinkedListExceptionEmpty();
}

// Тесты для MutableArraySequence
void testMutableArraySequenceBasic()
{
    cout << "  Проверка базовых методов (GetFirst, GetLast, Get, GetLength)" << endl;
    int data[] = {5, 10, 15};
    MutableArraySequence<int> seq(data, 3);
    assert(seq.GetLength() == 3);
    assert(seq.GetFirst() == 5);
    assert(seq.GetLast() == 15);
    assert(seq.Get(1) == 10);
    cout << "    OK" << endl;
}

void testMutableArraySequenceAppendPrependInsert()
{
    cout << "  Проверка Append, Prepend, InsertAt (изменяют текущий объект)" << endl;
    int data[] = {20, 30};
    MutableArraySequence<int> seq(data, 2);
    seq.Append(40);
    seq.Prepend(10);
    seq.InsertAt(25, 2);
    assert(seq.GetLength() == 5);
    assert(seq.Get(0) == 10);
    assert(seq.Get(1) == 20);
    assert(seq.Get(2) == 25);
    assert(seq.Get(3) == 30);
    assert(seq.Get(4) == 40);
    cout << "    OK" << endl;
}

void testMutableArraySequenceGetSubsequence()
{
    cout << "  Проверка GetSubsequence" << endl;
    int data[] = {0, 1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 6);
    Sequence<int>* sub = seq.GetSubsequence(2, 4);
    assert(sub->GetLength() == 3); // индексы 2,3,4
    assert(sub->Get(0) == 2);
    assert(sub->Get(2) == 4);
    delete sub;
    cout << "    OK" << endl;
}

void testMutableArraySequenceConcat()
{
    cout << "  Проверка Concat (возвращает новый объект, не изменяет текущий)" << endl;
    int a[] = {1, 2};
    int b[] = {3, 4};
    MutableArraySequence<int> seqA(a, 2);
    MutableArraySequence<int> seqB(b, 2);
    Sequence<int>* concat = seqA.Concat(&seqB);
    assert(concat->GetLength() == 4);
    assert(concat->Get(0) == 1);
    assert(concat->Get(2) == 3);
    assert(seqA.GetLength() == 2); // исходные не изменились
    delete concat;
    cout << "    OK" << endl;
}

void testMutableArraySequence()
{
    cout << "\n=== Тестирование MutableArraySequence ===" << endl;
    testMutableArraySequenceBasic();
    testMutableArraySequenceAppendPrependInsert();
    testMutableArraySequenceGetSubsequence();
    testMutableArraySequenceConcat();
}

// Тесты для ImmutableArraySequence
void testImmutableArraySequenceBasic()
{
    cout << "  Проверка базовых методов (GetFirst, GetLast, Get, GetLength)" << endl;
    int data[] = {7, 8, 9};
    ImmutableArraySequence<int> seq(data, 3);
    assert(seq.GetLength() == 3);
    assert(seq.GetFirst() == 7);
    assert(seq.GetLast() == 9);
    assert(seq.Get(1) == 8);
    cout << "    OK" << endl;
}

void testImmutableArraySequenceAppend()
{
    cout << "  Проверка Append (возвращает новый объект)" << endl;
    int data[] = {1, 2};
    ImmutableArraySequence<int> orig(data, 2);
    Sequence<int>* newSeq = orig.Append(3);
    assert(orig.GetLength() == 2);   // не изменилась
    assert(newSeq->GetLength() == 3);
    assert(newSeq->Get(2) == 3);
    delete newSeq;
    cout << "    OK" << endl;
}

void testImmutableArraySequencePrepend()
{
    cout << "  Проверка Prepend (возвращает новый объект)" << endl;
    int data[] = {2, 3};
    ImmutableArraySequence<int> orig(data, 2);
    Sequence<int>* newSeq = orig.Prepend(1);
    assert(orig.GetLength() == 2);
    assert(newSeq->GetLength() == 3);
    assert(newSeq->Get(0) == 1);
    delete newSeq;
    cout << "    OK" << endl;
}

void testImmutableArraySequenceInsertAt()
{
    cout << "  Проверка InsertAt (возвращает новый объект)" << endl;
    int data[] = {1, 3, 4};
    ImmutableArraySequence<int> orig(data, 3);
    Sequence<int>* newSeq = orig.InsertAt(2, 1);
    assert(orig.GetLength() == 3);
    assert(newSeq->GetLength() == 4);
    assert(newSeq->Get(1) == 2);
    delete newSeq;
    cout << "    OK" << endl;
}

void testImmutableArraySequence()
{
    cout << "\n=== Тестирование ImmutableArraySequence ===" << endl;
    testImmutableArraySequenceBasic();
    testImmutableArraySequenceAppend();
    testImmutableArraySequencePrepend();
    testImmutableArraySequenceInsertAt();
}
// Тесты для MutableListSequence

void testMutableListSequenceBasic()
{
    cout << "  Проверка базовых методов" << endl;
    int data[] = {11, 22, 33};
    MutableListSequence<int> seq(data, 3);
    assert(seq.GetLength() == 3);
    assert(seq.GetFirst() == 11);
    assert(seq.GetLast() == 33);
    assert(seq.Get(1) == 22);
    cout << "    OK" << endl;
}

void testMutableListSequenceAppendPrependInsert()
{
    cout << "  Проверка Append, Prepend, InsertAt" << endl;
    int data[] = {20, 30};
    MutableListSequence<int> seq(data, 2);
    seq.Append(40);
    seq.Prepend(10);
    seq.InsertAt(25, 2);
    assert(seq.GetLength() == 5);
    assert(seq.Get(0) == 10);
    assert(seq.Get(2) == 25);
    assert(seq.Get(4) == 40);
    cout << "    OK" << endl;
}

void testMutableListSequence()
{
    cout << "\n=== Тестирование MutableListSequence ===" << endl;
    testMutableListSequenceBasic();
    testMutableListSequenceAppendPrependInsert();
}

// Тесты для ImmutableListSequence

void testImmutableListSequenceBasic()
{
    cout << "  Проверка базовых методов" << endl;
    int data[] = {5, 6, 7};
    ImmutableListSequence<int> seq(data, 3);
    assert(seq.GetLength() == 3);
    assert(seq.GetFirst() == 5);
    assert(seq.GetLast() == 7);
    cout << "    OK" << endl;
}

void testImmutableListSequenceAppend()
{
    cout << "  Проверка Append (возвращает новый объект)" << endl;
    int data[] = {1, 2};
    ImmutableListSequence<int> orig(data, 2);
    Sequence<int>* newSeq = orig.Append(3);
    assert(orig.GetLength() == 2);
    assert(newSeq->GetLength() == 3);
    assert(newSeq->Get(2) == 3);
    delete newSeq;
    cout << "    OK" << endl;
}

void testImmutableListSequence()
{
    cout << "\n=== Тестирование ImmutableListSequence ===" << endl;
    testImmutableListSequenceBasic();
    testImmutableListSequenceAppend();
}

// Тесты для BitSequence

void testBitSequenceConstruction()
{
    cout << "  Проверка конструкторов и Get" << endl;
    Bit bits[] = {Bit::one, Bit::zero, Bit::one};
    BitSequence seq(bits, 3);
    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == Bit::one);
    assert(seq.Get(1) == Bit::zero);
    assert(seq.Get(2) == Bit::one);
    printBitSequence(&seq, "  bitSeq");
    cout << "    OK" << endl;
}

void testBitSequenceAppend()
{
    cout << "  Проверка Append (immutable)" << endl;
    Bit bits[] = {Bit::one, Bit::zero};
    BitSequence orig(bits, 2);
    Sequence<Bit>* newSeq = orig.Append(Bit::one);
    assert(orig.GetLength() == 2);
    assert(newSeq->GetLength() == 3);
    assert(newSeq->Get(2) == Bit::one);
    delete newSeq;
    cout << "    OK" << endl;
}

void testBitSequencePrepend()
{
    cout << "  Проверка Prepend (immutable)" << endl;
    Bit bits[] = {Bit::zero, Bit::one};
    BitSequence orig(bits, 2);
    Sequence<Bit>* newSeq = orig.Prepend(Bit::one);
    assert(orig.GetLength() == 2);
    assert(newSeq->GetLength() == 3);
    assert(newSeq->Get(0) == Bit::one);
    delete newSeq;
    cout << "    OK" << endl;
}

void testBitSequenceInsertAt()
{
    cout << "  Проверка InsertAt (immutable)" << endl;
    Bit bits[] = {Bit::one, Bit::one, Bit::zero};
    BitSequence orig(bits, 3);
    Sequence<Bit>* newSeq = orig.InsertAt(Bit::zero, 1);
    assert(orig.GetLength() == 3);
    assert(newSeq->GetLength() == 4);
    assert(newSeq->Get(1) == Bit::zero);
    delete newSeq;
    cout << "    OK" << endl;
}

void testBitSequenceConcat()
{
    cout << "  Проверка Concat (immutable)" << endl;
    Bit a[] = {Bit::one, Bit::zero};
    Bit b[] = {Bit::one, Bit::one};
    BitSequence seqA(a, 2);
    BitSequence seqB(b, 2);
    Sequence<Bit>* concat = seqA.Concat(&seqB);
    assert(concat->GetLength() == 4);
    assert(concat->Get(0) == Bit::one);
    assert(concat->Get(2) == Bit::one);
    delete concat;
    cout << "    OK" << endl;
}

void testBitSequence()
{
    cout << "\n=== Тестирование BitSequence ===" << endl;
    testBitSequenceConstruction();
    testBitSequenceAppend();
    testBitSequencePrepend();
    testBitSequenceInsertAt();
    testBitSequenceConcat();
}

void testMapReduce()
{
    cout << "  Проверка map и reduce (MutableArraySequence<int>)" << endl;
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    MutableArraySequence<int>* mapped = seq.map<int>([](int x) { return x * 2; });
    assert(mapped->GetLength() == 4);
    assert(mapped->Get(0) == 2);
    assert(mapped->Get(3) == 8);
    printSequence(mapped, "    map(*2)");

    int sum = seq.reduce<int>([](int a, int b) { return a + b; }, 0);
    assert(sum == 10);
    cout << "    reduce sum = " << sum << endl;

    double product = seq.reduce<double>([](double a, int b) { return a * b; }, 1.0);
    assert(product == 24.0);
    cout << "    reduce product = " << product << endl;

    delete mapped;
    cout << "    OK" << endl;
}

int main()
{
    try
    {
        testMapReduce();
        testDynamicArray();
        testLinkedList();
        testMutableArraySequence();
        testImmutableArraySequence();
        testMutableListSequence();
        testImmutableListSequence();
        testBitSequence();
        cout << "\n=== Все тесты успешно пройдены! ===" << endl;
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    return 0;
}