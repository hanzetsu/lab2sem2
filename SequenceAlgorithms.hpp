#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include <stdexcept>
#include <limits>   // добавлено

struct Stats
{
    int min;
    int max;
    double sum;
    int count;
};

template <typename T>
Stats computeStats(const Sequence<T> &seq)
{
    int n = seq.GetLength();
    if (n == 0) {
        // Пустая последовательность – возвращаем нулевые значения (можно выбросить исключение по желанию)
        return {0, 0, 0.0, 0};
    }
    Stats res = {seq.Get(0), seq.Get(0), 0.0, 0};
    for (int i = 0; i < n; ++i) {
        T val = seq.Get(i);
        if (val < res.min) res.min = val;
        if (val > res.max) res.max = val;
        res.sum += val;
        res.count++;
    }
    return res;
}

// Более эффективная быстрая сортировка (quick sort)
template <typename T>
void quickSort(DynamicArray<T> &arr, int left, int right)
{
    if (left >= right)
        return;
    T pivot = arr.Get((left + right) / 2);
    int i = left, j = right;
    while (i <= j)
    {
        while (arr.Get(i) < pivot)
            ++i;
        while (arr.Get(j) > pivot)
            --j;
        if (i <= j)
        {
            T tmp = arr.Get(i);
            arr.Set(i, arr.Get(j));
            arr.Set(j, tmp);
            ++i;
            --j;
        }
    }
    quickSort(arr, left, j);
    quickSort(arr, i, right);
}

template <typename T>
void sortDynamicArray(DynamicArray<T> &arr)
{
    quickSort(arr, 0, arr.GetSize() - 1);
}

// Функция для вычисления медианы последовательности
template <typename T>
double median(const Sequence<T> &seq)
{
    int n = seq.GetLength();
    if (n == 0)
        throw std::runtime_error("Пустая последовательность: невозможно вычислить медиану");

    // Копируем элементы в DynamicArray
    DynamicArray<T> copy(n);
    for (int i = 0; i < n; ++i)
    {
        copy.Set(i, seq.Get(i));
    }

    // Сортируем копию
    sortDynamicArray(copy);

    if (n % 2 == 1)
    {
        return static_cast<double>(copy.Get(n / 2));
    }
    else
    {
        return (static_cast<double>(copy.Get(n / 2 - 1)) + static_cast<double>(copy.Get(n / 2))) / 2.0;
    }
}