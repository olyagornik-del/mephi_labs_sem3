#include <cassert> // assert
#include <cstdio> //printf
#include <string>
#include <utility> // std::pair

#include "../../atd/TreeSet.h"

void testTreeSetEmpty() {
    TreeSet<int> Set;
    assert(Set.IsEmpty());
    assert(Set.GetCount() == 0);
    assert(!Set.Contains(1));

    bool threw = false;
    try { Set.Remove(1); } catch (const NotFound&) { threw = true; }
    assert(threw);

    printf("  [OK] testTreeSetEmpty\n");
}

void testTreeSetAddContainsRemove() {
    TreeSet<int> Set;
    Set.Add(5);
    Set.Add(1);
    Set.Add(9);
    assert(Set.GetCount() == 3);
    assert(Set.Contains(1) && Set.Contains(5) && Set.Contains(9));
    assert(!Set.Contains(4));

    Set.Remove(5);
    assert(!Set.Contains(5));
    assert(Set.GetCount() == 2);

    printf("  [OK] testTreeSetAddContainsRemove\n");
}

void testTreeSetDuplicate() {
    // во множестве нет повторов — повторное добавление это ошибка
    TreeSet<std::string> Set;
    Set.Add("кот");
    bool threw = false;
    try { Set.Add("кот"); } catch (const AlreadyExists&) { threw = true; }
    assert(threw);
    assert(Set.GetCount() == 1);

    printf("  [OK] testTreeSetDuplicate\n");
}

void testTreeSetPairs() {
    // множество клеток (как будущие ходы-кандидаты); у std::pair уже есть <
    TreeSet<std::pair<int, int>> Cells;
    Cells.Add({0, 0});
    Cells.Add({-2, 1});
    Cells.Add({1, -2});
    assert(Cells.Contains({-2, 1}));
    assert(!Cells.Contains({1, -1}));
    Cells.Remove({0, 0});
    assert(Cells.GetCount() == 2);

    printf("  [OK] testTreeSetPairs\n");
}

void testTreeSetCopy() {
    TreeSet<int> Set;
    Set.Add(1);
    TreeSet<int> Copy(Set);
    Copy.Add(2);
    assert(!Set.Contains(2) && Copy.Contains(2));

    printf("  [OK] testTreeSetCopy\n");
}

void testTreeSetAll() {
    printf("=== Тесты TreeSet ===\n");
    testTreeSetEmpty();
    testTreeSetAddContainsRemove();
    testTreeSetDuplicate();
    testTreeSetPairs();
    testTreeSetCopy();
    printf("=== Все тесты пройдены! ===\n\n");
}
