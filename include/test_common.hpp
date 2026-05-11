#pragma once

#include <iostream>
#include <string>
#include "Sequence.hpp"
#include <BitSequence.hpp>


#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "    Ошибка: " << msg << std::endl; \
            return false; \
        } \
    } while(0)

#define TEST_THROWS(expr, exceptionType, expectedMsgContains) \
    do { \
        bool caught = false; \
        try { expr; } \
        catch (const exceptionType& e) { \
            std::string w = e.what(); \
            if (w.find(expectedMsgContains) == std::string::npos) { \
                std::cerr << "    Ошибка: исключение выброшено, но сообщение не содержит '" << expectedMsgContains << "'" << std::endl; \
                return false; \
            } \
            caught = true; \
        } \
        catch (...) { \
            std::cerr << "    Ошибка: выброшено исключение неверного типа" << std::endl; \
            return false; \
        } \
        if (!caught) { \
            std::cerr << "    Ошибка: ожидалось исключение " << #exceptionType << ", но не выброшено" << std::endl; \
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
void printBitSequence(Sequence<Bit>* seq, const std::string& title) {
    std::cout << title << " [";
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << (seq->Get(i) == Bit::one ? "1" : "0");
    }
    std::cout << "]" << std::endl;
}