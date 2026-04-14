#pragma once

#include <cstdio>
#include <stdexcept>

template <typename T>
class DynamicArray
{
private:
    T *data;
    int size;
    int capacity;

public:
    DynamicArray(T *items, int count);
    DynamicArray(int size)
    {
        if (size < 0)
            throw std::invalid_argument("Размер не может быть < 0");
        this->size = size;
        capacity = size;
        data = new T[size];
    };
    DynamicArray(const DynamicArray<T> &other);
    ~DynamicArray()
    {
        delete[] data;
    };
    T Get(int index) const
    {
        if (index >= 0 && index < size)
            return data[index];
        else
            throw std::out_of_range("Индекс вышел за границу");
    }
    int GetSize()
    {
        return size;
    }
    void Set(int index, T value) const
    {
        if (index >= 0 && index < size)
            data[index] = value;
        else
            throw std::out_of_range("Индекс вышел за границу");
    }
    void Resize(int newSize);
};

template <typename T>
DynamicArray<T>::DynamicArray(T *items, int count)
{
    if (size < 0)
        throw std::invalid_argument("Размер не может быть < 0");
    size = count;
    data = new T[size];
    capacity = size;
    for (int i = 0; i < count; i++)
    {
        data[i] = items[i];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other) : size(other.size), capacity(other.capacity), data(new T[capacity])
{
    for (int i = 0; i < size; i++)
    {
        data[i] = other.data[i];
    }
}

template <typename T>
void DynamicArray<T>::Resize(int newSize)
{
    if (newSize < 0)
    {
        throw std::invalid_argument("Размер не может быть < 0");
    }
    T *newData = new T[newSize];
    int elementsToCopy = (newSize < size) ? newSize : size;
    for (int i = 0; i < elementsToCopy; ++i)
    {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    size = newSize;
    capacity = newSize;
}