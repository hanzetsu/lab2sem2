#pragma once
#include <iostream>
#include <string>
#include "Sequence.hpp"
#include "BitSequence.hpp"

#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "    Ошибка: " << msg << std::endl; \
            return false; \
        } \
    } while(0)

template <typename T>
void printSequence(Sequence<T>* seq, const std::string& title) {
    std::cout << title << " [";
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << seq->Get(i);
    }
    std::cout << "]" << std::endl;
}

inline void printBitSequence(Sequence<Bit>* seq, const std::string& title) {
    std::cout << title << " [";
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << (seq->Get(i) == Bit::one ? "1" : "0");
    }
    std::cout << "]" << std::endl;
}