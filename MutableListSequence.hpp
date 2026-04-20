#pragma once
#include "LinkedList.hpp"
#include "Sequence.hpp"

template <typename T>
class MutableListSequence : public Sequence<T>
{
private:
    LinkedList<T> *list;

public:
    ListSequence(T *items, int count) : list(new LinkedList<T>(items, count)) {}
    ListSequence() : list(new LinkedList<T>()) {}
    ListSequence(const ListSequence &other) : list(new LinkedList<T>(*other.list)) {}

    ~ListSequence()
    {
        delete list;
    }

    T GetFirst() const override
    {
        return list->GetFirst();
    }

    T GetLast() const override
    {
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
        ListSequence<T> *result = new ListSequence<T>();
        delete result->list;
        result->list = subList;
        return result;
    }

    Sequence<T> *Append(T item) override
    {
        list->Append(item);
        return this;
    }

    Sequence<T> *Prepend(T item) override
    {
        list->Prepend(item);
        return this;
    }

    Sequence<T> *InsertAt(T item, int index) override
    {
        list->InsertAt(item, index);
        return this;
    }

    Sequence<T> *Concat(Sequence<T> *seq) const override
    {
        const ListSequence<T> *other = dynamic_cast<const ListSequence<T> *>(seq);
        if (!other)
            throw std::invalid_argument("Неверный тип последовательности");

        ListSequence<T> *result = new ListSequence<T>(*this);
        result->list->Concat(*other->list);
        return result;
    }
};