#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "exceptions.hpp"
#include <cstddef>
#include <cstdint>

enum class Bit {
    zero,
    one
};

template <typename T = uint8_t>
class BasicBitSequence : public Sequence<Bit>
{
private:
    DynamicArray<T> words;
    std::size_t bitLength;

    static constexpr std::size_t bitsPerWord = sizeof(T) * 8;

    static std::size_t wordIndex(std::size_t bitIndex) noexcept {
        return bitIndex / bitsPerWord;
    }
    static int bitOffset(std::size_t bitIndex) noexcept {
        return static_cast<int>(bitIndex % bitsPerWord);
    }
    static T mask(int offset) noexcept {
        return T(1) << offset;
    }

public:
    BasicBitSequence() : words(0), bitLength(0) {}

    BasicBitSequence(std::size_t size, Bit initialValue = Bit::zero)
        : words((size + bitsPerWord - 1) / bitsPerWord), bitLength(size)
    {
        T fill = (initialValue == Bit::one) ? ~T(0) : T(0);
        for (std::size_t i = 0; i < words.GetSize(); ++i)
            words.Set(i, fill);
        if (size % bitsPerWord != 0 && initialValue == Bit::one)
        {
            std::size_t lastIdx = words.GetSize() - 1;
            T lastWord = words.Get(lastIdx);
            lastWord &= (T(1) << (size % bitsPerWord)) - 1;
            words.Set(lastIdx, lastWord);
        }
    }

    BasicBitSequence(const Bit *bits, std::size_t count) : BasicBitSequence(count)
    {
        for (std::size_t i = 0; i < count; ++i)
            if (bits[i] == Bit::one)
            {
                std::size_t idx = wordIndex(i);
                int off = bitOffset(i);
                T w = words.Get(idx);
                w |= mask(off);
                words.Set(idx, w);
            }
    }

    Bit GetFirst() const override
    {
        if (bitLength == 0)
            throw EmptyStructureException("BasicBitSequence::GetFirst");
        return Get(0);
    }

    Bit GetLast() const override
    {
        if (bitLength == 0)
            throw EmptyStructureException("BasicBitSequence::GetLast");
        return Get(bitLength - 1);
    }

    Bit Get(std::size_t index) const override
    {
        if (index >= bitLength)
            throw IndexOutOfRange(index, bitLength, "BasicBitSequence::Get");
        std::size_t idx = wordIndex(index);
        int off = bitOffset(index);
        T w = words.Get(idx);
        return (w & mask(off)) ? Bit::one : Bit::zero;
    }

    std::size_t GetLength() const override { return bitLength; }

    Sequence<Bit> *GetSubsequence(std::size_t startIndex, std::size_t endIndex) const override
    {
        if (startIndex > endIndex || endIndex >= bitLength)
            throw IndexOutOfRange(endIndex, bitLength, "BasicBitSequence::GetSubsequence");
        std::size_t newSize = endIndex - startIndex + 1;
        BasicBitSequence *sub = new BasicBitSequence(newSize);
        for (std::size_t i = 0; i < newSize; ++i)
        {
            if (Get(startIndex + i) == Bit::one)
            {
                std::size_t idx = wordIndex(i);
                int off = bitOffset(i);
                T w = sub->words.Get(idx);
                w |= mask(off);
                sub->words.Set(idx, w);
            }
        }
        return sub;
    }

    Sequence<Bit> *Append(Bit item) override
    {
        std::size_t idx = bitLength;
        std::size_t wIdx = wordIndex(idx);
        int off = bitOffset(idx);
        if (off == 0)
        {
            words.Resize(words.GetSize() + 1);
            words.Set(words.GetSize() - 1, 0);
        }
        T w = words.Get(wIdx);
        if (item == Bit::one)
            w |= mask(off);
        words.Set(wIdx, w);
        bitLength++;
        return this;
    }

    Sequence<Bit> *Prepend(Bit item) override
    {
        BasicBitSequence *newSeq = new BasicBitSequence(bitLength + 1);
        if (item == Bit::one)
        {
            T w = newSeq->words.Get(0);
            w |= mask(0);
            newSeq->words.Set(0, w);
        }
        for (std::size_t i = 0; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                std::size_t newIdx = i + 1;
                std::size_t idx = wordIndex(newIdx);
                int off = bitOffset(newIdx);
                T w = newSeq->words.Get(idx);
                w |= mask(off);
                newSeq->words.Set(idx, w);
            }
        return newSeq;
    }
    Sequence<Bit> *InsertAt(Bit item, std::size_t index) override
    {
        if (index > bitLength)
            throw IndexOutOfRange(index, bitLength + 1, "BasicBitSequence::InsertAt");
        BasicBitSequence *newSeq = new BasicBitSequence(bitLength + 1);
        for (std::size_t i = 0; i < index; ++i)
            if (Get(i) == Bit::one)
            {
                std::size_t idx = wordIndex(i);
                int off = bitOffset(i);
                T w = newSeq->words.Get(idx);
                w |= mask(off);
                newSeq->words.Set(idx, w);
            }
        if (item == Bit::one)
        {
            std::size_t idx = wordIndex(index);
            int off = bitOffset(index);
            T w = newSeq->words.Get(idx);
            w |= mask(off);
            newSeq->words.Set(idx, w);
        }
        for (std::size_t i = index; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                std::size_t newIdx = i + 1;
                std::size_t idx = wordIndex(newIdx);
                int off = bitOffset(newIdx);
                T w = newSeq->words.Get(idx);
                w |= mask(off);
                newSeq->words.Set(idx, w);
            }
        return newSeq;
    }

    Sequence<Bit> *Concat(Sequence<Bit> *seq) const override
    {
        if (!seq)
            throw InvalidArgument("BasicBitSequence::Concat: пустой указатель на последовательность");
        std::size_t otherLen = seq->GetLength();
        BasicBitSequence *result = new BasicBitSequence(bitLength + otherLen);
        for (std::size_t i = 0; i < bitLength; ++i)
            if (Get(i) == Bit::one)
            {
                std::size_t idx = wordIndex(i);
                int off = bitOffset(i);
                T w = result->words.Get(idx);
                w |= mask(off);
                result->words.Set(idx, w);
            }
        for (std::size_t i = 0; i < otherLen; ++i)
            if (seq->Get(i) == Bit::one)
            {
                std::size_t globalIdx = bitLength + i;
                std::size_t idx = wordIndex(globalIdx);
                int off = bitOffset(globalIdx);
                T w = result->words.Get(idx);
                w |= mask(off);
                result->words.Set(idx, w);
            }
        return result;
    }
};

using BitSequence = BasicBitSequence<uint8_t>;