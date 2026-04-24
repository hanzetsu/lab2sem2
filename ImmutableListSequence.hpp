#pragma once
#include "LinkedList.hpp"
#include "Sequence.hpp"

template <typename T>
class ImmutableListSequence : public Sequence<T>
{
private:
    LinkedList<T> *list;

public:
    ImmutableListSequence(T *items, int count) : list(new LinkedList<T>(items, count)) {}
    ImmutableListSequence() : list(new LinkedList<T>()) {}
    ImmutableListSequence(const ImmutableListSequence &other) : list(new LinkedList<T>(*other.list)) {}
    ImmutableListSequence(LinkedList<T> *lst) : list(lst) {}

    ~ImmutableListSequence()
    {
        delete list;
    }

    T GetFirst() const override
    {
        if (GetLength() == 0)
            throw std::out_of_range("Последовательность пуста");
        return list->GetFirst();
    }

    T GetLast() const override
    {
        if (GetLength() == 0)
            throw std::out_of_range("Последовательность пуста");
        return list->GetLast();
    }

    T Get(int index) const override
    {
        return list->Get(index);
    }

    int GetLength() const override
    {
        return list->GetLength();
    }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        LinkedList<T> *subList = list->GetSubList(startIndex, endIndex);
        return new ImmutableListSequence<T>(subList);
    }

    Sequence<T> *Append(T item) override
    {
        ImmutableListSequence<T> *newSeq = new ImmutableListSequence<T>(*this);
        newSeq->list->Append(item);
        return newSeq;
    }

    Sequence<T> *Prepend(T item) override
    {
        ImmutableListSequence<T> *newSeq = new ImmutableListSequence<T>(*this);
        newSeq->list->Prepend(item);
        return newSeq;
    }

    Sequence<T> *InsertAt(T item, int index) override
    {
        ImmutableListSequence<T> *newSeq = new ImmutableListSequence<T>(*this);
        newSeq->list->InsertAt(item, index);
        return newSeq;
    }

    Sequence<T> *Concat(Sequence<T> *seq) const override
    {
        const ImmutableListSequence<T> *other = dynamic_cast<const ImmutableListSequence<T> *>(seq);
        if (!other)
            throw std::invalid_argument("Неверный тип последовательности для конкатенации");
        LinkedList<T> *newList = new LinkedList<T>(*list);
        newList->Concat(*other->list);
        return new ImmutableListSequence<T>(newList);
    }
};