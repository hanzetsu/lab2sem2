#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "Bit.hpp"
#include <cstdint>
#include <stdexcept>

class BitSequence : public Sequence<Bit>
{
private:
    DynamicArray<uint8_t> *bytes;
    int bitLength;

    static int byteIndex(int bitIndex) noexcept { return bitIndex / 8; }    
    static int bitOffset(int bitIndex) noexcept { return bitIndex % 8; }
    static uint8_t mask(int offset) noexcept { return 1 << offset; }

    // Внутренний конструктор для создания из существующего массива байт
    BitSequence(DynamicArray<uint8_t> *arr, int length) : bytes(arr), bitLength(length) {}

public:
    BitSequence() : bytes(new DynamicArray<uint8_t>(0)), bitLength(0) {}

    BitSequence(int size, Bit initialValue = Bit::zero)
        : bytes(new DynamicArray<uint8_t>((size + 7) / 8)), bitLength(size)
    {
        uint8_t fill = (initialValue == Bit::one) ? 0xFF : 0x00;
        for (int i = 0; i < bytes->GetSize(); ++i)
            bytes->Set(i, fill);
        if (size % 8 != 0 && initialValue == Bit::one)
        {
            int lastIdx = bytes->GetSize() - 1;
            uint8_t lastByte = bytes->Get(lastIdx);
            lastByte &= (1 << (size % 8)) - 1;
            bytes->Set(lastIdx, lastByte);
        }
    }

    BitSequence(const Bit *bits, int count) : BitSequence(count)
    {
        for (int i = 0; i < count; ++i)
            if (bits[i] == Bit::one)
            {
                int idx = byteIndex(i), off = bitOffset(i);
                uint8_t b = bytes->Get(idx);
                b |= mask(off);
                bytes->Set(idx, b);
            }
    }

    BitSequence(const BitSequence &other)
        : bytes(new DynamicArray<uint8_t>(*other.bytes)), bitLength(other.bitLength) {}

    BitSequence &operator=(const BitSequence &other)
    {
        if (this != &other)
        {
            delete bytes;
            bytes = new DynamicArray<uint8_t>(*other.bytes);
            bitLength = other.bitLength;
        }
        return *this;
    }

    ~BitSequence() override { delete bytes; }

    Bit GetFirst() const override
    {
        if (bitLength == 0)
            throw std::out_of_range("Последовательность битов пуста");
        return Get(0);
    }

    Bit GetLast() const override
    {
        if (bitLength == 0)
            throw std::out_of_range("Последовательность битов пуста");
        return Get(bitLength - 1);
    }

    Bit Get(int index) const override
    {
        if (index < 0 || index >= bitLength)
            throw std::out_of_range("Индекс вне диапазона");
        int byteIdx = byteIndex(index);
        int bitOff = bitOffset(index);
        uint8_t byte = bytes->Get(byteIdx);
        return (byte & mask(bitOff)) ? Bit::one : Bit::zero;
    }

    int GetLength() const override { return bitLength; }

    Sequence<Bit> *GetSubsequence(int startIndex, int endIndex) const override
    {
        if (startIndex < 0 || endIndex >= bitLength || startIndex > endIndex)
            throw std::out_of_range("Неверные индексы подпоследовательности");
        int newSize = endIndex - startIndex + 1;
        BitSequence *sub = new BitSequence(newSize);
        for (int i = 0; i < newSize; ++i)
        {
            if (Get(startIndex + i) == Bit::one)
            {
                int byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t b = sub->bytes->Get(byteIdx);
                b |= mask(bitOff);
                sub->bytes->Set(byteIdx, b);
            }
        }
        return sub;
    }

    // Immutable методы (не меняют текущий объект)
    Sequence<Bit> *Append(Bit item) override
    {
        BitSequence *newSeq = new BitSequence(*this);
        int idx = newSeq->bitLength;
        int byteIdx = byteIndex(idx);
        int bitOff = bitOffset(idx);
        if (bitOff == 0)
        {
            int oldSize = newSeq->bytes->GetSize();
            newSeq->bytes->Resize(oldSize + 1);
            newSeq->bytes->Set(oldSize, 0);
        }
        uint8_t b = newSeq->bytes->Get(byteIdx);
        if (item == Bit::one)
            b |= mask(bitOff);
        newSeq->bytes->Set(byteIdx, b);
        newSeq->bitLength++;
        return newSeq;
    }

    Sequence<Bit> *Prepend(Bit item) override
    {
        BitSequence *newSeq = new BitSequence(bitLength + 1);
        // Устанавливаем первый бит
        if (item == Bit::one)
        {
            uint8_t b = newSeq->bytes->Get(0);
            b |= mask(0);
            newSeq->bytes->Set(0, b);
        }
        // Копируем старые биты со сдвигом
        for (int i = 0; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                int newIdx = i + 1;
                int byteIdx = byteIndex(newIdx);
                int bitOff = bitOffset(newIdx);
                uint8_t b = newSeq->bytes->Get(byteIdx);
                b |= mask(bitOff);
                newSeq->bytes->Set(byteIdx, b);
            }
        return newSeq;
    }

