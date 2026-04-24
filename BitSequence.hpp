#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "Bit.hpp"
#include <cstdint>
#include <stdexcept>
#include <algorithm>

class BitSequence : public Sequence<Bit>
{
private:
    DynamicArray<uint8_t> *bytes;
    int bitLength;

    // Вспомогательные методы
    static int byteIndex(int bitIndex) noexcept { return bitIndex / 8; }
    static int bitOffset(int bitIndex) noexcept { return bitIndex % 8; }
    static uint8_t mask(int bitOffset) noexcept { return 1 << bitOffset; }

    // Конструктор, принимающий уже готовый DynamicArray<uint8_t> и длину (для внутреннего использования)
    BitSequence(DynamicArray<uint8_t>* arr, int length) : bytes(arr), bitLength(length) {}

public:
    // Конструкторы
    BitSequence() : bytes(new DynamicArray<uint8_t>(0)), bitLength(0) {}

    BitSequence(int size, Bit initialValue = Bit::zero)
        : bytes(new DynamicArray<uint8_t>((size + 7) / 8)), bitLength(size)
    {
        uint8_t fillByte = (initialValue == Bit::one) ? 0xFF : 0x00;
        for (int i = 0; i < bytes->GetSize(); ++i)
            bytes->Set(i, fillByte);
        // Если size не кратен 8, нужно обнулить лишние биты в последнем байте
        if (size % 8 != 0 && initialValue == Bit::one) {
            int lastIdx = bytes->GetSize() - 1;
            uint8_t lastByte = bytes->Get(lastIdx);
            int bitsToKeep = size % 8;
            uint8_t mask = (1 << bitsToKeep) - 1;
            lastByte &= mask;
            bytes->Set(lastIdx, lastByte);
        }
    }

    BitSequence(const Bit* bits, int count) : BitSequence(count)
    {
        for (int i = 0; i < count; ++i)
        {
            if (bits[i] == Bit::one)
            {
                int byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t byte = bytes->Get(byteIdx);
                byte |= mask(bitOff);
                bytes->Set(byteIdx, byte);
            }
        }
    }

    // Конструктор копирования (глубокое копирование)
    BitSequence(const BitSequence& other)
        : bytes(new DynamicArray<uint8_t>(*other.bytes)), bitLength(other.bitLength) {}

    // Оператор присваивания с глубоким копированием (защита от самоприсваивания)
    BitSequence& operator=(const BitSequence& other)
    {
        if (this != &other)
        {
            delete bytes;
            bytes = new DynamicArray<uint8_t>(*other.bytes);
            bitLength = other.bitLength;
        }
        return *this;
    }

    // Деструктор
    ~BitSequence() override
    {
        delete bytes;
    }

    // Методы доступа
    Bit GetFirst() const override
    {
        if (bitLength == 0)
            throw std::out_of_range("BitSequence is empty");
        return Get(0);
    }

    Bit GetLast() const override
    {
        if (bitLength == 0)
            throw std::out_of_range("BitSequence is empty");
        return Get(bitLength - 1);
    }

    Bit Get(int index) const override
    {
        if (index < 0 || index >= bitLength)
            throw std::out_of_range("Index out of range");
        int byteIdx = byteIndex(index);
        int bitOff = bitOffset(index);
        uint8_t byte = bytes->Get(byteIdx);
        return (byte & mask(bitOff)) ? Bit::one : Bit::zero;
    }

    int GetLength() const override
    {
        return bitLength;
    }

    // Immutable операции – создают новый объект, не изменяя текущий
    Sequence<Bit>* GetSubsequence(int startIndex, int endIndex) const override
    {
        // endIndex – исключительный (как в стандартной практике C++)
        if (startIndex < 0 || endIndex > bitLength || startIndex > endIndex)
            throw std::out_of_range("Invalid subsequence indices");
        int newSize = endIndex - startIndex;
        BitSequence* sub = new BitSequence(newSize);
        for (int i = 0; i < newSize; ++i)
        {
            Bit b = Get(startIndex + i);
            if (b == Bit::one)
            {
                int byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t byte = sub->bytes->Get(byteIdx);
                byte |= mask(bitOff);
                sub->bytes->Set(byteIdx, byte);
            }
        }
        return sub;
    }

    Sequence<Bit>* Append(Bit item) override
    {
        BitSequence* result = new BitSequence(*this);
        int newBitIndex = result->bitLength;
        int byteIdx = byteIndex(newBitIndex);
        int bitOff = bitOffset(newBitIndex);
        if (bitOff == 0)
        {
            int oldSize = result->bytes->GetSize();
            result->bytes->Resize(oldSize + 1);
            result->bytes->Set(oldSize, 0);
        }
        uint8_t byte = result->bytes->Get(byteIdx);
        if (item == Bit::one)
            byte |= mask(bitOff);
        result->bytes->Set(byteIdx, byte);
        result->bitLength++;
        return result;
    }

    Sequence<Bit>* Prepend(Bit item) override
    {
        BitSequence* result = new BitSequence(bitLength + 1);
        // Установить первый бит
        if (item == Bit::one)
        {
            uint8_t byte = result->bytes->Get(0);
            byte |= mask(0);
            result->bytes->Set(0, byte);
        }
        // Скопировать старые биты со сдвигом вправо
        for (int i = 0; i < bitLength; ++i)
        {
            Bit b = Get(i);
            if (b == Bit::one)
            {
                int newIdx = i + 1;
                int byteIdx = byteIndex(newIdx);
                int bitOff = bitOffset(newIdx);
                uint8_t byte = result->bytes->Get(byteIdx);
                byte |= mask(bitOff);
                result->bytes->Set(byteIdx, byte);
            }
        }
        return result;
    }

