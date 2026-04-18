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

    T Get(int index) const
    {
        data->Get(index);
    }
    int GetLength() const
    {
        data->GetSize;
    }
    void Set(int index, T value)
    {
        data->Set(index, value);
    }
    void Resize(int newSize)
    {
        data->Resize(newSize);
    }
    ArraySequence<T> *GetSubsequence(int startIndex, int endIndex) const;
    ArraySequence<T> *Append(T item);
};

template <typename T>
ArraySequence<T> *ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    if (startIndex < 0 || endIndex >= data->GetSize() || startIndex > endIndex)
        throw std::out_of_range("Передаваемые параметры startIndex и/или endIndex - некорректны");
    else
    {
        int newSize = endIndex - startIndex;
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
ArraySequence<T> *ArraySequence<T>::Append(T item) {
    int size = data->GetSize;
    data->Set(size, item);
}