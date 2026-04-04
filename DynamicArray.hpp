#pragma once

#include <cstdio>

template <class T>
class DynamicArray
{
private:
    T *data;
    int size;
    int capacity;

public:
    DynamicArray(T *items, int count);
    DynamicArray(int size) : size (size), capacity (size), data(new T[size]);
    DynamicArray(const DynamicArray<T> &other);
    T Get(int index) {
        return data[index];
    }
    int GetSize() {
        return size;
    }
    void Set(int index, T value) {
        data[index] = value;
    }
    void Resize(int newSize);
};

template <class T>
DynamicArray<T>::DynamicArray(T *items, int count) : size(count), data(new T[size]), capacity(size)
{
    for (int i = 0; i < count; i++)
    {
        data[i] = items[i];
    }
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other) : 
size(other.size), capacity(other.capacity), data(new T[capacity])
{
    for (int i = 0; i < size; i++)
    {
        data[i] = other.data[i];
    }
}

template <class T>
void DynamicArray<T>::Resize(int newSize) {
    T *newData = new T[newSize];
    if (newSize > this->size) {
        for (int i = 0; i < newSize; i++)
        {
            newData[i] = data[i];
        }
    }
    else if(newSize < this->size) {
        for (int i = 0; i < newSize; i++)
        {
            newData[i] = data[i];
        }
        this->size = newSize;
    }
    this->capacity = newSize;
    this->data = newdata;
}