    Sequence<Bit> *InsertAt(Bit item, int index) override
    {
        if (index < 0 || index > bitLength)
            throw std::out_of_range("Неверный индекс вставки");
        BitSequence *newSeq = new BitSequence(bitLength + 1);
        // Копируем биты до index
        for (int i = 0; i < index; ++i)
            if (Get(i) == Bit::one)
            {
                int byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t b = newSeq->bytes->Get(byteIdx);
                b |= mask(bitOff);
                newSeq->bytes->Set(byteIdx, b);
            }
        // Вставляем item
        if (item == Bit::one)
        {
            int byteIdx = byteIndex(index);
            int bitOff = bitOffset(index);
            uint8_t b = newSeq->bytes->Get(byteIdx);
            b |= mask(bitOff);
            newSeq->bytes->Set(byteIdx, b);
        }
        // Копируем биты после index
        for (int i = index; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                int newIdx = i + 1;
                int byteIdx = byteIndex(newIdx);
                int bitOff = bitOffset(newIdx);
                uint8_t b = newSeq->bytes->Get(byteIdx);
                b |= mask(bitOff);
                newSeq->bytes->Set(byteIdx, b);
            }
        return newSeq;
    }

    Sequence<Bit> *Concat(Sequence<Bit> *seq) const override
    {
        if (!seq)
            throw std::invalid_argument("Пустой указатель на последовательность");
        int otherLen = seq->GetLength();
        BitSequence *result = new BitSequence(bitLength + otherLen);
        // Копируем текущие биты
        for (int i = 0; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                int byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t b = result->bytes->Get(byteIdx);
                b |= mask(bitOff);
                result->bytes->Set(byteIdx, b);
            }
        // Копируем биты из seq
        for (int i = 0; i < otherLen; ++i)
            if (seq->Get(i) == Bit::one)
            {
                int idx = bitLength + i;
                int byteIdx = byteIndex(idx);
                int bitOff = bitOffset(idx);
                uint8_t b = result->bytes->Get(byteIdx);
                b |= mask(bitOff);
                result->bytes->Set(byteIdx, b);
            }
        return result;
    }

    // Побитовые операции
    BitSequence *And(const BitSequence &other) const
    {
        if (bitLength != other.bitLength)
            throw std::invalid_argument("Длины последовательностей не совпадают");
        BitSequence *res = new BitSequence(bitLength);
        int numBytes = bytes->GetSize();
        for (int i = 0; i < numBytes; ++i)
            res->bytes->Set(i, bytes->Get(i) & other.bytes->Get(i));
        if (bitLength % 8 != 0)
        {
            int lastIdx = numBytes - 1;
            uint8_t lastByte = res->bytes->Get(lastIdx);
            lastByte &= (1 << (bitLength % 8)) - 1;
            res->bytes->Set(lastIdx, lastByte);
        }
        return res;
    }

    BitSequence *Or(const BitSequence &other) const
    {
        if (bitLength != other.bitLength)
            throw std::invalid_argument("Длины последовательностей не совпадают");
        BitSequence *res = new BitSequence(bitLength);
        int numBytes = bytes->GetSize();
        for (int i = 0; i < numBytes; ++i)
            res->bytes->Set(i, bytes->Get(i) | other.bytes->Get(i));
        if (bitLength % 8 != 0)
        {
            int lastIdx = numBytes - 1;
            uint8_t lastByte = res->bytes->Get(lastIdx);
            lastByte &= (1 << (bitLength % 8)) - 1;
            res->bytes->Set(lastIdx, lastByte);
        }
        return res;
    }

    BitSequence *Xor(const BitSequence &other) const
    {
        if (bitLength != other.bitLength)
            throw std::invalid_argument("Длины последовательностей не совпадают");
        BitSequence *res = new BitSequence(bitLength);
        int numBytes = bytes->GetSize();
        for (int i = 0; i < numBytes; ++i)
            res->bytes->Set(i, bytes->Get(i) ^ other.bytes->Get(i));
        if (bitLength % 8 != 0)
        {
            int lastIdx = numBytes - 1;
            uint8_t lastByte = res->bytes->Get(lastIdx);
            lastByte &= (1 << (bitLength % 8)) - 1;
            res->bytes->Set(lastIdx, lastByte);
        }
        return res;
    }

    BitSequence *Not() const
    {
        BitSequence *res = new BitSequence(bitLength);
        int numBytes = bytes->GetSize();
        for (int i = 0; i < numBytes; ++i)
            res->bytes->Set(i, ~bytes->Get(i));
        if (bitLength % 8 != 0)
        {
            int lastIdx = numBytes - 1;
            uint8_t lastByte = res->bytes->Get(lastIdx);
            lastByte &= (1 << (bitLength % 8)) - 1;
            res->bytes->Set(lastIdx, lastByte);
        }
        return res;
    }
};