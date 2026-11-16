#include <cassert> // assert
#include <cstdio> //printf
#include <string>

#include "../../atd/TreeDictionary.h"

// прообраз будущей координаты Coord из ЛР-2: сравнение сначала по x, потом по y
struct Point {
    int x;
    int y;
    bool operator<(const Point &other) const {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }
};

void testTreeDictionaryEmpty() {
    TreeDictionary<int, int> Dict;
    assert(Dict.GetCount() == 0);
    assert(!Dict.ContainsKey(1));

    // get и remove несуществующего ключа — исключение
    bool threw = false;
    try { Dict.Get(1); } catch (const NotFound&) { threw = true; }
    assert(threw);
    threw = false;
    try { Dict.Remove(1); } catch (const NotFound&) { threw = true; }
    assert(threw);

    printf("  [OK] testTreeDictionaryEmpty\n");
}

void testTreeDictionaryAddGet() {
    TreeDictionary<int, std::string> Dict;
    Dict.Add(3, "три");
    Dict.Add(1, "один");
    Dict.Add(2, "два");
    assert(Dict.GetCount() == 3);

    assert(Dict.Get(1) == "один");
    assert(Dict.Get(2) == "два");
    assert(Dict.Get(3) == "три");
    assert(Dict.ContainsKey(2));
    assert(!Dict.ContainsKey(4));

    printf("  [OK] testTreeDictionaryAddGet\n");
}

void testTreeDictionaryDuplicateKey() {
    // тот же ключ с другим значением — исключение, старое значение остаётся
    TreeDictionary<int, std::string> Dict;
    Dict.Add(1, "один");
    bool threw = false;
    try { Dict.Add(1, "уно"); } catch (const AlreadyExists&) { threw = true; }
    assert(threw);
    assert(Dict.GetCount() == 1);
    assert(Dict.Get(1) == "один");

    printf("  [OK] testTreeDictionaryDuplicateKey\n");
}

void testTreeDictionaryRemove() {
    TreeDictionary<int, std::string> Dict;
    Dict.Add(1, "один");
    Dict.Add(2, "два");
    Dict.Remove(1);
    assert(!Dict.ContainsKey(1));
    assert(Dict.Get(2) == "два");
    assert(Dict.GetCount() == 1);

    // после удаления ключ можно добавить заново с другим значением
    Dict.Add(1, "уно");
    assert(Dict.Get(1) == "уно");

    printf("  [OK] testTreeDictionaryRemove\n");
}

void testTreeDictionaryCapacity() {
    // у дерева вместимость = количество
    TreeDictionary<int, int> Dict;
    assert(Dict.GetCapacity() == 0);
    for (int i = 0; i < 5; i++)
        Dict.Add(i, i);
    assert(Dict.GetCapacity() == 5);
    Dict.Remove(0);
    assert(Dict.GetCapacity() == 4);

    printf("  [OK] testTreeDictionaryCapacity\n");
}

void testTreeDictionaryPointKeys() {
    // как будущая доска: координата -> фигура, в т.ч. отрицательные координаты
    TreeDictionary<Point, char> Board;
    Board.Add(Point{0, 0}, 'X');
    Board.Add(Point{1, 0}, 'O');
    Board.Add(Point{-1, 5}, 'X');
    Board.Add(Point{0, -3}, 'O');

    assert(Board.Get(Point{0, 0}) == 'X');
    assert(Board.Get(Point{-1, 5}) == 'X');
    assert(Board.Get(Point{0, -3}) == 'O');
    // (0,1) и (1,0) — разные клетки
    assert(!Board.ContainsKey(Point{0, 1}));

    Board.Remove(Point{1, 0}); // "отмена хода"
    assert(!Board.ContainsKey(Point{1, 0}));
    assert(Board.GetCount() == 3);

    printf("  [OK] testTreeDictionaryPointKeys\n");
}

void testTreeDictionaryThroughInterface() {
    // так словарём будет пользоваться доска: знает только IDictionary
    IDictionary<std::string, int> *Dict = new TreeDictionary<std::string, int>();
    Dict->Add("кот", 3);
    Dict->Add("арбуз", 5);
    assert(Dict->Get("кот") == 3);
    assert(Dict->ContainsKey("арбуз"));
    assert(Dict->GetCount() == 2);
    delete Dict; // виртуальный деструктор: удалится и дерево внутри

    printf("  [OK] testTreeDictionaryThroughInterface\n");
}

void testTreeDictionaryCopy() {
    // копия независима (копируется дерево внутри)
    TreeDictionary<int, int> Dict;
    Dict.Add(1, 10);
    TreeDictionary<int, int> Copy(Dict);
    Copy.Remove(1);
    Copy.Add(2, 20);
    assert(Dict.ContainsKey(1) && !Dict.ContainsKey(2));
    assert(!Copy.ContainsKey(1) && Copy.Get(2) == 20);

    printf("  [OK] testTreeDictionaryCopy\n");
}

void testTreeDictionaryStress() {
    // 10000 ключей вперемешку, значение = квадрат ключа
    TreeDictionary<int, long long> Dict;
    int n = 10000;
    for (int i = 0; i < n; i++) {
        int key = (i * 7919) % n;
        Dict.Add(key, (long long)key * key);
    }
    assert(Dict.GetCount() == n);
    for (int i = 0; i < n; i++)
        assert(Dict.Get(i) == (long long)i * i);

    for (int i = 0; i < n; i += 2)
        Dict.Remove(i);
    assert(Dict.GetCount() == n / 2);
    for (int i = 0; i < n; i++)
        assert(Dict.ContainsKey(i) == (i % 2 == 1));

    printf("  [OK] testTreeDictionaryStress\n");
}

void testTreeDictionaryAll() {
    printf("=== Тесты TreeDictionary ===\n");
    testTreeDictionaryEmpty();
    testTreeDictionaryAddGet();
    testTreeDictionaryDuplicateKey();
    testTreeDictionaryRemove();
    testTreeDictionaryCapacity();
    testTreeDictionaryPointKeys();
    testTreeDictionaryThroughInterface();
    testTreeDictionaryCopy();
    testTreeDictionaryStress();
    printf("=== Все тесты пройдены! ===\n\n");
}
