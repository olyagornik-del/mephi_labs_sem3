#ifndef LABS_TREESET_H
#define LABS_TREESET_H

#include <functional> // std::less
#include "../core/AVLTree.h"

// множество на AVL-дереве: элементы без повторов, сам элемент и есть ключ.
// интерфейса ISet нет — реализация одна, переключать не на что (ягни)
template <class T, class Compare = std::less<T>>
class TreeSet {
private:
    AVLTree<T, Compare> tree;

public:
    void Add(const T &item) {
        tree.Insert(item); // уже есть — AlreadyExists из дерева
    }

    bool Contains(const T &item) const {
        return tree.Contains(item);
    }

    void Remove(const T &item) {
        tree.Remove(item); // нет — NotFound из дерева
    }

    int GetCount() const {
        return tree.GetCount();
    }

    bool IsEmpty() const {
        return tree.IsEmpty();
    }
};

#endif //LABS_TREESET_H
