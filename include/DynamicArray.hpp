#pragma once

#include "exceptions.hpp"
#include <cstddef>

template <typename T>
class DynamicArray
{
private:
    T *data;
    std::size_t size;

public:
    DynamicArray(T *items, std::size_t count);
    DynamicArray(std::size_t size);
    DynamicArray() : data(nullptr), size(0) {}
    DynamicArray(const DynamicArray<T> &other);
    DynamicArray<T>& operator=(const DynamicArray<T>& other);
    ~DynamicArray() { delete[] data; }

    T Get(std::size_t index) const;
    std::size_t GetSize() const { return size; }
    T* GetRawData() const { return data; }
    void Set(std::size_t index, T value);
    void Resize(std::size_t newSize);
};

template <typename T>
DynamicArray<T>::DynamicArray(std::size_t size)
{
    this->size = size;
    data = new T[size];
}

template <typename T>
T DynamicArray<T>::Get(std::size_t index) const
{
    if (index >= size)
        throw IndexOutOfRange(index, size, "DynamicArray::Get");
    return data[index];
}

template <typename T>
void DynamicArray<T>::Set(std::size_t index, T value)
{
    if (index >= size)
        throw IndexOutOfRange(index, size, "DynamicArray::Set");
    data[index] = value;
}

template <typename T>
DynamicArray<T>::DynamicArray(T *items, std::size_t count)
{
    if (items == nullptr)
        throw InvalidArgument("DynamicArray::DynamicArray: нулевой указатель на элементы");
    size = count;
    data = new T[size];
    for (std::size_t i = 0; i < count; ++i)
        data[i] = items[i];
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other)
    : size(other.size), data(new T[size])
{
    for (std::size_t i = 0; i < size; ++i)
        data[i] = other.data[i];
}

template <typename T>
void DynamicArray<T>::Resize(std::size_t newSize)
{
    T *newData = new T[newSize];
    std::size_t elementsToCopy = (newSize < size) ? newSize : size;
    for (std::size_t i = 0; i < elementsToCopy; ++i)
        newData[i] = data[i];
    delete[] data;
    data = newData;
    size = newSize;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
{
    if (this != &other)
    {
        delete[] data;
        size = other.size;
        data = new T[size];
        for (std::size_t i = 0; i < size; ++i)
            data[i] = other.data[i];
    }
    return *this;
}