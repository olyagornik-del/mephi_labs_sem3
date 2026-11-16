#ifndef LABS_TREEDICTIONARY_H
#define LABS_TREEDICTIONARY_H

#include <functional> // std::less
#include "../my_except.h"
#include "../core/AVLTree.h"
#include "IDictionary.h"
#include "KeyAndValue.h"

// словарь на AVL-дереве: в узлах лежат записи {ключ, значение}, упорядоченные по ключу.
// ключ должен уметь сравниваться (Compare, по умолчанию key1 < key2)
template <class K, class V, class Compare = std::less<K>>
class TreeDictionary : public IDictionary<K, V> {
private:
    AVLTree<KeyAndValue<K, V>, KeyLess<K, V, Compare>> tree;
    // поиск по ключу идёт через запись-заглушку SearchEntry (зачем она — см. KeyAndValue.h)

public:
    void Add(const K &key, const V &value) override {
        tree.Insert(KeyAndValue<K, V>{key, value}); // дубликат ключа — AlreadyExists из дерева
    }

    V Get(const K &key) const override {
        const KeyAndValue<K, V> *found = tree.Find(SearchEntry<K, V>(key));
        if (found == nullptr)
            throw NotFound("такого ключа нет в словаре");
        return found->value;
    }

    bool ContainsKey(const K &key) const override {
        return tree.Contains(SearchEntry<K, V>(key));
    }

    void Remove(const K &key) override {
        tree.Remove(SearchEntry<K, V>(key)); // ключа нет — NotFound из дерева
    }

    int GetCount() const override {
        return tree.GetCount();
    }

    // у дерева нет заранее выделенного места: узел создаётся при вставке,
    // поэтому всё выделенное всегда занято — вместимость равна количеству
    int GetCapacity() const override {
        return tree.GetCount();
    }
};

#endif //LABS_TREEDICTIONARY_H
