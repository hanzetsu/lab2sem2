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

    // Добавление элемента (fluent)
    SequenceBuilder<T>& add(const T& value) {
        int oldSize = items->GetSize();
        items->Resize(oldSize + 1);
        items->Set(oldSize, value);
        return *this;
    }

    // Выбор типа последовательности
    SequenceBuilder<T>& setType(Type t) {
        type = t;
        return *this;
    }

    // Удобные методы для выбора типа
    SequenceBuilder<T>& mutableArray() { setType(Type::MutableArray); return *this; }
    SequenceBuilder<T>& mutableList()  { setType(Type::MutableList);  return *this; }
    SequenceBuilder<T>& immutableArray(){ setType(Type::ImmutableArray); return *this; }
    SequenceBuilder<T>& immutableList() { setType(Type::ImmutableList); return *this; }

    // Построение последовательности
    Sequence<T>* build() const {
        int len = items->GetSize();
        const T* rawData = items->GetRawData();  // указатель на массив
        // Мы должны скопировать данные, так как построенный объект будет владеть своей копией.
        // Конструкторы MutableArraySequence и т.д. принимают T* items, int count и делают копию.
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
        return nullptr; // никогда не выполнится
    }

    // Очистить накопленные элементы (если хотим переиспользовать Builder)
    void clear() {
        delete items;
        items = new DynamicArray<T>();
    }

private:
    DynamicArray<T>* items;
    Type type = Type::MutableArray; // по умолчанию
};