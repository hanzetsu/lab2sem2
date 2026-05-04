#pragma once
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "MutableArraySequence.hpp"
#include "MutableListSequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"

template <typename T>
class SequenceBuilder {
public:
    enum class Type {
        MutableArray,
        MutableList,
        ImmutableArray,
        ImmutableList
    };

    SequenceBuilder() : items(new DynamicArray<T>()) {}
    ~SequenceBuilder() { delete items; }

    SequenceBuilder<T>& add(const T& value) {
        int oldSize = items->GetSize();
        items->Resize(oldSize + 1);
        items->Set(oldSize, value);
        return *this;
    }


    SequenceBuilder<T>& setType(Type t) {
        type = t;
        return *this;
    }

    SequenceBuilder<T>& mutableArray() { setType(Type::MutableArray); return *this; }
    SequenceBuilder<T>& mutableList()  { setType(Type::MutableList);  return *this; }
    SequenceBuilder<T>& immutableArray(){ setType(Type::ImmutableArray); return *this; }
    SequenceBuilder<T>& immutableList() { setType(Type::ImmutableList); return *this; }

    Sequence<T>* build() const {
        int len = items->GetSize();
        const T* rawData = items->GetRawData();
        switch (type) {
            case Type::MutableArray:
                return new MutableArraySequence<T>(const_cast<T*>(rawData), len);
            case Type::MutableList:
                return new MutableListSequence<T>(const_cast<T*>(rawData), len);
            case Type::ImmutableArray:
                return new ImmutableArraySequence<T>(const_cast<T*>(rawData), len);
            case Type::ImmutableList:
                return new ImmutableListSequence<T>(const_cast<T*>(rawData), len);
        }
        return nullptr;
    }

    void clear() {
        delete items;
        items = new DynamicArray<T>();
    }

private:
    DynamicArray<T>* items;
    Type type;
};