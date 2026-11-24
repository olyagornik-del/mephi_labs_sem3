#ifndef LABS_HASHTABLE_H
#define LABS_HASHTABLE_H

#include <cstddef> // std::size_t
#include <functional> // std::hash, std::equal_to
#include "../my_except.h"
#include "DynamicArray.h"
#include "LinkedList.h"

// хеш-таблица с цепочками: массив корзин, каждая корзина — связный список.
// номер корзины = хеш-код элемента % число корзин; элементы с одинаковым номером (коллизия)
// лежат в одном списке, и при поиске проходим только по нему.
// Hash — как получить хеш-код (по умолчанию std::hash), Equal — как проверить равенство (==)
//   HashTable<int> t;
//   HashTable<Coord, CoordHash> t2;   — свой хеш для своего типа
template <class T, class Hash = std::hash<T>, class Equal = std::equal_to<T>>
class HashTable {
private:
    static const int MIN_BUCKETS = 8; // меньше 8 корзин не сжимаемся

    DynamicArray<LinkedList<T>> buckets;
    int count;
    Hash hash;   // объект-функтор: hash(x) — хеш-код x
    Equal equal; // объект-функтор: equal(a, b) == true, если a и b равны

    // в какую корзину попадает элемент
    int BucketIndex(const T &item) const {
        return (int)(hash(item) % (std::size_t)buckets.GetSize());
    }

    // номер элемента внутри списка-корзины или -1, если его там нет
    int IndexInBucket(const LinkedList<T> &bucket, const T &item) const {
        int i = 0;
        for (auto it = bucket.GetIterator(); it.HasNext(); it.Next(), i++) {
            if (equal(it.Current(), item))
                return i;
        }
        return -1;
    }

    // рехеширование: новый массив из new_bucket_count корзин, все элементы раскладываем заново
    // (номер корзины считается от числа корзин, поэтому у многих он поменяется)
    void Rebuild(int new_bucket_count) {
        DynamicArray<LinkedList<T>> new_buckets(new_bucket_count);
        for (int b = 0; b < buckets.GetSize(); b++) {
            const LinkedList<T> &bucket = buckets.GetRef(b);
            for (auto it = bucket.GetIterator(); it.HasNext(); it.Next()) {
                int index = (int)(hash(it.Current()) % (std::size_t)new_bucket_count);
                new_buckets.GetRef(index).Append(it.Current());
            }
        }
        buckets = new_buckets;
    }

public:
    //конструктор по умолчанию — 8 пустых корзин
    HashTable(): buckets(MIN_BUCKETS), count(0) {}
    // копирование, присваивание и удаление писать не нужно:
    // их делают DynamicArray и LinkedList (у обоих есть копирование и деструктор)

    void Insert(const T &item) {
        LinkedList<T> &bucket = buckets.GetRef(BucketIndex(item));
        if (IndexInBucket(bucket, item) != -1)
            throw AlreadyExists("такой элемент уже есть в таблице");
        bucket.Append(item);
        count++;
        // в среднем больше 1 элемента на корзину — корзин вдвое больше
        if (count > buckets.GetSize())
            Rebuild(buckets.GetSize() * 2);
    }

    bool Contains(const T &item) const {
        const LinkedList<T> &bucket = buckets.GetRef(BucketIndex(item));
        return IndexInBucket(bucket, item) != -1;
    }

    // если элемент есть — кладём то, что лежит в таблице, в found и возвращаем true.
    // нужен словарю: найти запись по ключу и достать из неё значение.
    // (указатель, как у дерева, не вернуть — LinkedList отдаёт элементы только копиями)
    bool TryFind(const T &item, T &found) const {
        const LinkedList<T> &bucket = buckets.GetRef(BucketIndex(item));
        for (auto it = bucket.GetIterator(); it.HasNext(); it.Next()) {
            if (equal(it.Current(), item)) {
                found = it.Current();
                return true;
            }
        }
        return false;
    }

    void Remove(const T &item) {
        LinkedList<T> &bucket = buckets.GetRef(BucketIndex(item));
        int index = IndexInBucket(bucket, item);
        if (index == -1)
            throw NotFound("такого элемента нет в таблице");
        bucket.RemoveAt(index);
        count--;
        // меньше 1/4 элемента на корзину — корзин вдвое меньше.
        // порог 1/4, а не 1/2: иначе сразу после расширения одно удаление сжимало бы таблицу обратно
        if (count < buckets.GetSize() / 4 && buckets.GetSize() > MIN_BUCKETS)
            Rebuild(buckets.GetSize() / 2);
    }

    int GetCount() const {
        return count;
    }

    bool IsEmpty() const {
        return count == 0;
    }

    // число корзин — для тестов, меню и GetCapacity словаря
    int GetBucketCount() const {
        return buckets.GetSize();
    }
};

#endif //LABS_HASHTABLE_H