    Sequence<Bit>* InsertAt(Bit item, int index) override
    {
        if (index < 0 || index > bitLength)
            throw std::out_of_range("Invalid index for InsertAt");

        BitSequence* result = new BitSequence(bitLength + 1);
        // Копируем биты до index
        for (int i = 0; i < index; ++i)
        {
            if (Get(i) == Bit::one)
            {
                int byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t byte = result->bytes->Get(byteIdx);
                byte |= mask(bitOff);
                result->bytes->Set(byteIdx, byte);
            }
        }
        // Вставляем item
        if (item == Bit::one)
        {
            int byteIdx = byteIndex(index);
            int bitOff = bitOffset(index);
            uint8_t byte = result->bytes->Get(byteIdx);
            byte |= mask(bitOff);
            result->bytes->Set(byteIdx, byte);
        }
        // Копируем биты после index
        for (int i = index; i < bitLength; ++i)
        {
            if (Get(i) == Bit::one)
            {
                int newIdx = i + 1;
                int byteIdx = byteIndex(newIdx);
                int bitOff = bitOffset(newIdx);
                uint8_t byte = result->bytes->Get(byteIdx);
                byte |= mask(bitOff);
                result->bytes->Set(byteIdx, byte);
            }
        }
        return result;
    }

    Sequence<Bit>* Concat(Sequence<Bit>* list) const override
    {
        if (!list)
            throw std::invalid_argument("Concat: null pointer");

        int otherLen = list->GetLength();
        BitSequence* result = new BitSequence(bitLength + otherLen);
        // Копируем текущие биты
        for (int i = 0; i < bitLength; ++i)
        {
            if (Get(i) == Bit::one)
            {
                int byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t byte = result->bytes->Get(byteIdx);
                byte |= mask(bitOff);
                result->bytes->Set(byteIdx, byte);
            }
        }
        // Копируем биты из list
        for (int i = 0; i < otherLen; ++i)
        {
            if (list->Get(i) == Bit::one)
            {
                int idx = bitLength + i;
                int byteIdx = byteIndex(idx);
                int bitOff = bitOffset(idx);
                uint8_t byte = result->bytes->Get(byteIdx);
                byte |= mask(bitOff);
                result->bytes->Set(byteIdx, byte);
            }
        }
        return result;
    }

    // Побитовые операции (immutable, возвращают новый объект)
    BitSequence* And(const BitSequence& other) const
    {
        if (bitLength != other.bitLength)
            throw std::invalid_argument("AND: sequences must have same length");
        BitSequence* result = new BitSequence(bitLength);
        int numBytes = bytes->GetSize();
        for (int i = 0; i < numBytes; ++i)
        {
            uint8_t b1 = bytes->Get(i);
            uint8_t b2 = other.bytes->Get(i);
            result->bytes->Set(i, b1 & b2);
        }
        // Обнулить лишние биты в последнем байте, если нужно
        if (bitLength % 8 != 0)
        {
            int lastIdx = numBytes - 1;
            uint8_t lastByte = result->bytes->Get(lastIdx);
            int bitsToKeep = bitLength % 8;
            uint8_t maskLast = (1 << bitsToKeep) - 1;
            lastByte &= maskLast;
            result->bytes->Set(lastIdx, lastByte);
        }
        return result;
    }

    BitSequence* Or(const BitSequence& other) const
    {
        if (bitLength != other.bitLength)
            throw std::invalid_argument("OR: sequences must have same length");
        BitSequence* result = new BitSequence(bitLength);
        int numBytes = bytes->GetSize();
        for (int i = 0; i < numBytes; ++i)
        {
            uint8_t b1 = bytes->Get(i);
            uint8_t b2 = other.bytes->Get(i);
            result->bytes->Set(i, b1 | b2);
        }
        if (bitLength % 8 != 0)
        {
            int lastIdx = numBytes - 1;
            uint8_t lastByte = result->bytes->Get(lastIdx);
            int bitsToKeep = bitLength % 8;
            uint8_t maskLast = (1 << bitsToKeep) - 1;
            lastByte &= maskLast;
            result->bytes->Set(lastIdx, lastByte);
        }
        return result;
    }

    BitSequence* Xor(const BitSequence& other) const
    {
        if (bitLength != other.bitLength)
            throw std::invalid_argument("XOR: sequences must have same length");
        BitSequence* result = new BitSequence(bitLength);
        int numBytes = bytes->GetSize();
        for (int i = 0; i < numBytes; ++i)
        {
            uint8_t b1 = bytes->Get(i);
            uint8_t b2 = other.bytes->Get(i);
            result->bytes->Set(i, b1 ^ b2);
        }
        if (bitLength % 8 != 0)
        {
            int lastIdx = numBytes - 1;
            uint8_t lastByte = result->bytes->Get(lastIdx);
            int bitsToKeep = bitLength % 8;
            uint8_t maskLast = (1 << bitsToKeep) - 1;
            lastByte &= maskLast;
            result->bytes->Set(lastIdx, lastByte);
        }
        return result;
    }

    BitSequence* Not() const
    {
        BitSequence* result = new BitSequence(bitLength);
        int numBytes = bytes->GetSize();
        for (int i = 0; i < numBytes; ++i)
        {
            uint8_t b = bytes->Get(i);
            result->bytes->Set(i, ~b);
        }
        if (bitLength % 8 != 0)
        {
            int lastIdx = numBytes - 1;
            uint8_t lastByte = result->bytes->Get(lastIdx);
            int bitsToKeep = bitLength % 8;
            uint8_t maskLast = (1 << bitsToKeep) - 1;
            lastByte &= maskLast;
            result->bytes->Set(lastIdx, lastByte);
        }
        return result;
    }
};