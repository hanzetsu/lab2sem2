#pragma once
#include <stdexcept>
#include <cstddef>

template <typename T>
class Node
{
    template <class U>
    friend class LinkedList;

private:
    T data;
    Node<T> *prev;
    Node<T> *next;
    Node(const T &val) : data(val), prev(nullptr), next(nullptr) {};
};

template <typename T>
class LinkedList
{
private:
    std::size_t size;
    Node<T> *head;
    Node<T> *tail;

public:
    LinkedList(T *items, std::size_t count);
    LinkedList() : head(nullptr), tail(nullptr), size(0) {};
    LinkedList(const LinkedList<T> &list);
    ~LinkedList()
    {
        Node<T> *temp;
        while (head != nullptr)
        {
            temp = head->next;
            delete head;
            head = temp;
        }
    };
    T GetFirst()
    {
        if (size == 0)
            throw std::out_of_range("Нет элементов");
        return head->data;
    };
    T GetLast()
    {
        if (size == 0)
            throw std::out_of_range("Нет элементов");
        return tail->data;
    };
    T Get(std::size_t index);
    LinkedList<T> *GetSubList(std::size_t startIndex, std::size_t endIndex);
    std::size_t GetLength() { return size; }
    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, std::size_t index);
    void Concat(LinkedList<T> &list);
};

template <typename T>
T LinkedList<T>::Get(std::size_t index)
{
    if (index >= size)
        throw std::out_of_range("Индекс вне диапазона");
    Node<T> *temp = head;
    for (std::size_t i = 0; i < index; i++)
        temp = temp->next;
    return temp->data;
}

template <typename T>
LinkedList<T>::LinkedList(T *items, std::size_t count) : head(nullptr), tail(nullptr), size(0)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        Node<T> *newNode = new Node<T>(items[i]);
        if (!head)
            head = tail = newNode;
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &list) : LinkedList()
{
    Node<T> *tmp = list.head;
    while (tmp != nullptr)
    {
        Append(tmp->data);
        tmp = tmp->next;
    }
}

template <typename T>
LinkedList<T> *LinkedList<T>::GetSubList(std::size_t startIndex, std::size_t endIndex)
{
    if (startIndex > endIndex || endIndex >= size)
        throw std::out_of_range("Неверные индексы подсписка");
    LinkedList<T> *newList = new LinkedList<T>();
    Node<T> *tmp = head;
    for (std::size_t i = 0; i < startIndex; i++)
        tmp = tmp->next;
    for (std::size_t i = startIndex; i <= endIndex; i++)
    {
        newList->Append(tmp->data);
        tmp = tmp->next;
    }
    return newList;
}

template <typename T>
void LinkedList<T>::Append(T item)
{
    Node<T> *newTail = new Node<T>(item);
    if (size == 0)
        head = tail = newTail;
    else
    {
        tail->next = newTail;
        newTail->prev = tail;
        tail = newTail;
    }
    size++;
}

template <typename T>
void LinkedList<T>::Prepend(T item)
{
    Node<T> *newHead = new Node<T>(item);
    if (size == 0)
        head = tail = newHead;
    else
    {
        head->prev = newHead;
        newHead->next = head;
        head = newHead;
    }
    size++;
}

template <typename T>
void LinkedList<T>::InsertAt(T item, std::size_t index)
{
    if (index > size)
        throw std::out_of_range("Неверный индекс вставки");
    if (index == 0)
    {
        Prepend(item);
        return;
    }
    if (index == size)
    {
        Append(item);
        return;
    }
    Node<T> *current = head;
    for (std::size_t i = 0; i < index; i++)
        current = current->next;
    Node<T> *newNode = new Node<T>(item);
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    size++;
}

template <typename T>
void LinkedList<T>::Concat(LinkedList<T> &list)
{
    if (list.size == 0)
        return;
    if (size == 0)
    {
        head = list.head;
        tail = list.tail;
    }
    else
    {
        tail->next = list.head;
        list.head->prev = tail;
        tail = list.tail;
    }
    size += list.size;
    list.head = list.tail = nullptr;
    list.size = 0;
}