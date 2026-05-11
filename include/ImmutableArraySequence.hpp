#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "exceptions.hpp"

template <typename T>
class ImmutableArraySequence : public Sequence<T>
{
private:
    DynamicArray<T> data;

public:
    ImmutableArraySequence(T *items, std::size_t count) : data(items, count) {}
    ImmutableArraySequence(std::size_t size) : data(size) {}
    ImmutableArraySequence(const ImmutableArraySequence &other) = default;
    ImmutableArraySequence() : data(0) {}

    T GetFirst() const override
    {
        if (GetLength() == 0)
            throw EmptyStructureException("ImmutableArraySequence::GetFirst");
        return data.Get(0);
    }

    T GetLast() const override
    {
        std::size_t len = GetLength();
        if (len == 0)
            throw EmptyStructureException("ImmutableArraySequence::GetLast");
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

    Sequence<T> *GetSubsequence(std::size_t startIndex, std::size_t endIndex) const override
    {
        std::size_t len = GetLength();
        if (startIndex > endIndex || endIndex >= len)
            throw IndexOutOfRange(endIndex, len, "ImmutableArraySequence::GetSubsequence");
        std::size_t newSize = endIndex - startIndex + 1;
        ImmutableArraySequence<T> *sub = new ImmutableArraySequence<T>(newSize);
        for (std::size_t i = 0; i < newSize; ++i)
            sub->data.Set(i, data.Get(startIndex + i));
        return sub;
    }

    Sequence<T> *Append(T item) override
    {
        std::size_t oldSize = GetLength();
        ImmutableArraySequence<T> *newSeq = new ImmutableArraySequence<T>(oldSize + 1);
        for (std::size_t i = 0; i < oldSize; ++i)
            newSeq->data.Set(i, data.Get(i));
        newSeq->data.Set(oldSize, item);
        return newSeq;
    }

    Sequence<T> *Prepend(T item) override
    {
        std::size_t oldSize = GetLength();
        ImmutableArraySequence<T> *newSeq = new ImmutableArraySequence<T>(oldSize + 1);
        newSeq->data.Set(0, item);
        for (std::size_t i = 0; i < oldSize; ++i)
            newSeq->data.Set(i + 1, data.Get(i));
        return newSeq;
    }

    Sequence<T> *InsertAt(T item, std::size_t index) override
    {
        std::size_t oldSize = GetLength();
        if (index > oldSize)
            throw IndexOutOfRange(index, oldSize + 1, "ImmutableArraySequence::InsertAt");
        ImmutableArraySequence<T> *newSeq = new ImmutableArraySequence<T>(oldSize + 1);
        for (std::size_t i = 0; i < index; ++i)
            newSeq->data.Set(i, data.Get(i));
        newSeq->data.Set(index, item);
        for (std::size_t i = index; i < oldSize; ++i)
            newSeq->data.Set(i + 1, data.Get(i));
        return newSeq;
    }

    Sequence<T> *Concat(Sequence<T> *seq) const override
    {
        std::size_t thisLen = GetLength();
        std::size_t otherLen = seq->GetLength();
        ImmutableArraySequence<T> *result = new ImmutableArraySequence<T>(thisLen + otherLen);
        for (std::size_t i = 0; i < thisLen; ++i)
            result->data.Set(i, data.Get(i));
        for (std::size_t i = 0; i < otherLen; ++i)
            result->data.Set(thisLen + i, seq->Get(i));
        return result;
    }
};