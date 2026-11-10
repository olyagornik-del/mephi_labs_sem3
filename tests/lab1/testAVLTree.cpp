#include <cassert> // assert
#include <cmath> // log2
#include <cstdio> //printf
#include <functional> // std::greater
#include <string>

#include "../../core/AVLTree.h"

// теория: высота AVL-дерева из n узлов не больше ~1.44 * log2(n + 2)
static bool HeightIsAVL(int height, int n) {
    return height <= 1.45 * std::log2(n + 2);
}

// дерево для большинства тестов:
//          50
//        /    \
//      30      70
//     /  \    /  \
//   20   40  60   80
static void FillSample(AVLTree<int> &Tree) {
    int items[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++)
        Tree.Insert(items[i]);
}

void testAVLEmpty() {
    AVLTree<int> Tree;
    assert(Tree.IsEmpty());
    assert(Tree.GetCount() == 0);
    assert(!Tree.Contains(5));

    // удаление из пустого — исключение
    bool threw = false;
    try { Tree.Remove(5); } catch (const NotFound&) { threw = true; }
    assert(threw);

    printf("  [OK] testAVLEmpty\n");
}

void testAVLInsertContains() {
    AVLTree<int> Tree;
    FillSample(Tree);
    assert(Tree.GetCount() == 7);
    assert(!Tree.IsEmpty());

    int present[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++)
        assert(Tree.Contains(present[i]));

    // меньше минимума, больше максимума, между узлами
    assert(!Tree.Contains(10));
    assert(!Tree.Contains(90));
    assert(!Tree.Contains(45));

    printf("  [OK] testAVLInsertContains\n");
}

void testAVLDuplicate() {
    AVLTree<int> Tree;
    FillSample(Tree);

    // повторная вставка — исключение, а дерево не меняется
    bool threw = false;
    try { Tree.Insert(40); } catch (const AlreadyExists&) { threw = true; }
    assert(threw);
    assert(Tree.GetCount() == 7);
    assert(Tree.Contains(40));

    printf("  [OK] testAVLDuplicate\n");
}

void testAVLRemoveLeaf() {
    // случай 1: у узла нет детей
    AVLTree<int> Tree;
    FillSample(Tree);
    Tree.Remove(20);
    assert(!Tree.Contains(20));
    assert(Tree.GetCount() == 6);
    assert(Tree.Contains(30) && Tree.Contains(40));

    printf("  [OK] testAVLRemoveLeaf\n");
}

void testAVLRemoveOneChild() {
    // случай 2: у узла один ребёнок (сначала убираем 20, тогда у 30 остаётся только 40)
    AVLTree<int> Tree;
    FillSample(Tree);
    Tree.Remove(20);
    Tree.Remove(30);
    assert(!Tree.Contains(30));
    assert(Tree.Contains(40)); // ребёнок поднялся на место 30
    assert(Tree.Contains(50));
    assert(Tree.GetCount() == 5);

    printf("  [OK] testAVLRemoveOneChild\n");
}

void testAVLRemoveTwoChildren() {
    // случай 3: у узла двое детей, в т.ч. корень
    AVLTree<int> Tree;
    FillSample(Tree);
    Tree.Remove(30); // на место 30 встанет 40
    assert(!Tree.Contains(30));
    assert(Tree.Contains(20) && Tree.Contains(40));

    Tree.Remove(50); // корень, на его место встанет 60
    assert(!Tree.Contains(50));
    int rest[] = {20, 40, 60, 70, 80};
    for (int i = 0; i < 5; i++)
        assert(Tree.Contains(rest[i]));
    assert(Tree.GetCount() == 5);

    printf("  [OK] testAVLRemoveTwoChildren\n");
}

void testAVLRemoveAll() {
    // удаляем всё по одному — дерево снова пустое и снова работает
    AVLTree<int> Tree;
    FillSample(Tree);
    int items[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++)
        Tree.Remove(items[i]);
    assert(Tree.IsEmpty());

    bool threw = false;
    try { Tree.Remove(50); } catch (const NotFound&) { threw = true; }
    assert(threw);

    Tree.Insert(1);
    assert(Tree.Contains(1) && Tree.GetCount() == 1);

    printf("  [OK] testAVLRemoveAll\n");
}

void testAVLCopy() {
    // копия независима: меняем одно дерево — другое не трогается
    AVLTree<int> Tree;
    FillSample(Tree);

    AVLTree<int> Copy(Tree);
    Copy.Remove(50);
    Copy.Insert(55);
    assert(Tree.Contains(50) && !Tree.Contains(55));
    assert(!Copy.Contains(50) && Copy.Contains(55));

    AVLTree<int> Assigned;
    Assigned.Insert(1);
    Assigned = Tree; // старое содержимое (1) должно удалиться
    assert(!Assigned.Contains(1));
    assert(Assigned.GetCount() == 7);
    Assigned = Assigned; // присваивание самому себе ничего не ломает
    assert(Assigned.GetCount() == 7 && Assigned.Contains(80));

    printf("  [OK] testAVLCopy\n");
}

