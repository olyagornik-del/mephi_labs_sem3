#ifndef LABS_BINARYSEARCHTREE_H
#define LABS_BINARYSEARCHTREE_H

#include <functional> // std::less
#include "../my_except.h"

// бинарное дерево поиска: слева от узла всё меньше него, справа — больше.
// сравнение задаётся параметром Compare:
//   BinarySearchTree<int> t; — обычный порядок (a < b)
//   BinarySearchTree<int, std::greater<int>> t2;    — обратный порядок
// равенство отдельно не нужно: если ни a<b, ни b<a — значит a и b равны
template <class T, class Compare = std::less<T>>
class BinarySearchTree {
private:
    struct Node {
        T value;
        Node *left;
        Node *right;
    };
    Node *root;
    int count;
    Compare cmp; // объект-функтор: cmp(a, b) == true, если a "меньше" b

    // спуск от корня: возвращает узел с таким значением или nullptr.
    // через него потом словарь будет доставать значение по ключу (Get)
    Node* FindNode(const T &item) const {
        Node *current = root;
        while (current != nullptr) {
            if (cmp(item, current->value))
                current = current->left; // искомое меньше — налево
            else if (cmp(current->value, item))
                current = current->right; // искомое больше — направо
            else
                return current; // не меньше и не больше — нашли
        }
        return nullptr;
    }

    // вставка в поддерево с корнем node, возвращает новый корень поддерева.
    // рекурсия (а не цикл), потому что в AVL на обратном пути вверх
    // будем пересчитывать высоты и поворачивать узлы
    Node* InsertInto(Node *node, const T &item) {
        if (node == nullptr)
            return new Node{item, nullptr, nullptr}; // нашли пустое место
        if (cmp(item, node->value))
            node->left = InsertInto(node->left, item);
        else if (cmp(node->value, item))
            node->right = InsertInto(node->right, item);
        else
            throw AlreadyExists("такой элемент уже есть в дереве");
        //исключение летит до того, как мы что-то поменяли — дерево остаётся целым
        return node;
    }

    // самый левый (минимальный) узел поддерева
    static Node* MinNode(Node *node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    // удаление из поддерева с корнем node, возвращает новый корень поддерева
    Node* RemoveFrom(Node *node, const T &item) {
        if (node == nullptr)
            throw NotFound("такого элемента нет в дереве");
        if (cmp(item, node->value)) {
            node->left = RemoveFrom(node->left, item);
            return node;
        }
        if (cmp(node->value, item)) {
            node->right = RemoveFrom(node->right, item);
            return node;
        }
        // нашли удаляемый узел, дальше 3 случая
        // 1) и 2) детей нет или ребёнок один — ставим на место узла его ребёнка (или nullptr)
        if (node->left == nullptr) {
            Node *child = node->right;
            delete node;
            return child;
        }
        if (node->right == nullptr) {
            Node *child = node->left;
            delete node;
            return child;
        }
        // 3) двое детей — берём следующее по порядку значение (минимум правого поддерева),
        // кладём его сюда, а сам тот узел удаляем из правого поддерева.
        // у минимума нет левого ребёнка, поэтому там сработает простой случай 1)
        Node *next = MinNode(node->right);
        node->value = next->value;
        // ищем по node->value, а не по next->value: next будет удалён внутри вызова
        node->right = RemoveFrom(node->right, node->value);
        return node;
    }

    // копия поддерева: сначала узел, потом его левое и правое поддеревья
    static Node* CopyNodes(const Node *node) {
        if (node == nullptr)
            return nullptr;
        return new Node{node->value, CopyNodes(node->left), CopyNodes(node->right)};
    }

    // удаление поддерева: сначала дети, потом сам узел (иначе потеряем указатели на детей)
    static void DeleteNodes(Node *node) {
        if (node == nullptr)
            return;
        DeleteNodes(node->left);
        DeleteNodes(node->right);
        delete node;
    }

public:
    //конструктор по умолчанию — пустое дерево
    BinarySearchTree(): root(nullptr), count(0) {}

    // конструктор копирования — своё дерево с теми же значениями (а не общие узлы)
    BinarySearchTree(const BinarySearchTree<T, Compare> &other):
        root(CopyNodes(other.root)), count(other.count), cmp(other.cmp) {}

    BinarySearchTree<T, Compare>& operator=(const BinarySearchTree<T, Compare> &other) {
        if (this == &other) { // t = t;
            return *this;
        }
        Node *copy = CopyNodes(other.root); // сначала копируем, потом удаляем старое
        DeleteNodes(root);
        root = copy;
        count = other.count;
        cmp = other.cmp;
        return *this;
    }

    void Insert(const T &item) {
        root = InsertInto(root, item);
        count++; // сюда дойдём, только если не было исключения
    }

    bool Contains(const T &item) const {
        return FindNode(item) != nullptr;
    }

    void Remove(const T &item) {
        root = RemoveFrom(root, item);
        count--;
    }

    int GetCount() const {
        return count;
    }

    bool IsEmpty() const {
        return count == 0;
    }

    ~BinarySearchTree() {
        DeleteNodes(root);
    }
};

#endif //LABS_BINARYSEARCHTREE_H
