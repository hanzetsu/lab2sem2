#pragma once
#include "LinkedList.hpp"
#include "Sequence.hpp"

template <typename T>
class MutableListSequence : public Sequence<T>
{
private:
    LinkedList<T> *list;

public:
    MutableListSequence(T *items, int count) : list(new LinkedList<T>(items, count)) {}
    MutableListSequence() : list(new LinkedList<T>()) {}
    MutableListSequence(const MutableListSequence &other) : list(new LinkedList<T>(*other.list)) {}

    ~MutableListSequence()
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
        MutableListSequence<T> *result = new MutableListSequence<T>();
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
        const MutableListSequence<T> *other = dynamic_cast<const MutableListSequence<T> *>(seq);
        if (!other)
            throw std::invalid_argument("Неверный тип последовательности для конкатенации");
        MutableListSequence<T> *result = new MutableListSequence<T>(*this);
        result->list->Concat(*other->list);
        return result;
    }
template <typename U, typename Func>
MutableListSequence<U>* map(Func f) const {
    int len = this->GetLength();
    MutableListSequence<U>* result = new MutableListSequence<U>();
    for (int i = 0; i < len; ++i) {
        result->Append(f(this->Get(i)));
    }
    return result;
}

template <typename U, typename Func>
U reduce(Func f, U init) const {
    U acc = init;
    for (int i = 0; i < this->GetLength(); ++i) {
        acc = f(acc, this->Get(i));
    }
    return acc;
}
};