#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"

template <typename T>
class MutableArraySequence : public Sequence<T>
{
private:
    DynamicArray<T> *data;

public:
    MutableArraySequence(T *items, int count) : data(new DynamicArray<T>(items, count)) {}
    MutableArraySequence(int size) : data(new DynamicArray<T>(size)) {}
    MutableArraySequence(const MutableArraySequence &other) : data(new DynamicArray<T>(*other.data)) {}
    ~MutableArraySequence() { delete data; }

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

    void Set(int index, T value) { data->Set(index, value); }
    void Resize(int newSize) { data->Resize(newSize); }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex)
            throw std::out_of_range("Неверные индексы подпоследовательности");
        int newSize = endIndex - startIndex + 1;
        MutableArraySequence<T> *sub = new MutableArraySequence<T>(newSize);
        for (int i = 0; i < newSize; ++i)
            sub->data->Set(i, data->Get(startIndex + i));
        return sub;
    }

    Sequence<T> *Append(T item) override
    {
        int oldSize = GetLength();
        data->Resize(oldSize + 1);
        data->Set(oldSize, item);
        return this;
    }

    Sequence<T> *Prepend(T item) override
    {
        int oldSize = GetLength();
        DynamicArray<T> *newData = new DynamicArray<T>(oldSize + 1);
        newData->Set(0, item);
        for (int i = 0; i < oldSize; ++i)
            newData->Set(i + 1, data->Get(i));
        delete data;
        data = newData;
        return this;
    }

    Sequence<T> *InsertAt(T item, int index) override
    {
        int len = GetLength();
        if (index < 0 || index > len)
            throw std::out_of_range("Неверный индекс вставки");
        DynamicArray<T> *newData = new DynamicArray<T>(len + 1);
        for (int i = 0; i < index; ++i)
            newData->Set(i, data->Get(i));
        newData->Set(index, item);
        for (int i = index; i < len; ++i)
            newData->Set(i + 1, data->Get(i));
        delete data;
        data = newData;
        return this;
    }

    Sequence<T> *Concat(Sequence<T> *list) const override
    {
        int thisLen = GetLength();
        int otherLen = list->GetLength();
        MutableArraySequence<T> *result = new MutableArraySequence<T>(thisLen + otherLen);
        for (int i = 0; i < thisLen; ++i)
            result->data->Set(i, data->Get(i));
        for (int i = 0; i < otherLen; ++i)
            result->data->Set(thisLen + i, list->Get(i));
        return result;
    }
};