#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"

template <typename T>
class MutableArraySequence : public Sequence<T>
{
private:
    DynamicArray<T> data;

public:
    MutableArraySequence(T *items, std::size_t count) : data(items, count) {}
    MutableArraySequence(std::size_t size) : data(size) {}
    MutableArraySequence(const MutableArraySequence &other) = default;
    MutableArraySequence() : data(0) {}

    T GetFirst() const override
    {
        if (GetLength() == 0)
            throw std::out_of_range("Последовательность пуста");
        return data.Get(0);
    }

    T GetLast() const override
    {
        std::size_t len = GetLength();
        if (len == 0)
            throw std::out_of_range("Последовательность пуста");
        return data.Get(len - 1);
    }

    T Get(std::size_t index) const override
    {
        return data.Get(index);
    }

    std::size_t GetLength() const override
    {
        return data.GetSize();
    }

    void Set(std::size_t index, T value) { data.Set(index, value); }
    void Resize(std::size_t newSize) { data.Resize(newSize); }

    Sequence<T> *GetSubsequence(std::size_t startIndex, std::size_t endIndex) const override
    {
        if (startIndex > endIndex || endIndex >= GetLength())
            throw std::out_of_range("Неверные индексы подпоследовательности");
        std::size_t newSize = endIndex - startIndex + 1;
        MutableArraySequence<T> *sub = new MutableArraySequence<T>(newSize);
        for (std::size_t i = 0; i < newSize; ++i)
            sub->data.Set(i, data.Get(startIndex + i));
        return sub;
    }

    Sequence<T> *Append(T item) override
    {
        std::size_t oldSize = GetLength();
        data.Resize(oldSize + 1);
        data.Set(oldSize, item);
        return this;
    }

    Sequence<T> *Prepend(T item) override
    {
        std::size_t oldSize = GetLength();
        DynamicArray<T> newData(oldSize + 1);
        newData.Set(0, item);
        for (std::size_t i = 0; i < oldSize; ++i)
            newData.Set(i + 1, data.Get(i));
        data = newData;
        return this;
    }

    Sequence<T> *InsertAt(T item, std::size_t index) override
    {
        std::size_t len = GetLength();
        if (index > len)
            throw std::out_of_range("Неверный индекс вставки");
        DynamicArray<T> newData(len + 1);
        for (std::size_t i = 0; i < index; ++i)
            newData.Set(i, data.Get(i));
        newData.Set(index, item);
        for (std::size_t i = index; i < len; ++i)
            newData.Set(i + 1, data.Get(i));
        data = newData;
        return this;
    }

    Sequence<T> *Concat(Sequence<T> *list) const override
    {
        std::size_t thisLen = GetLength();
        std::size_t otherLen = list->GetLength();
        MutableArraySequence<T> *result = new MutableArraySequence<T>(thisLen + otherLen);
        for (std::size_t i = 0; i < thisLen; ++i)
            result->data.Set(i, data.Get(i));
        for (std::size_t i = 0; i < otherLen; ++i)
            result->data.Set(thisLen + i, list->Get(i));
        return result;
    }

    template <typename U, typename Func>
    MutableArraySequence<U> *map(Func f) const
    {
        std::size_t len = this->GetLength();
        MutableArraySequence<U> *result = new MutableArraySequence<U>(len);
        for (std::size_t i = 0; i < len; ++i)
        {
            result->Set(i, f(this->Get(i)));
        }
        return result;
    }

    template <typename U, typename Func>
    U reduce(Func f, U init) const
    {
        U acc = init;
        for (std::size_t i = 0; i < this->GetLength(); ++i)
        {
            acc = f(acc, this->Get(i));
        }
        return acc;
    }
};