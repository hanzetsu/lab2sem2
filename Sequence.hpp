#include "LinkedList.hpp"
#include "DynamicArray.hpp"

template <class T>
class Sequence
{
public:
    virtual ~Sequence() = default;
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual Sequence<T> *GetSubsequence(int startIndex, int endIndex) const = 0;
    virtual int GetLength() const = 0;
    virtual Sequence<T> *Append(T item) = 0;
    virtual Sequence<T> *Prepend(T item) = 0;
    virtual Sequence<T> *InsertAt(T item, int index) = 0;
    virtual Sequence<T> *Concat(Sequence<T> *list) const = 0;
};

template <typename T>
class ArraySequence : public Sequence<T>
{
private:
    DynamicArray<T> *data;

public:
    ArraySequence(T *items, int count) : data(new DynamicArray<T>(items, count)) {}
    ArraySequence(int size) : data(new DynamicArray<T>(size)) {}
    ArraySequence(const ArraySequence &other) : data(new DynamicArray<T>(*other.data)) {}
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
T ArraySequence<T>::GetFirst() const
{
    if (GetLength() == 0)
        throw std::out_of_range("Последовательность пуста");
    else
        return data->Get(0);
}
template <typename T>
T ArraySequence<T>::GetLast() const
{
    int length = GetLength();
    if (length == 0)
        throw std::out_of_range("Последовательность пуста");
    else
        return data->Get(length - 1);
}

template <typename T>
Sequence<T> *ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const
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
Sequence<T> *ArraySequence<T>::Append(T item)
{
    int size = data->GetSize();
    data->Resize(size + 1);
    data->Set(size, item);
    return this;
}

template <typename T>
Sequence<T> *ArraySequence<T>::Prepend(T item)
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
Sequence<T> *ArraySequence<T>::InsertAt(T item, int index)
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
Sequence<T> *ArraySequence<T>::Concat(Sequence<T> *list) const
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

template <typename T>
class ListSequence : public Sequence<T>
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