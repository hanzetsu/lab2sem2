#include "LinkedList.hpp"
#include "Sequence.hpp"

template <typename T>
class ImmutableListSequence : public Sequence<T>
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

    // Методы
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

    Sequence<T> *Append(T item) override
    {
        ListSequence<T> *newSequence = new ListSequence<T>(*this);
        newSequence->list->Append(item);
        return newSequence;
    }

    Sequence<T> *Prepend(T item) override
    {
        ListSequence<T> *newSequence = new ListSequence<T>(*this);
        newSequence->list->Prepend(item);
        return newSequence;
    }

    Sequence<T> *InsertAt(T item, int index) override
    {
        ListSequence<T> *newSequence = new ListSequence<T>(*this);
        newSequence->list->InsertAt(item, index);
        return newSequence;
    }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        LinkedList<T> *subList = list->GetSubList(startIndex, endIndex);
        ListSequence<T> *result = new ListSequence<T>();
        result->list = subList;
        return result;
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