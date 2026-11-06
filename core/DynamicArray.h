#ifndef LAB2_DYNAMICARRAY_H
#define LAB2_DYNAMICARRAY_H

#include "../my_except.h"

template <class T>

class DynamicArray {
private:
    T *data;
    int size;
    int capacity;

public:
    // конструктор по умолчанию
    DynamicArray(): data(nullptr), size(0), capacity(0) {}
    // конструктор куда передаём только размер, заполняем нулями
    explicit DynamicArray(int size) {
        if (size<0)
            throw InvalidArgument("size");
        this->size = size;
        data = new T [size]{};
        capacity = size;
    }
    // конструктор с размером и переданными данными
    DynamicArray(T *items, int size) {
        if (size<0) {
            throw InvalidArgument("size");
        }
        this->size = size;
        data = new T [size]{};
        for (int i=0; i<size; i++) {
            data[i] = items[i];
        }
        capacity = size;
    }
    //копирующий конструктор
    DynamicArray(const DynamicArray<T> &other_arr): size(other_arr.size) {
        data = new T[size]{};
        for (int i=0; i<size; i++) {
            data[i] = other_arr.data[i];
        }
        capacity = size;
    }

    //получаем размер списка
    int GetSize() const {
        return size;
    }
    // достаём элемент по индексу
    T Get(int index) const {
        if (index < 0 || index >= size) {
            throw OutOfRange("index", index, 0, size - 1);
        }
        return data[index];
    }
    //изменить элемент по номеру
    void Set(int index, T value) {
        if (index < 0 || index >= size) {
            throw OutOfRange("index", index, 0, size - 1);
        }
        data[index] = value;
    }
    // изменение размера
    void Resize(int new_size) {
        if (new_size>capacity) {
            capacity = (new_size > capacity * 2) ? new_size : capacity * 2;
            T* new_data = new T [capacity]{};
            for (int i=0; i<size; i++) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            size = new_size;
        }
        else {
            size = new_size;
        }
    }

    //удаляем элемент по индексу
    void RemoveAt(int index) {
        if (size == 0) {
            throw OutOfRange("ну нельзя у пустого массива что-то удалить");
        }
        if (index < 0 || index >= size) {
            throw OutOfRange("index", index, 0, size - 1);
        }
        for (int i = index+1; i < size; i++ ) {
            data[i-1] = data[i]; //можно и Set(i-1, Get(i)); но это влечёт доп проверки, а зачем
        }
        Resize(size-1);
    }

    ~DynamicArray() {
        delete[] data;
    }
};

#endif //LAB2_DYNAMICARRAY_H