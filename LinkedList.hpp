#pragma once

template <typename T>
class Node {
    template <class U> friend class LinkedList;
    private:
    T data;
    Node *prev;
    Node *next;
    Node (const T& val) : data(val), prev(nullptr), next(nullptr){};
};

template <typename T> 
class LinkedList {
    private:
    int size;
    Node<T> *head;
    Node<T> *tail;
    public:
    LinkedList (T* items, int count);
    LinkedList () : head(nullptr), tail(nullptr), size(0) {};
    LinkedList (const LinkedList <T> & list);
    ~LinkedList () {
        Node <T>* temp;
            while (head != nullptr) {
                temp = head->next;
                delete head;
                head = temp;
            }
    };
    T GetFirst() {
        return head;
    };
    T GetLast() {
        return tail;
    };
    T Get(int index) {
        Node<T>* temp;
        for (int i = 0; i < index; i++) {
            temp = head->next
        }
        return temp;
    };
    LinkedList<T>* GetSubList(int startIndex, int endIndex) {
        Node<T>* temp;
        for (int i = 0; i < startIndex; i ++) {
            temp = head->next;
            delete head;
            head = temp;
        }
        Node<T>* endList = Get(endIndex);
        Node<T>* endListNext = endList->next;
        while (endList != nullptr) {
            temp = endListNext->next;
            delete endListNext;
            endListNext = temp;
        }
    };
    int GetLength() {
        return size;
    };
    void Append(T item);
    void Prepend(T item);   
    void InsertAt(T item, int index);
    LinkedList<T>* Concat(LinkedList<T> *list);
};

template <typename T>
LinkedList<T>::LinkedList(T* items, int count) : head(nullptr), tail(nullptr), size(0) {
    for (int i = 0; i < count; ++i) {
        Node<T>* newNode = new Node<T>(items[i]);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }
}

template <typename T>
void LinkedList<T>::Append(T item) {
        Node<T>* newTail = new Node<T>;
        tail->next = newTail;
        newTail.next = nullptr;
        newTail->prev = tail;
        tail = newTail;
        tail->data = item;
        size++;
    };

template <typename T>
void LinkedList<T>::Prepend(T item) {
    Node<T> newHead = new Node<T>;
    head->prev = newHead;
    newHead.data = item;
    newHead.prev = nullptr;
    newHead.next = head;
    head = newHead;
    size++;
}
template <typename T>
void LinkedList<T>::InsertAt(T item, int index){
    Node<T> old = Get(index);
    Node<T> newNode = new Node<T>;
    newNode.next = old.next;
    old.next = newNode;
    newNode.prev = old;
    newNode.data = item;
    size++;
};

template <typename T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T> *list) {
    this->tail->next = list->head;
    list->head->prev = this->tail;
    list->head = this->head;
    this->tail = list->tail;
    this->size = this->size + list->size;
    list->size = this->size;
};