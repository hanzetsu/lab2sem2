#pragma once
#include "LinkedList.hpp"
#include "Sequence.hpp"
#include "exceptions.hpp"

template <typename T>
class ImmutableListSequence : public Sequence<T>
{
private:
    LinkedList<T> list;

public:
    ImmutableListSequence(T *items, std::size_t count) : list(items, count) {}
    ImmutableListSequence() : list() {}
    ImmutableListSequence(const ImmutableListSequence &other) = default;
    explicit ImmutableListSequence(const LinkedList<T> &lst) : list(lst) {}

    T GetFirst() const override
    {
        if (GetLength() == 0)
            throw EmptyStructureException("ImmutableListSequence::GetFirst");
        return list.GetFirst();
    }

    T GetLast() const override
    {
        if (GetLength() == 0)
            throw EmptyStructureException("ImmutableListSequence::GetLast");
        return list.GetLast();
    }

    T Get(std::size_t index) const override
    {
        return list.Get(index);
    }

    std::size_t GetLength() const override
    {
        return list.GetLength();
    }

    Sequence<T> *GetSubsequence(std::size_t startIndex, std::size_t endIndex) const override
    {
        LinkedList<T> *subListPtr = list.GetSubList(startIndex, endIndex);
        ImmutableListSequence<T> *result = new ImmutableListSequence<T>(*subListPtr);
        delete subListPtr;
        return result;
    }

    Sequence<T> *Append(T item) override
    {
        ImmutableListSequence<T> *newSeq = new ImmutableListSequence<T>(*this);
        newSeq->list.Append(item);
        return newSeq;
    }

    Sequence<T> *Prepend(T item) override
    {
        ImmutableListSequence<T> *newSeq = new ImmutableListSequence<T>(*this);
        newSeq->list.Prepend(item);
        return newSeq;
    }

    Sequence<T> *InsertAt(T item, std::size_t index) override
    {
        ImmutableListSequence<T> *newSeq = new ImmutableListSequence<T>(*this);
        newSeq->list.InsertAt(item, index);
        return newSeq;
    }

    Sequence<T> *Concat(Sequence<T> *seq) const override
    {
        const ImmutableListSequence<T> *other = dynamic_cast<const ImmutableListSequence<T> *>(seq);
        if (!other)
            throw InvalidArgument("ImmutableListSequence::Concat: неверный тип последовательности");
        LinkedList<T> newList = list;
        for (auto it = other->list.begin(); it != other->list.end(); ++it)
            newList.Append(*it);
        return new ImmutableListSequence<T>(newList);
    }
};