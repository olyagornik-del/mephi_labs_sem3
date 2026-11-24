#include <cassert> // assert
#include <cstdio> //printf

#include "../../core/DynamicArray.h"
#include "../../core/LinkedList.h"

// тесты на исправления в классах из lab4: operator= и GetRef

void testLinkedListAssign() {
    int a[] = {1, 2, 3};
    int b[] = {7, 8};
    LinkedList<int> First(a, 3);
    LinkedList<int> Second(b, 2);

    Second = First; // старые узлы Second (7, 8) должны удалиться
    assert(Second.GetLength() == 3);
    assert(Second.Get(0) == 1 && Second.Get(2) == 3);

    // списки независимы: меняем один — другой не меняется
    Second.Append(4);
    First.RemoveAt(0);
    assert(First.GetLength() == 2 && First.Get(0) == 2);
    assert(Second.GetLength() == 4 && Second.Get(0) == 1 && Second.GetLast() == 4);

    // присваивание пустого и самому себе
    LinkedList<int> Empty;
    Second = Second;
    assert(Second.GetLength() == 4);
    Second = Empty;
    assert(Second.GetLength() == 0);
    Second.Append(5); // после присваивания пустого список рабочий
    assert(Second.GetFirst() == 5 && Second.GetLast() == 5);

    printf("  [OK] testLinkedListAssign\n");
}

void testDynamicArrayAssign() {
    int a[] = {1, 2, 3};
    DynamicArray<int> First(a, 3);
    DynamicArray<int> Second(5);

    Second = First;
    assert(Second.GetSize() == 3 && Second.Get(1) == 2);
    Second.Set(1, 20);
    assert(First.Get(1) == 2); // независимы

    Second = Second;
    assert(Second.GetSize() == 3 && Second.Get(1) == 20);

    printf("  [OK] testDynamicArrayAssign\n");
}

void testDynamicArrayGetRef() {
    DynamicArray<int> Arr(3);
    Arr.GetRef(1) = 42; // меняем элемент прямо в массиве
    assert(Arr.Get(1) == 42);

    bool threw = false;
    try { Arr.GetRef(3); } catch (const OutOfRange&) { threw = true; }
    assert(threw);

    const DynamicArray<int> &ConstArr = Arr;
    assert(ConstArr.GetRef(1) == 42); // const-версия только читает

    printf("  [OK] testDynamicArrayGetRef\n");
}

void testArrayOfLists() {
    // ровно то, из чего сделана хеш-таблица: массив списков.
    // до исправлений Resize и = копировали только указатели — тут бы всё упало
    DynamicArray<LinkedList<int>> Buckets(2);
    Buckets.GetRef(0).Append(1);
    Buckets.GetRef(0).Append(2);
    Buckets.GetRef(1).Append(3);

    Buckets.Resize(10); // внутри копирование списков в новый массив
    assert(Buckets.GetRef(0).GetLength() == 2);
    assert(Buckets.GetRef(1).Get(0) == 3);

    DynamicArray<LinkedList<int>> Copy(1);
    Copy = Buckets;
    Copy.GetRef(0).Append(99);
    assert(Buckets.GetRef(0).GetLength() == 2); // оригинал не тронут
    assert(Copy.GetRef(0).GetLength() == 3);

    printf("  [OK] testArrayOfLists\n");
}

void testCoreCopyAll() {
    printf("=== Тесты копирования DynamicArray/LinkedList ===\n");
    testLinkedListAssign();
    testDynamicArrayAssign();
    testDynamicArrayGetRef();
    testArrayOfLists();
    printf("=== Все тесты пройдены! ===\n\n");
}
