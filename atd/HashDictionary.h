#ifndef LABS_HASHDICTIONARY_H
#define LABS_HASHDICTIONARY_H

#include <functional> // std::hash, std::equal_to
#include "../my_except.h"
#include "../core/HashTable.h"
#include "IDictionary.h"
#include "KeyAndValue.h"

// словарь на хеш-таблице: записи {ключ, значение} раскладываются по корзинам по хешу ключа.
// ключ должен уметь хешироваться (Hash) и сравниваться на равенство (Equal)
template <class K, class V, class Hash = std::hash<K>, class Equal = std::equal_to<K>>
class HashDictionary : public IDictionary<K, V> {
private:
    HashTable<KeyAndValue<K, V>, KeyHash<K, V, Hash>, KeyEqual<K, V, Equal>> table;
    // поиск по ключу идёт через запись-заглушку SearchEntry (зачем она — см. KeyAndValue.h)

public:
    void Add(const K &key, const V &value) override {
        table.Insert(KeyAndValue<K, V>{key, value}); // дубликат ключа — AlreadyExists из таблицы
    }

    V Get(const K &key) const override {
        KeyAndValue<K, V> found = SearchEntry<K, V>(key);
        if (!table.TryFind(SearchEntry<K, V>(key), found))
            throw NotFound("такого ключа нет в словаре");
        return found.value;
    }

    bool ContainsKey(const K &key) const override {
        return table.Contains(SearchEntry<K, V>(key));
    }

    void Remove(const K &key) override {
        table.Remove(SearchEntry<K, V>(key)); // ключа нет — NotFound из таблицы
    }

    int GetCount() const override {
        return table.GetCount();
    }

    // таблица расширяется, когда записей становится больше, чем корзин,
    // поэтому без перестройки влезает ровно столько записей, сколько корзин
    int GetCapacity() const override {
        return table.GetBucketCount();
    }
};

#endif //LABS_HASHDICTIONARY_H
