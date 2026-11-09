#include <cassert> // assert
#include <cstdio> //printf
#include <functional> // std::greater
#include <string>

#include "../../core/BinarySearchTree.h"

// дерево для большинства тестов:
//          50
//        /    \
//      30      70
//     /  \    /  \
//   20   40  60   80
static void FillSample(BinarySearchTree<int> &Tree) {
    int items[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++)
        Tree.Insert(items[i]);
}

void testBSTEmpty() {
    BinarySearchTree<int> Tree;
    assert(Tree.IsEmpty());
    assert(Tree.GetCount() == 0);
    assert(!Tree.Contains(5));

    // удаление из пустого — исключение
    bool threw = false;
    try { Tree.Remove(5); } catch (const NotFound&) { threw = true; }
    assert(threw);

    printf("  [OK] testBSTEmpty\n");
}

void testBSTInsertContains() {
    BinarySearchTree<int> Tree;
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

    printf("  [OK] testBSTInsertContains\n");
}

void testBSTDuplicate() {
    BinarySearchTree<int> Tree;
    FillSample(Tree);

    // повторная вставка — исключение, а дерево не меняется
    bool threw = false;
    try { Tree.Insert(40); } catch (const AlreadyExists&) { threw = true; }
    assert(threw);
    assert(Tree.GetCount() == 7);
    assert(Tree.Contains(40));

    printf("  [OK] testBSTDuplicate\n");
}

void testBSTRemoveLeaf() {
    // случай 1: у узла нет детей
    BinarySearchTree<int> Tree;
    FillSample(Tree);
    Tree.Remove(20);
    assert(!Tree.Contains(20));
    assert(Tree.GetCount() == 6);
    assert(Tree.Contains(30) && Tree.Contains(40));

    printf("  [OK] testBSTRemoveLeaf\n");
}

void testBSTRemoveOneChild() {
    // случай 2: у узла один ребёнок (сначала убираем 20, тогда у 30 остаётся только 40)
    BinarySearchTree<int> Tree;
    FillSample(Tree);
    Tree.Remove(20);
    Tree.Remove(30);
    assert(!Tree.Contains(30));
    assert(Tree.Contains(40)); // ребёнок поднялся на место 30
    assert(Tree.Contains(50));
    assert(Tree.GetCount() == 5);

    printf("  [OK] testBSTRemoveOneChild\n");
}

void testBSTRemoveTwoChildren() {
    // случай 3: у узла двое детей, в т.ч. корень
    BinarySearchTree<int> Tree;
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

    printf("  [OK] testBSTRemoveTwoChildren\n");
}

void testBSTRemoveAll() {
    // удаляем всё по одному — дерево снова пустое и снова работает
    BinarySearchTree<int> Tree;
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

    printf("  [OK] testBSTRemoveAll\n");
}

void testBSTCopy() {
    // копия независима: меняем одно дерево — другое не трогается
    BinarySearchTree<int> Tree;
    FillSample(Tree);

    BinarySearchTree<int> Copy(Tree);
    Copy.Remove(50);
    Copy.Insert(55);
    assert(Tree.Contains(50) && !Tree.Contains(55));
    assert(!Copy.Contains(50) && Copy.Contains(55));

    BinarySearchTree<int> Assigned;
    Assigned.Insert(1);
    Assigned = Tree; // старое содержимое (1) должно удалиться
    assert(!Assigned.Contains(1));
    assert(Assigned.GetCount() == 7);
    Assigned = Assigned; // присваивание самому себе ничего не ломает
    assert(Assigned.GetCount() == 7 && Assigned.Contains(80));

    printf("  [OK] testBSTCopy\n");
}

void testBSTOtherTypes() {
    // шаблон работает с разными типами
    BinarySearchTree<std::string> Words;
    Words.Insert("кот");
    Words.Insert("арбуз");
    Words.Insert("якорь");
    assert(Words.Contains("арбуз") && !Words.Contains("собака"));

    BinarySearchTree<double> Numbers;
    Numbers.Insert(2.5);
    Numbers.Insert(-1.25);
    assert(Numbers.Contains(-1.25) && !Numbers.Contains(0.0));

    // обратный порядок через компаратор
    BinarySearchTree<int, std::greater<int>> Reverse;
    Reverse.Insert(1);
    Reverse.Insert(3);
    Reverse.Insert(2);
    Reverse.Remove(3);
    assert(Reverse.Contains(1) && Reverse.Contains(2) && !Reverse.Contains(3));

    printf("  [OK] testBSTOtherTypes\n");
}

void testBSTStress() {
    // вперемешку вставляем 1000 чисел, удаляем чётные, проверяем остальные
    BinarySearchTree<int> Tree;
    int n = 1000;
    for (int i = 0; i < n; i++)
        Tree.Insert((i * 7919) % n); // 7919 простое, поэтому получаются все числа 0..999 вперемешку
    assert(Tree.GetCount() == n);

    for (int i = 0; i < n; i += 2)
        Tree.Remove(i);
    assert(Tree.GetCount() == n / 2);
    for (int i = 0; i < n; i++)
        assert(Tree.Contains(i) == (i % 2 == 1));

    printf("  [OK] testBSTStress\n");
}

void testBinarySearchTreeAll() {
    printf("=== Тесты BinarySearchTree ===\n");
    testBSTEmpty();
    testBSTInsertContains();
    testBSTDuplicate();
    testBSTRemoveLeaf();
    testBSTRemoveOneChild();
    testBSTRemoveTwoChildren();
    testBSTRemoveAll();
    testBSTCopy();
    testBSTOtherTypes();
    testBSTStress();
    printf("=== Все тесты пройдены! ===\n\n");
}
