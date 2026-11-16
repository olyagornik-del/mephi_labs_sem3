#ifndef LABS_KEYANDVALUE_H
#define LABS_KEYANDVALUE_H

#include <cstddef> // std::size_t
#include <functional> // std::less, std::hash, std::equal_to

// одна запись словаря: ключ (по нему ищем) + значение (его получаем).
// дерево хранит в узле один объект T — поэтому ключ и значение складываем в одну структуру
template <class K, class V>
struct KeyAndValue {
    K key;
    V value;
};

// компаратор для дерева из записей: сравнивает ТОЛЬКО ключи, на значение не смотрит.
// поэтому {(1,1), X} и {(1,1), O} для дерева равны — ключ остаётся уникальным
template <class K, class V, class Compare = std::less<K>>
struct KeyLess {
    Compare cmp; // как сравнивать сами ключи (по умолчанию key1 < key2)

    bool operator()(const KeyAndValue<K, V> &a, const KeyAndValue<K, V> &b) const {
        return cmp(a.key, b.key);
    }
};

// хеш записи для хеш-таблицы = хеш её ключа (значение не участвует)
template <class K, class V, class Hash = std::hash<K>>
struct KeyHash {
    Hash hash; // как хешировать сами ключи

    std::size_t operator()(const KeyAndValue<K, V> &entry) const {
        return hash(entry.key);
    }
};

// равенство записей для хеш-таблицы = равенство ключей.
// дерево выводит равенство из "меньше", а у хеш-таблицы "меньше" нет — нужно отдельное ==
template <class K, class V, class Equal = std::equal_to<K>>
struct KeyEqual {
    Equal equal; // как сравнивать сами ключи на равенство (по умолчанию key1 == key2)

    bool operator()(const KeyAndValue<K, V> &a, const KeyAndValue<K, V> &b) const {
        return equal(a.key, b.key);
    }
};

// словарь ищет записи, а у нас есть только ключ — значение мы как раз и хотим узнать.
// поэтому собираем запись-заглушку: ключ настоящий, значение пустое (V{}).
// KeyLess / KeyHash / KeyEqual смотрят только на ключ, так что пустое значение на поиск не влияет.
// из-за этого у V должен быть конструктор по умолчанию
template <class K, class V>
KeyAndValue<K, V> SearchEntry(const K &key) {
    return KeyAndValue<K, V>{key, V{}};
}

#endif //LABS_KEYANDVALUE_H