void testAVLOtherTypes() {
    // шаблон работает с разными типами
    AVLTree<std::string> Words;
    Words.Insert("кот");
    Words.Insert("арбуз");
    Words.Insert("якорь");
    assert(Words.Contains("арбуз") && !Words.Contains("собака"));

    AVLTree<double> Numbers;
    Numbers.Insert(2.5);
    Numbers.Insert(-1.25);
    assert(Numbers.Contains(-1.25) && !Numbers.Contains(0.0));

    // обратный порядок через компаратор
    AVLTree<int, std::greater<int>> Reverse;
    Reverse.Insert(1);
    Reverse.Insert(3);
    Reverse.Insert(2);
    Reverse.Remove(3);
    assert(Reverse.Contains(1) && Reverse.Contains(2) && !Reverse.Contains(3));

    printf("  [OK] testAVLOtherTypes\n");
}

void testAVLStress() {
    // вперемешку вставляем 1000 чисел, удаляем чётные, проверяем остальные
    AVLTree<int> Tree;
    int n = 1000;
    for (int i = 0; i < n; i++)
        Tree.Insert((i * 7919) % n); // 7919 простое, поэтому получаются все числа 0..999 вперемешку
    assert(Tree.GetCount() == n);

    for (int i = 0; i < n; i += 2)
        Tree.Remove(i);
    assert(Tree.GetCount() == n / 2);
    for (int i = 0; i < n; i++)
        assert(Tree.Contains(i) == (i % 2 == 1));

    printf("  [OK] testAVLStress\n");
}

void testAVLRotations() {
    // 4 случая перекоса на трёх элементах: после поворота высота 2 (корень + два ребёнка),
    // без балансировки была бы палка высоты 3
    int orders[4][3] = {
        {3, 2, 1}, // LL — один правый поворот
        {1, 2, 3}, // RR — один левый поворот
        {3, 1, 2}, // LR — левый вокруг 1, потом правый вокруг 3
        {1, 3, 2}, // RL — правый вокруг 3, потом левый вокруг 1
    };
    for (int i = 0; i < 4; i++) {
        AVLTree<int> Tree;
        for (int j = 0; j < 3; j++)
            Tree.Insert(orders[i][j]);
        assert(Tree.GetHeight() == 2);
        assert(Tree.Contains(1) && Tree.Contains(2) && Tree.Contains(3));
    }

    printf("  [OK] testAVLRotations\n");
}

void testAVLHeight() {
    AVLTree<int> Tree;
    assert(Tree.GetHeight() == 0); // пустое
    Tree.Insert(10);
    assert(Tree.GetHeight() == 1); // один лист

    // FillSample даёт идеальное дерево из 7 узлов — высота 3
    AVLTree<int> Sample;
    FillSample(Sample);
    assert(Sample.GetHeight() == 3);

    printf("  [OK] testAVLHeight\n");
}

void testAVLSortedInput() {
    // числа подряд — худший случай для дерева без балансировки (палка высоты n,
    // рекурсия переполнила бы стек). AVL остаётся низким
    int n = 100000;
    AVLTree<int> Up;
    for (int i = 0; i < n; i++)
        Up.Insert(i);
    assert(Up.GetCount() == n);
    assert(HeightIsAVL(Up.GetHeight(), n));

    AVLTree<int> Down;
    for (int i = n - 1; i >= 0; i--)
        Down.Insert(i);
    assert(HeightIsAVL(Down.GetHeight(), n));

    // всё на месте
    for (int i = 0; i < n; i += 997)
        assert(Up.Contains(i) && Down.Contains(i));

    printf("  [OK] testAVLSortedInput\n");
}

void testAVLRemoveRebalance() {
    //     2              3
    //    / \            / \
    //   1   3     →    2   4
    //        \
    //         4
    // удаляем 1 — у 2 перевес вправо (-2), нужен левый поворот
    AVLTree<int> Tree;
    int items[] = {2, 1, 3, 4};
    for (int i = 0; i < 4; i++)
        Tree.Insert(items[i]);
    assert(Tree.GetHeight() == 3);
    Tree.Remove(1);
    assert(Tree.GetHeight() == 2);
    assert(Tree.Contains(2) && Tree.Contains(3) && Tree.Contains(4));

    //     2               4
    //    / \             / \
    //   1   4     →     2   5
    //      / \           \
    //     3   5           3
    // удаляем 1 — у правого ребёнка (4) баланс 0: этот случай бывает только при удалении
    AVLTree<int> Zero;
    int items2[] = {2, 1, 4, 3, 5};
    for (int i = 0; i < 5; i++)
        Zero.Insert(items2[i]);
    Zero.Remove(1);
    assert(Zero.GetHeight() == 3);
    assert(Zero.GetCount() == 4);
    for (int i = 2; i <= 5; i++)
        assert(Zero.Contains(i));

    // удаляем подряд половину большого дерева — высота остаётся AVL-ной
    AVLTree<int> Big;
    int n = 10000;
    for (int i = 0; i < n; i++)
        Big.Insert(i);
    for (int i = 0; i < n / 2; i++)
        Big.Remove(i);
    assert(Big.GetCount() == n / 2);
    assert(HeightIsAVL(Big.GetHeight(), n / 2));

    printf("  [OK] testAVLRemoveRebalance\n");
}

void testAVLTreeAll() {
    printf("=== Тесты AVLTree ===\n");
    testAVLEmpty();
    testAVLInsertContains();
    testAVLDuplicate();
    testAVLRemoveLeaf();
    testAVLRemoveOneChild();
    testAVLRemoveTwoChildren();
    testAVLRemoveAll();
    testAVLCopy();
    testAVLOtherTypes();
    testAVLStress();
    testAVLRotations();
    testAVLHeight();
    testAVLSortedInput();
    testAVLRemoveRebalance();
    printf("=== Все тесты пройдены! ===\n\n");
}
