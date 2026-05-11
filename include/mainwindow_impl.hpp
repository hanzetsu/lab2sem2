#pragma once
#include <QMessageBox>

template<typename T>
void MainWindow::appendToSequence(Sequence<T>*& seq, T value)
{
    MutableArraySequence<T>* mArr = dynamic_cast<MutableArraySequence<T>*>(seq);
    MutableListSequence<T>* mList = dynamic_cast<MutableListSequence<T>*>(seq);
    if (mArr) mArr->Append(value);
    else if (mList) mList->Append(value);
    else {
        Sequence<T>* newSeq = seq->Append(value);
        delete seq;
        seq = newSeq;
    }
}

template<typename T>
void MainWindow::prependToSequence(Sequence<T>*& seq, T value)
{
    MutableArraySequence<T>* mArr = dynamic_cast<MutableArraySequence<T>*>(seq);
    MutableListSequence<T>* mList = dynamic_cast<MutableListSequence<T>*>(seq);
    if (mArr) mArr->Prepend(value);
    else if (mList) mList->Prepend(value);
    else {
        Sequence<T>* newSeq = seq->Prepend(value);
        delete seq;
        seq = newSeq;
    }
}

template<typename T>
void MainWindow::insertToSequence(Sequence<T>*& seq, T value, std::size_t index)
{
    MutableArraySequence<T>* mArr = dynamic_cast<MutableArraySequence<T>*>(seq);
    MutableListSequence<T>* mList = dynamic_cast<MutableListSequence<T>*>(seq);
    if (mArr) mArr->InsertAt(value, index);
    else if (mList) mList->InsertAt(value, index);
    else {
        Sequence<T>* newSeq = seq->InsertAt(value, index);
        delete seq;
        seq = newSeq;
    }
}

template<typename T>
void MainWindow::removeFromSequence(Sequence<T>*& seq, std::size_t index)
{
    std::size_t len = seq->GetLength();
    if (index >= len) throw IndexOutOfRange(index, len, "removeFromSequence");
    Sequence<T>* newSeq = nullptr;
    if (dynamic_cast<MutableArraySequence<T>*>(seq))
        newSeq = new MutableArraySequence<T>();
    else if (dynamic_cast<MutableListSequence<T>*>(seq))
        newSeq = new MutableListSequence<T>();
    else if (dynamic_cast<ImmutableArraySequence<T>*>(seq))
        newSeq = new ImmutableArraySequence<T>();
    else if (dynamic_cast<ImmutableListSequence<T>*>(seq))
        newSeq = new ImmutableListSequence<T>();
    else
        throw InvalidArgument("Неизвестный тип последовательности");

    for (std::size_t i = 0; i < len; ++i) {
        if (i == index) continue;
        if (auto* mutableSeq = dynamic_cast<MutableArraySequence<T>*>(newSeq)) {
            mutableSeq->Append(seq->Get(i));
        } else if (auto* mutableSeq = dynamic_cast<MutableListSequence<T>*>(newSeq)) {
            mutableSeq->Append(seq->Get(i));
        } else {
            Sequence<T>* temp = newSeq->Append(seq->Get(i));
            if (temp != newSeq) {
                delete newSeq;
                newSeq = temp;
            }
        }
    }
    delete seq;
    seq = newSeq;
}

template<typename T>
void MainWindow::editInSequence(Sequence<T>*& seq, std::size_t index, T newValue)
{
    MutableArraySequence<T>* mArr = dynamic_cast<MutableArraySequence<T>*>(seq);
    MutableListSequence<T>* mList = dynamic_cast<MutableListSequence<T>*>(seq);
    if (mArr) {
        mArr->Set(index, newValue);
    }
    else if (mList) {
        std::size_t len = seq->GetLength();
        if (index >= len) throw IndexOutOfRange(index, len, "editInSequence (list)");
        Sequence<T>* newSeq = nullptr;
        if (dynamic_cast<MutableListSequence<T>*>(seq))
            newSeq = new MutableListSequence<T>();
        else
            newSeq = new ImmutableListSequence<T>();
        for (std::size_t i = 0; i < len; ++i) {
            T val = (i == index) ? newValue : seq->Get(i);
            if (auto* mutableSeq = dynamic_cast<MutableArraySequence<T>*>(newSeq)) {
                mutableSeq->Append(val);
            } else if (auto* mutableSeq = dynamic_cast<MutableListSequence<T>*>(newSeq)) {
                mutableSeq->Append(val);
            } else {
                Sequence<T>* temp = newSeq->Append(val);
                if (temp != newSeq) {
                    delete newSeq;
                    newSeq = temp;
                }
            }
        }
        delete seq;
        seq = newSeq;
    }
    else {
        std::size_t len = seq->GetLength();
        Sequence<T>* newSeq = nullptr;
        if (dynamic_cast<ImmutableArraySequence<T>*>(seq))
            newSeq = new ImmutableArraySequence<T>(len);
        else if (dynamic_cast<ImmutableListSequence<T>*>(seq))
            newSeq = new ImmutableListSequence<T>();
        else
            throw InvalidArgument("Неизвестный тип последовательности");
        for (std::size_t i = 0; i < len; ++i) {
            T val = (i == index) ? newValue : seq->Get(i);
            Sequence<T>* temp = newSeq->Append(val);
            if (temp != newSeq) {
                delete newSeq;
                newSeq = temp;
            }
        }
        delete seq;
        seq = newSeq;
    }
}