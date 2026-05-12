#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <ranges>
#include "Sequence.hpp"
#include "BitSequence.hpp"
#include "DynamicArray.hpp"

#define TEST_ASSERT(cond, msg)                           \
    for (bool _ok = true; _ok && !(cond); _ok = false)   \
    {                                                    \
        std::cerr << "    Ошибка: " << msg << std::endl; \
        return false;                                    \
    }

template <typename T>
void printSequence(Sequence<T> *seq, const std::string &title)
{
    std::cout << title << " [";
    for (size_t i = 0; i < seq->GetLength(); ++i)
    {
        if (i > 0)
            std::cout << ", ";
        std::cout << seq->Get(i);
    }
    std::cout << "]" << std::endl;
}

inline void printBitSequence(Sequence<Bit> *seq, const std::string &title)
{
    std::cout << title << " [";
    for (size_t i = 0; i < seq->GetLength(); ++i)
    {
        if (i > 0)
            std::cout << ", ";
        std::cout << (seq->Get(i) == Bit::one ? "1" : "0");
    }
    std::cout << "]" << std::endl;
}