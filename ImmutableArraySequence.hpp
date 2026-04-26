#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"

template <typename T>
class ImmutableArraySequence : public Sequence<T>
{
private:
    DynamicArray<T> *data;

public:
    ImmutableArraySequence(T *items, int count) : data(new DynamicArray<T>(items, count)) {}
    ImmutableArraySequence(int size) : data(new DynamicArray<T>(size)) {}
    ImmutableArraySequence(const ImmutableArraySequence &other) : data(new DynamicArray<T>(*other.data)) {}
    ImmutableArraySequence() : data(new DynamicArray<T>(0)) {}
    ~ImmutableArraySequence() { delete data; }

    T GetFirst() const override
    {
        if (GetLength() == 0)
            throw std::out_of_range("Последовательность пуста");
        return data->Get(0);
    }

    T GetLast() const override
    {
        int len = GetLength();
        if (len == 0)
            throw std::out_of_range("Последовательность пуста");
        return data->Get(len - 1);
    }

    T Get(int index) const override
    {
        return data->Get(index);
    }

    int GetLength() const override
    {
        return data->GetSize();
    }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex)
            throw std::out_of_range("Неверные индексы подпоследовательности");
        int newSize = endIndex - startIndex + 1;
        ImmutableArraySequence<T> *sub = new ImmutableArraySequence<T>(newSize);
        for (int i = 0; i < newSize; ++i)
            sub->data->Set(i, data->Get(startIndex + i));
        return sub;
    }

    Sequence<T> *Append(T item) override
    {
        int oldSize = GetLength();
        ImmutableArraySequence<T> *newSeq = new ImmutableArraySequence<T>(oldSize + 1);
        for (int i = 0; i < oldSize; ++i)
            newSeq->data->Set(i, data->Get(i));
        newSeq->data->Set(oldSize, item);
        return newSeq;
    }

    Sequence<T> *Prepend(T item) override
    {
        int oldSize = GetLength();
        ImmutableArraySequence<T> *newSeq = new ImmutableArraySequence<T>(oldSize + 1);
        newSeq->data->Set(0, item);
        for (int i = 0; i < oldSize; ++i)
            newSeq->data->Set(i + 1, data->Get(i));
        return newSeq;
    }

    Sequence<T> *InsertAt(T item, int index) override
    {
        int oldSize = GetLength();
        if (index < 0 || index > oldSize)
            throw std::out_of_range("Неверный индекс вставки");
        ImmutableArraySequence<T> *newSeq = new ImmutableArraySequence<T>(oldSize + 1);
        for (int i = 0; i < index; ++i)
            newSeq->data->Set(i, data->Get(i));
        newSeq->data->Set(index, item);
        for (int i = index; i < oldSize; ++i)
            newSeq->data->Set(i + 1, data->Get(i));
        return newSeq;
    }

    Sequence<T> *Concat(Sequence<T> *seq) const override
    {
        int thisLen = GetLength();
        int otherLen = seq->GetLength();
        ImmutableArraySequence<T> *result = new ImmutableArraySequence<T>(thisLen + otherLen);
        for (int i = 0; i < thisLen; ++i)
            result->data->Set(i, data->Get(i));
        for (int i = 0; i < otherLen; ++i)
            result->data->Set(thisLen + i, seq->Get(i));
        return result;
    }
};