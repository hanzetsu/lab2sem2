#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "exceptions.hpp"
#include <cstdint>
#include <cstddef>

enum class Bit {
    zero,
    one
};

class BitSequence : public Sequence<Bit>
{
private:
    DynamicArray<uint8_t> bytes;
    std::size_t bitLength;

    static std::size_t byteIndex(std::size_t bitIndex) noexcept { return bitIndex / 8; }
    static int bitOffset(std::size_t bitIndex) noexcept { return static_cast<int>(bitIndex % 8); }
    static uint8_t mask(int offset) noexcept { return 1 << offset; }

public:
    BitSequence() : bytes(0), bitLength(0) {}

    BitSequence(std::size_t size, Bit initialValue = Bit::zero)
        : bytes((size + 7) / 8), bitLength(size)
    {
        uint8_t fill = (initialValue == Bit::one) ? 0xFF : 0x00;
        for (std::size_t i = 0; i < bytes.GetSize(); ++i)
            bytes.Set(i, fill);
        if (size % 8 != 0 && initialValue == Bit::one)
        {
            std::size_t lastIdx = bytes.GetSize() - 1;
            uint8_t lastByte = bytes.Get(lastIdx);
            lastByte &= (1 << (size % 8)) - 1;
            bytes.Set(lastIdx, lastByte);
        }
    }

    BitSequence(const Bit *bits, std::size_t count) : BitSequence(count)
    {
        for (std::size_t i = 0; i < count; ++i)
            if (bits[i] == Bit::one)
            {
                std::size_t idx = byteIndex(i);
                int off = bitOffset(i);
                uint8_t b = bytes.Get(idx);
                b |= mask(off);
                bytes.Set(idx, b);
            }
    }

    Bit GetFirst() const override
    {
        if (bitLength == 0)
            throw EmptyStructureException("BitSequence::GetFirst");
        return Get(0);
    }

    Bit GetLast() const override
    {
        if (bitLength == 0)
            throw EmptyStructureException("BitSequence::GetLast");
        return Get(bitLength - 1);
    }

    Bit Get(std::size_t index) const override
    {
        if (index >= bitLength)
            throw IndexOutOfRange(index, bitLength, "BitSequence::Get");
        std::size_t byteIdx = byteIndex(index);
        int bitOff = bitOffset(index);
        uint8_t byte = bytes.Get(byteIdx);
        return (byte & mask(bitOff)) ? Bit::one : Bit::zero;
    }

    std::size_t GetLength() const override { return bitLength; }

    Sequence<Bit> *GetSubsequence(std::size_t startIndex, std::size_t endIndex) const override
    {
        if (startIndex > endIndex || endIndex >= bitLength)
            throw IndexOutOfRange(endIndex, bitLength, "BitSequence::GetSubsequence");
        std::size_t newSize = endIndex - startIndex + 1;
        BitSequence *sub = new BitSequence(newSize);
        for (std::size_t i = 0; i < newSize; ++i)
        {
            if (Get(startIndex + i) == Bit::one)
            {
                std::size_t byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t b = sub->bytes.Get(byteIdx);
                b |= mask(bitOff);
                sub->bytes.Set(byteIdx, b);
            }
        }
        return sub;
    }

    Sequence<Bit> *Append(Bit item) override
    {
        std::size_t idx = bitLength;
        std::size_t byteIdx = byteIndex(idx);
        int bitOff = bitOffset(idx);
        if (bitOff == 0)
        {
            bytes.Resize(bytes.GetSize() + 1);
            bytes.Set(bytes.GetSize() - 1, 0);
        }
        uint8_t b = bytes.Get(byteIdx);
        if (item == Bit::one)
            b |= mask(bitOff);
        bytes.Set(byteIdx, b);
        bitLength++;
        return this;
    }

    Sequence<Bit> *Prepend(Bit item) override
    {
        BitSequence *newSeq = new BitSequence(bitLength + 1);
        if (item == Bit::one)
        {
            uint8_t b = newSeq->bytes.Get(0);
            b |= mask(0);
            newSeq->bytes.Set(0, b);
        }
        for (std::size_t i = 0; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                std::size_t newIdx = i + 1;
                std::size_t byteIdx = byteIndex(newIdx);
                int bitOff = bitOffset(newIdx);
                uint8_t b = newSeq->bytes.Get(byteIdx);
                b |= mask(bitOff);
                newSeq->bytes.Set(byteIdx, b);
            }
        return newSeq;
    }

    Sequence<Bit> *InsertAt(Bit item, std::size_t index) override
    {
        if (index > bitLength)
            throw IndexOutOfRange(index, bitLength + 1, "BitSequence::InsertAt");
        BitSequence *newSeq = new BitSequence(bitLength + 1);
        for (std::size_t i = 0; i < index; ++i)
            if (Get(i) == Bit::one)
            {
                std::size_t byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t b = newSeq->bytes.Get(byteIdx);
                b |= mask(bitOff);
                newSeq->bytes.Set(byteIdx, b);
            }
        if (item == Bit::one)
        {
            std::size_t byteIdx = byteIndex(index);
            int bitOff = bitOffset(index);
            uint8_t b = newSeq->bytes.Get(byteIdx);
            b |= mask(bitOff);
            newSeq->bytes.Set(byteIdx, b);
        }
        for (std::size_t i = index; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                std::size_t newIdx = i + 1;
                std::size_t byteIdx = byteIndex(newIdx);
                int bitOff = bitOffset(newIdx);
                uint8_t b = newSeq->bytes.Get(byteIdx);
                b |= mask(bitOff);
                newSeq->bytes.Set(byteIdx, b);
            }
        return newSeq;
    }

    Sequence<Bit> *Concat(Sequence<Bit> *seq) const override
    {
        if (!seq)
            throw InvalidArgument("BitSequence::Concat: пустой указатель на последовательность");
        std::size_t otherLen = seq->GetLength();
        BitSequence *result = new BitSequence(bitLength + otherLen);
        for (std::size_t i = 0; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                std::size_t byteIdx = byteIndex(i);
                int bitOff = bitOffset(i);
                uint8_t b = result->bytes.Get(byteIdx);
                b |= mask(bitOff);
                result->bytes.Set(byteIdx, b);
            }
        for (std::size_t i = 0; i < otherLen; ++i)
            if (seq->Get(i) == Bit::one)
            {
                std::size_t idx = bitLength + i;
                std::size_t byteIdx = byteIndex(idx);
                int bitOff = bitOffset(idx);
                uint8_t b = result->bytes.Get(byteIdx);
                b |= mask(bitOff);
                result->bytes.Set(byteIdx, b);
            }
        return result;
    }
};