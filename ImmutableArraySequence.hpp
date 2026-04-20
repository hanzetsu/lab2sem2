#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"

template <typename T>
class ImmutableArraySequence : public Sequence<T>
{
private:
    DynamicArray<T> *data;

public:
    ArraySequence(T *items, int count) : data(new DynamicArray<T>(items, count)) {}
    ArraySequence(int size) : data(new DynamicArray<T>(size)) {}
    ArraySequence(const ArraySequence &other) : data(new DynamicArray<T>(*other.data)) {}
    ~ArraySequence() { delete data; }

    T GetFirst() const override
    {
        return data->Get(0);
    }

    T GetLast() const override
    {
        return data->Get(data->GetSize() - 1);
    }

    T Get(int index) const override
    {
        return data->Get(index);
    }

    int GetLength() const override
    {
        return data->GetSize();
    }

    Sequence<T> *Append(T item) override
    {
        int size = data->GetSize();
        ArraySequence<T> *newSequence = new ArraySequence<T>(size + 1);
        for (int i = 0; i < size; ++i)
        {
            newSequence->data->Set(i, data->Get(i));
        }
        newSequence->data->Set(size, item);
        return newSequence;
    }

    Sequence<T> *Prepend(T item) override
    {
        int size = data->GetSize();
        ArraySequence<T> *newSequence = new ArraySequence<T>(size + 1);
        newSequence->data->Set(0, item);
        for (int i = 0; i < size; ++i)
        {
            newSequence->data->Set(i + 1, data->Get(i));
        }
        return newSequence;
    }

    Sequence<T> *InsertAt(T item, int index) override
    {
        int size = data->GetSize();
        ArraySequence<T> *newSequence = new ArraySequence<T>(size + 1);
        for (int i = 0; i < index; ++i)
        {
            newSequence->data->Set(i, data->Get(i));
        }
        newSequence->data->Set(index, item);
        for (int i = index; i < size; ++i)
        {
            newSequence->data->Set(i + 1, data->Get(i));
        }
        return newSequence;
    }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        int newSize = endIndex - startIndex + 1;
        ArraySequence<T> *newSequence = new ArraySequence<T>(newSize);
        for (int i = 0; i < newSize; ++i)
        {
            newSequence->data->Set(i, data->Get(startIndex + i));
        }
        return newSequence;
    }

    Sequence<T> *Concat(Sequence<T> *seq) const override
    {
        int thisLen = GetLength();
        int otherLen = seq->GetLength();
        ArraySequence<T> *result = new ArraySequence<T>(thisLen + otherLen);
        for (int i = 0; i < thisLen; ++i)
        {
            result->data->Set(i, data->Get(i));
        }
        for (int i = 0; i < otherLen; ++i)
        {
            result->data->Set(thisLen + i, seq->Get(i));
        }
        return result;
    }
};