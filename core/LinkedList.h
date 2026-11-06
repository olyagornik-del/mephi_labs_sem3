#ifndef LAB2_LINKEDLIST_H
#define LAB2_LINKEDLIST_H

#include "../my_except.h"

template <class T>

class LinkedList {
private:
    struct Node {
        T value;
        Node *next;
    };
    Node *head;
    Node *tail;
    int size;
public:
    //конструктор по умолчанию
    LinkedList(): head(nullptr), tail(nullptr), size(0){}
    //конструктор делающий LinkedList из массива
    LinkedList(T *items, int size): size(size) {
        if (size<0) {
            throw OutOfRange("size", size, 0, 1000000);
        }
        if (size==0) {
            head = nullptr;
            tail = nullptr;
        }
        else {
            head = new Node{items[0], nullptr};
        }
        Node *current = head;
        for (int i=1; i<size; i++) {
            current->next = new Node{items[i], nullptr};
            current = current->next;
        }
        tail = current;
    }
    //конструктор копирования из LinkedList
    LinkedList(const LinkedList<T> &other_list): size(other_list.size) {
        if (size==0) {
            head = nullptr;
            tail = nullptr;
            return;
        }
        else {
            head = new Node{other_list.head->value, nullptr};
        }
        Node *current = head;
        Node *other_current = other_list.head->next;
        for (int i=1; i<size; i++) {
            current->next = new Node{other_current->value,nullptr};
            current = current->next;
            other_current = other_current->next;
        }
        tail = current;
    }

    //получаем размер списка
    int GetLength() const {
        return size;
    }
    // достаём первый элемент
    T GetFirst() const {
        if (head==nullptr) {
            throw OutOfRange("Список пуст :/");
        }
        return head->value;
    }
    // достаём последний элемент
    T GetLast() const {
        if (head==nullptr) {
            throw OutOfRange("Список пуст :/");
        }
        return tail->value;
    }
    // достаём элемент по индексу
    T Get(int index) const {
        if (head==nullptr) {
            throw OutOfRange("Список пуст :/");
        }
        if (index<0 || index >=size) {
            throw OutOfRange("index", index, 0, size-1);
        }
        Node* current = head;
        for (int i=0; i<index; i++)
            current = current->next;
        return current->value;
    }
    // прибавление элемента в конце
    void Append(T item) {
        Node *new_node = new Node{item, nullptr};
        if (tail==nullptr) {
            head = new_node;
            tail = new_node;
        }
        else {
            tail->next = new_node;
            tail = new_node;
        }
        size++;
    }
    // прибавление в начале
    void Prepend(T item) {
        Node *new_node = new Node{item, head};
        head = new_node;
        if (tail == nullptr)
            tail = new_node;
        size++;
    }
    // прибавление элемента по индексу
    void InsertAt(int index, T item) {
        if (index<0 || index >size) {
            throw OutOfRange("index", index, 0, (int)size);
        }
        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == size) {
            Append(item);
            return;
        }
        Node *prev_el = head;
        for (int i=0; i<index-1; i++) {
            prev_el = prev_el->next;
        }
        Node *next_el = prev_el->next;
        Node *new_node = new Node{item, next_el};
        prev_el->next = new_node;
        size++;
    }
    // подсписок
    LinkedList<T>* GetSubList(int start_index, int end_index) const {
        if (start_index<0 || start_index>end_index || end_index >= size) {
            throw OutOfRange("неверные индексы start_index, end_index");
        }
        LinkedList<T> *result = new LinkedList<T>();
        for (int i = start_index; i <= end_index; i++)
            result->Append(Get(i));
        return result;
    }
    // склеивание списков
    LinkedList<T>* Concat(LinkedList<T> *second_list) const {
        if (size == 0)
            return new LinkedList<T>(*second_list);
        if (second_list->size == 0)
            return new LinkedList<T>(*this);
        LinkedList<T>* result = new LinkedList<T>(*this); //создаём копию нашего, 1ого списка
        Node* current = second_list->head;
        while (current != nullptr) {
            result->Append(current->value);
            current = current->next;
        }
        return result;
    }

    //итератор, чтобы от квадратичной сложности избавиться
    class Iterator {
        Node* node;
    public:
        explicit Iterator(Node* n): node(n) {}
        bool HasNext() const { return node != nullptr; }
        T Current() const { return node->value; }
        void Next() { node = node->next; }
    };
    Iterator GetIterator() const { return Iterator(head); }

    //удалить по индексу
    void RemoveAt(int index) {
        if (size == 0) {
            throw OutOfRange("ну нельзя у пустого массива что-то удалить");
        }
        if (index < 0 || index >= size) {
            throw OutOfRange("index", index, 0, size - 1);
        }
        if (index != 0) {
            Node *temp = head;
            for (int i = 0; i < index - 1; i++) {
                temp = temp->next;
            }
            Node *will_delete = temp->next; //узел, который удалим
            temp->next = will_delete->next;
            if (will_delete == tail) {
                tail = temp;
            }
            delete will_delete;
            size--;
        }
        else {
            Node *will_delete = head;
            head = head->next;
            if (head == nullptr) // список опустел (был единственный узел)
                tail = nullptr;
            delete will_delete;
            size--;
        }

    }

    ~LinkedList() {
        Node *current = head;
        for (int i=0; i<size; i++) {
            Node *next_node = current->next;
            delete current;
            current = next_node;
        }
    }

};

#endif //LAB2_LINKEDLIST_H