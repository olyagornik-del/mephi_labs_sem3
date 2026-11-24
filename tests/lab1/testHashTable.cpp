#include <cassert> // assert
#include <cstddef> // std::size_t
#include <cstdio> //printf
#include <string>

#include "../../core/HashTable.h"

// плохой хеш нарочно: всё попадает в одну корзину — одна длинная цепочка
struct ConstantHash {
    std::size_t operator()(int) const {
        return 0;
    }
};

void testHashTableEmpty() {
    HashTable<int> Table;
    assert(Table.IsEmpty());
    assert(Table.GetCount() == 0);
    assert(Table.GetBucketCount() == 8);
    assert(!Table.Contains(1));

    bool threw = false;
    try { Table.Remove(1); } catch (const NotFound&) { threw = true; }
    assert(threw);

    printf("  [OK] testHashTableEmpty\n");
}

void testHashTableInsertContainsRemove() {
    HashTable<std::string> Table;
    Table.Insert("кот");
    Table.Insert("арбуз");
    Table.Insert("якорь");
    assert(Table.GetCount() == 3);
    assert(Table.Contains("арбуз") && !Table.Contains("собака"));

    Table.Remove("кот");
    assert(!Table.Contains("кот"));
    assert(Table.GetCount() == 2);

    std::string found;
    assert(Table.TryFind("якорь", found) && found == "якорь");
    assert(!Table.TryFind("кот", found));

    printf("  [OK] testHashTableInsertContainsRemove\n");
}

void testHashTableDuplicate() {
    HashTable<int> Table;
    Table.Insert(5);
    bool threw = false;
    try { Table.Insert(5); } catch (const AlreadyExists&) { threw = true; }
    assert(threw);
    assert(Table.GetCount() == 1);

    printf("  [OK] testHashTableDuplicate\n");
}

void testHashTableCollisions() {
    // все элементы в одной корзине — таблица медленная, но должна работать правильно
    HashTable<int, ConstantHash> Table;
    for (int i = 0; i < 50; i++)
        Table.Insert(i);
    for (int i = 0; i < 50; i++)
        assert(Table.Contains(i));

    // удаляем из начала, середины и конца цепочки
    Table.Remove(0);
    Table.Remove(25);
    Table.Remove(49);
    assert(!Table.Contains(0) && !Table.Contains(25) && !Table.Contains(49));
    assert(Table.Contains(1) && Table.Contains(24) && Table.Contains(48));
    assert(Table.GetCount() == 47);

    printf("  [OK] testHashTableCollisions\n");
}

void testHashTableGrowShrink() {
    HashTable<int> Table;
    // 8 элементов в 8 корзинах — ещё не расширяемся
    for (int i = 0; i < 8; i++)
        Table.Insert(i);
    assert(Table.GetBucketCount() == 8);
    // 9-й — заполнение больше 1, корзин 16
    Table.Insert(8);
    assert(Table.GetBucketCount() == 16);
    // после перестройки все на месте
    for (int i = 0; i < 9; i++)
        assert(Table.Contains(i));

    // удаляем до 3 элементов: 3 < 16/4 — сжимаемся до 8
    for (int i = 0; i < 6; i++)
        Table.Remove(i);
    assert(Table.GetCount() == 3);
    assert(Table.GetBucketCount() == 8);
    assert(Table.Contains(6) && Table.Contains(7) && Table.Contains(8));

    // меньше 8 корзин не бывает
    for (int i = 6; i < 9; i++)
        Table.Remove(i);
    assert(Table.IsEmpty() && Table.GetBucketCount() == 8);

    printf("  [OK] testHashTableGrowShrink\n");
}

void testHashTablePattern() {
    // числа, кратные 8: у int хеш-код = само число, при 8 корзинах все попадают в корзину 0.
    // проверяем, что "узорные" ключи не ломают таблицу (а перестройка их раскидывает)
    HashTable<int> Table;
    for (int i = 0; i < 100; i++)
        Table.Insert(i * 8);
    for (int i = 0; i < 100; i++)
        assert(Table.Contains(i * 8));
    assert(!Table.Contains(4));

    printf("  [OK] testHashTablePattern\n");
}

void testHashTableCopy() {
    HashTable<int> Table;
    for (int i = 0; i < 20; i++)
        Table.Insert(i);

    HashTable<int> Copy(Table);
    Copy.Remove(0);
    Copy.Insert(100);
    assert(Table.Contains(0) && !Table.Contains(100));
    assert(!Copy.Contains(0) && Copy.Contains(100));

    HashTable<int> Assigned;
    Assigned.Insert(-1);
    Assigned = Table;
    assert(!Assigned.Contains(-1));
    assert(Assigned.GetCount() == 20 && Assigned.GetBucketCount() == Table.GetBucketCount());

    printf("  [OK] testHashTableCopy\n");
}

void testHashTableStress() {
    // 10000 чисел вперемешку, удаляем чётные — таблица растёт, потом сжимается
    HashTable<int> Table;
    int n = 10000;
    for (int i = 0; i < n; i++)
        Table.Insert((i * 7919) % n);
    assert(Table.GetCount() == n);
    assert(Table.GetBucketCount() >= n); // заполнение не больше 1

    for (int i = 0; i < n; i += 2)
        Table.Remove(i);
    assert(Table.GetCount() == n / 2);
    for (int i = 0; i < n; i++)
        assert(Table.Contains(i) == (i % 2 == 1));

    printf("  [OK] testHashTableStress\n");
}

void testHashTableAll() {
    printf("=== Тесты HashTable ===\n");
    testHashTableEmpty();
    testHashTableInsertContainsRemove();
    testHashTableDuplicate();
    testHashTableCollisions();
    testHashTableGrowShrink();
    testHashTablePattern();
    testHashTableCopy();
    testHashTableStress();
    printf("=== Все тесты пройдены! ===\n\n");
}
