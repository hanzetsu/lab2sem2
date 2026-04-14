#pragma once
#include <stdexcept>

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
    int size;
    Node<T> *head;
    Node<T> *tail;

public:
    LinkedList(T *items, int count);
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
        if (size == 0) throw std::out_of_range("Нет элементов");
        return head->data;
    };
    T GetLast()
    {
        if (size == 0) throw std::out_of_range("Нет элементов");
        return tail->data;
    };
    T Get(int index)
    {
        if (index < 0 || index >= size)
            throw std::invalid_argument("Индекс не может быть < 0 или больше/равен размеру");
        Node<T> *temp = head;
        for (int i = 0; i < index; i++)
        {
            temp = temp->next;
        }
        return temp->data;
    };

    LinkedList<T> *GetSubList(int startIndex, int endIndex)
    {
        LinkedList<T> *newList = new LinkedList<T>();
        if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
            throw std::out_of_range("Передаваемые параметры startIndex и/или endIndex - некорректны");
        else
        {
            int newSize = endIndex - startIndex + 1;
            Node<T> *tmp = head;
            for (int i = 0; i < startIndex; i++)
                tmp = tmp->next;

            for (int i = 0; i < newSize; i++)
            {
                T newData = tmp->data;
                newList->Append(newData);
                tmp = tmp->next;
            }
            return newList;
        }
    };

    int GetLength()
    {
        return size;
    };
    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    void Concat(LinkedList<T> &list);
};

template <typename T>
LinkedList<T>::LinkedList(T *items, int count) : head(nullptr), tail(nullptr), size(0)
{
    for (int i = 0; i < count; ++i)
    {
        Node<T> *newNode = new Node<T>(items[i]);
        if (!head)
        {
            head = tail = newNode;
        }
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
    if (list->size != 0)
    {
        Node<T> *tmp = list.head;
        T newData;
        while (tmp != nullptr)
        {
            newData = tmp->data;
            Append(newData);
            tmp = tmp->next;
        }
    }
}

template <typename T>
void LinkedList<T>::Append(T item)
{
    Node<T> *newTail = new Node<T>(item);
    if (size == 0)
    {
        head = newTail;
        tail = newTail;
    }
    else
    {
        tail->next = newTail;
        newTail->prev = tail;
        tail = newTail;
    }
    size++;
};

template <typename T>
void LinkedList<T>::Prepend(T item)
{
    Node<T> *newHead = new Node<T>(item);
    if (size == 0)
    {
        head = newHead;
        tail = newHead;
    }
    else
    {
        head->prev = newHead;
        newHead->next = head;
        head = newHead;
    }
    size++;
}
template <typename T>
void LinkedList<T>::InsertAt(T item, int index)
{
    if (index > size || index < 0)
        throw std::out_of_range("Индекс вставки неверный");
    else if (index == 0)
    {
        Prepend(item);
            return;
    }
    else if (index == size)
    {
        Append(item);
        return;
    }
    Node<T> *current = head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    Node<T> *newNode = new Node<T>(item);
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    size++;
};

template <typename T>
void LinkedList<T>::Concat(LinkedList<T> &list)
{
    if (list.size == 0)
        return;
    else if (size == 0)
    {
        head = list.head;
        tail = list.tail;
    }
    else
    {
        this->tail->next = list.head;
        list.head->prev = this->tail;
        this->tail = list.tail;
    }
    size = size + list.size;
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
};