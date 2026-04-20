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
    MutableArraySequence(const ArraySequence &other) : data(new DynamicArray<T>(*other.data)) {}
    ~ArraySequence() { delete data; }
    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override
    {
        return data->Get(index);
    }
    int GetLength() const override
    {
        return data->GetSize();
    }
    void Set(int index, T value) override
    {
        data->Set(index, value);
    }
    void Resize(int newSize) override
    {
        data->Resize(newSize);
    }
    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override;
    Sequence<T> *Append(T item) override;
    Sequence<T> *Prepend(T item) override;
    Sequence<T> *InsertAt(T item, int index) override;
    Sequence<T> *Concat(Sequence<T> *list) const override;
};
template <typename T>
T MutableArraySequence<T>::GetFirst() const
{
    if (GetLength() == 0)
        throw std::out_of_range("Последовательность пуста");
    else
        return data->Get(0);
}
template <typename T>
T MutableArraySequence<T>::GetLast() const
{
    int length = GetLength();
    if (length == 0)
        throw std::out_of_range("Последовательность пуста");
    else
        return data->Get(length - 1);
}

template <typename T>
Sequence<T> *MutableArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    if (startIndex < 0 || endIndex >= data->GetSize() || startIndex > endIndex)
        throw std::out_of_range("Передаваемые параметры startIndex и/или endIndex - некорректны");
    else
    {
        int newSize = endIndex - startIndex + 1;
        int tmp = startIndex;
        ArraySequence<T> *newSequence = new ArraySequence<T>(newSize);
        for (int i = 0; i < newSize; i++)
        {
            T value = data->Get(tmp);
            newSequence->data->Set(i, value);
            ++tmp;
        }
        return newSequence;
    }
}

template <typename T>
Sequence<T> *MutableArraySequence<T>::Append(T item)
{
    int size = data->GetSize();
    data->Resize(size + 1);
    data->Set(size, item);
    return this;
}

template <typename T>
Sequence<T> *MutableArraySequence<T>::Prepend(T item)
{
    int oldSize = data->GetSize();
    DynamicArray<T> *newData = new DynamicArray<T>(oldSize + 1);
    newData->Set(0, item);
    for (int i = 0; i < oldSize; ++i)
        newData->Set(i + 1, data->Get(i));
    delete data;
    data = newData;
    return this;
}
template <typename T>
Sequence<T> *MutableArraySequence<T>::InsertAt(T item, int index)
{
    int len = data->GetSize();
    if (index < 0 || index > len)
        throw std::out_of_range("Неверный индекс");
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
template <typename T>
Sequence<T> *MutableArraySequence<T>::Concat(Sequence<T> *list) const
{
    int thisLen = GetLength();
    int otherLen = list->GetLength();
    ArraySequence<T> *result = new ArraySequence<T>(thisLen + otherLen);
    for (int i = 0; i < thisLen; ++i)
        result->data->Set(i, data->Get(i));
    for (int i = 0; i < otherLen; ++i)
        result->data->Set(thisLen + i, list->Get(i));
    return result;
}