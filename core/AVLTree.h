#ifndef LABS_AVLTREE_H
#define LABS_AVLTREE_H

#include <algorithm> // std::max
#include <functional> // std::less
#include "../my_except.h"

// сбалансированное (AVL) бинарное дерево поиска: слева от узла всё меньше него, справа — больше.
// сравнение задаётся параметром Compare:
//   AVLTree<int> t; — обычный порядок (a < b)
//   AVLTree<int, std::greater<int>> t2;    — обратный порядок
// равенство отдельно не нужно: если ни a<b, ни b<a — значит a и b равны

// балансировка: у каждого узла высоты левого и правого поддеревьев отличаются не больше чем на 1,
// поэтому высота всего дерева ~log n и поиск/вставка/удаление идут за O(log n)
template <class T, class Compare = std::less<T>>
class AVLTree {
private:
    struct Node {
        T value;
        Node *left;
        Node *right;
        int height; // сколько узлов на самом длинном пути от этого узла вниз до листа (у корня)
    };
    Node *root;
    int count;
    Compare cmp; // объект-функтор: cmp(a, b) == true, если a "меньше" b
// функтор всм можно вызвать скобками
    // высота с учётом пустого места: у nullptr высота 0
    
    // static => у него нет this, и он не видит полей root, count и cmp.
    //Статический метод принадлежит классу целиком, а не отдельному дереву.
    //Ему доступно только то, что передали в параметрах.
    static int Height(const Node *node) {
        return (node == nullptr) ? 0 : node->height;
    }

    // высота узла = 1 (он сам) + высота более высокого из детей
    static void UpdateHeight(Node *node) {
        node->height = 1 + std::max(Height(node->left), Height(node->right));
    }

    // баланс = высота левого - высота правого, просто число:
    // +2 — перевес влево, -2 — перевес вправо, -1/0/+1 — всё в порядке
    static int BalanceFactor(const Node *node) {
        return Height(node->left) - Height(node->right);
    }

    // правый поворот вокруг y, возвращает новый корень поддерева (x):
    //        y              x
    //       / \            / \
    //      x   C    →     A   y
    //     / \                / \
    //    A   B              B   C
    // порядок A < x < B < y < C не меняется, B переезжает от x к y
    static Node* RotateRight(Node *y) {
        Node *x = y->left;
        Node *b = x->right;
        x->right = y;
        y->left = b;
        UpdateHeight(y); // сначала y — он теперь ниже x
        UpdateHeight(x);
        return x;
    }

    // левый поворот вокруг x — зеркальный правому:
    //      x                  y
    //     / \                / \
    //    A   y      →       x   C
    //       / \            / \
    //      B   C          A   B
    static Node* RotateLeft(Node *x) {
        Node *y = x->right;
        Node *b = y->left;
        y->left = x;
        x->right = b;
        UpdateHeight(x); // сначала x — он теперь ниже y
        UpdateHeight(y);
        return y;
    }

    // чинит ОДИН узел (если он перевешивает), возвращает новый корень его поддерева.
    // вызывается на обратном пути рекурсии, поэтому дети node уже сбалансированы
    static Node* Rebalance(Node *node) {
        UpdateHeight(node);
        int balance = BalanceFactor(node);
        if (balance > 1) { // перевес влево
            // LR: у левого ребёнка перевес вправо — сначала превращаем в LL
            if (BalanceFactor(node->left) < 0)
                node->left = RotateLeft(node->left);
            return RotateRight(node); // LL
        }
        if (balance < -1) { // перевес вправо
            // RL: у правого ребёнка перевес влево — сначала превращаем в RR
            if (BalanceFactor(node->right) > 0)
                node->right = RotateRight(node->right);
            return RotateLeft(node); // RR
        }
        // баланс ребёнка 0 бывает только после удаления — тогда хватает одного поворота,
        // поэтому выше строгие < 0 и > 0
        return node;
    }

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
    // рекурсия (а не цикл), потому что на обратном пути вверх
    // пересчитываем высоты и поворачиваем узлы (Rebalance)
    Node* InsertInto(Node *node, const T &item) {
        if (node == nullptr)
            return new Node{item, nullptr, nullptr, 1}; // нашли пустое место, новый лист высоты 1
        if (cmp(item, node->value))
            node->left = InsertInto(node->left, item);
        else if (cmp(node->value, item))
            node->right = InsertInto(node->right, item);
        else
            throw AlreadyExists("такой элемент уже есть в дереве");
        //исключение летит до того, как мы что-то поменяли — дерево остаётся целым
        return Rebalance(node);
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
            return Rebalance(node);
        }
        if (cmp(node->value, item)) {
            node->right = RemoveFrom(node->right, item);
            return Rebalance(node);
        }
        // нашли удаляемый узел, дальше 3 случая
        // 1) и 2) детей нет или ребёнок один — ставим на место узла его ребёнка (или nullptr).
        // поддерево ребёнка и так сбалансировано, а родителя починит Rebalance уровнем выше
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
        return Rebalance(node);
    }

    // копия поддерева: сначала узел, потом его левое и правое поддеревья
    static Node* CopyNodes(const Node *node) {
        if (node == nullptr)
            return nullptr;
        return new Node{node->value, CopyNodes(node->left), CopyNodes(node->right), node->height};
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
    AVLTree(): root(nullptr), count(0) {}

    // конструктор копирования — своё дерево с теми же значениями (а не общие узлы)
    AVLTree(const AVLTree<T, Compare> &other):
        root(CopyNodes(other.root)), count(other.count), cmp(other.cmp) {}

    AVLTree<T, Compare>& operator=(const AVLTree<T, Compare> &other) {
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

    // высота всего дерева (у пустого 0) — для тестов и меню
    int GetHeight() const {
        return Height(root);
    }

    ~AVLTree() {
        DeleteNodes(root);
    }
};

#endif //LABS_AVLTREE_H
