#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "Bit.hpp"
#include <cstdint>

class BitSequence : public Sequence<Bit>
{
private:
    DynamicArray<uint8_t> *bytes;
    int bitLength;

public:
    ~BitSequence()
    {
        delete bytes;
    }
    BitSequence()
    {
        bytes = new DynamicArray<uint8_t>(0);
        bitLength = 0;
    }
    Bit GetFirst() const
    {
        if (bitLength == 0)
            throw std::out_of_range("Пусто");
        return Get(0);
    }
    Bit GetLast() const
    {
        if (bitLength == 0)
            throw std::out_of_range("Пусто");
        return Get(bitLength - 1);
    }
    Bit Get(int index) const;
    Sequence<Bit> *GetSubsequence(int startIndex, int endIndex) const;
    int GetLength() const
    {
        return bitLength;
    }
    Sequence<Bit> *Append(Bit item);
    Sequence<Bit> *Prepend(Bit item);
    Sequence<Bit> *InsertAt(Bit item, int index);
    Sequence<Bit> *Concat(Sequence<Bit> *list) const;
};

Bit BitSequence::Get(int index) const
{
    if (index < 0 || index >= bitLength)
        throw std::out_of_range("Индекс вне диапазона");

    int byteIndex = index / 8;
    int bitIndex = index % 8;

    uint8_t byte = bytes->Get(byteIndex);

    return (byte & (1 << bitIndex)) ? Bit::one : Bit::zero;
}

Sequence<Bit> *BitSequence::Append(Bit item)
{
    int newBitIndex = bitLength;
    int byteIndex = newBitIndex / 8;
    int bitIndex = newBitIndex % 8;

    if (bitIndex == 0)
    {
        int oldSize = bytes->GetSize();
        bytes->Resize(oldSize + 1);
        bytes->Set(oldSize, 0);
    }

    uint8_t byte = bytes->Get(byteIndex);

    if (item == Bit::one)
        byte |= (1 << bitIndex);

    bytes->Set(byteIndex, byte);
    bitLength++;

    return this;
}

Sequence<Bit> *BitSequence::Prepend(Bit item)
{
    BitSequence *result = new BitSequence();

    result->Append(item);

    for (int i = 0; i < bitLength; i++)
        result->Append(Get(i));

    return result;
}

Sequence<Bit> *BitSequence::InsertAt(Bit item, int index)
{
    if (index < 0 || index > bitLength)
        throw std::out_of_range("Неверный индекс");

    BitSequence *result = new BitSequence();

    for (int i = 0; i < index; i++)
        result->Append(Get(i));

    result->Append(item);

    for (int i = index; i < bitLength; i++)
        result->Append(Get(i));

    return result;
}
Sequence<Bit> *BitSequence::GetSubsequence(int startIndex, int endIndex) const
{
    if (startIndex < 0 || endIndex >= bitLength || startIndex > endIndex)
        throw std::out_of_range("Ошибка индексов");

    BitSequence *result = new BitSequence();

    for (int i = startIndex; i <= endIndex; i++)
        result->Append(Get(i));

    return result;
}

Sequence<Bit> *BitSequence::Concat(Sequence<Bit> *list) const
{
    BitSequence *result = new BitSequence();

    for (int i = 0; i < bitLength; i++)
        result->Append(Get(i));

    for (int i = 0; i < list->GetLength(); i++)
        result->Append(list->Get(i));

    return result;
}