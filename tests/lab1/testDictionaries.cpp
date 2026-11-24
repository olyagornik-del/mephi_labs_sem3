#include <cassert> // assert
#include <cstdio> //printf
#include <string>

#include "../../atd/IDictionary.h"
#include "../../atd/TreeDictionary.h"
#include "../../atd/HashDictionary.h"
#include "testTypes.h" // Point, PointHash

// общие проверки: функция знает только IDictionary, поэтому одинаково
// проверяет обе реализации — так и доска будет работать с любой из них

static void CheckBasic(IDictionary<int, std::string> *Dict) {
    assert(Dict->GetCount() == 0);
    Dict->Add(3, "три");
    Dict->Add(1, "один");
    Dict->Add(2, "два");
    assert(Dict->GetCount() == 3);
    assert(Dict->Get(1) == "один" && Dict->Get(3) == "три");
    assert(Dict->ContainsKey(2) && !Dict->ContainsKey(4));

    Dict->Remove(2);
    assert(!Dict->ContainsKey(2));
    assert(Dict->GetCount() == 2);
}

static void CheckErrors(IDictionary<int, std::string> *Dict) {
    Dict->Add(1, "один");

    bool threw = false;
    try { Dict->Add(1, "уно"); } catch (const AlreadyExists&) { threw = true; }
    assert(threw);
    assert(Dict->Get(1) == "один"); // старое значение не изменилось

    threw = false;
    try { Dict->Get(5); } catch (const NotFound&) { threw = true; }
    assert(threw);

    threw = false;
    try { Dict->Remove(5); } catch (const NotFound&) { threw = true; }
    assert(threw);
}

static void CheckBoard(IDictionary<Point, char> *Board) {
    // как будущая доска: ставим фигуры, отменяем ходы, клетки с отрицательными координатами
    Board->Add(Point{0, 0}, 'X');
    Board->Add(Point{1, 0}, 'O');
    Board->Add(Point{0, 1}, 'X');
    Board->Add(Point{-3, 2}, 'O');
    assert(Board->Get(Point{1, 0}) == 'O');
    assert(Board->Get(Point{0, 1}) == 'X'); // (1,0) и (0,1) — разные клетки
    assert(Board->Get(Point{-3, 2}) == 'O');

    Board->Remove(Point{1, 0}); // отмена хода
    assert(!Board->ContainsKey(Point{1, 0}));
    assert(Board->GetCount() == 3);
}

static void CheckStress(IDictionary<int, int> *Dict) {
    int n = 10000;
    for (int i = 0; i < n; i++)
        Dict->Add((i * 7919) % n, i);
    assert(Dict->GetCount() == n);
    for (int i = 0; i < n; i += 2)
        Dict->Remove(i);
    for (int i = 0; i < n; i++)
        assert(Dict->ContainsKey(i) == (i % 2 == 1));
}

void testDictionariesSameBehaviour() {
    // каждую проверку гоняем на обеих реализациях
    IDictionary<int, std::string> *Tree = new TreeDictionary<int, std::string>();
    IDictionary<int, std::string> *Hash = new HashDictionary<int, std::string>();
    CheckBasic(Tree);
    CheckBasic(Hash);
    delete Tree;
    delete Hash;

    Tree = new TreeDictionary<int, std::string>();
    Hash = new HashDictionary<int, std::string>();
    CheckErrors(Tree);
    CheckErrors(Hash);
    delete Tree;
    delete Hash;

    IDictionary<Point, char> *TreeBoard = new TreeDictionary<Point, char>();
    IDictionary<Point, char> *HashBoard = new HashDictionary<Point, char, PointHash>();
    CheckBoard(TreeBoard);
    CheckBoard(HashBoard);
    delete TreeBoard;
    delete HashBoard;

    IDictionary<int, int> *TreeBig = new TreeDictionary<int, int>();
    IDictionary<int, int> *HashBig = new HashDictionary<int, int>();
    CheckStress(TreeBig);
    CheckStress(HashBig);
    delete TreeBig;
    delete HashBig;

    printf("  [OK] testDictionariesSameBehaviour\n");
}

void testHashDictionaryCapacity() {
    // вместимость хеш-словаря = число корзин, растёт вместе с записями
    HashDictionary<int, int> Dict;
    assert(Dict.GetCapacity() == 8);
    for (int i = 0; i < 9; i++)
        Dict.Add(i, i);
    assert(Dict.GetCapacity() == 16);
    assert(Dict.GetCount() <= Dict.GetCapacity());

    printf("  [OK] testHashDictionaryCapacity\n");
}

void testDictionariesAll() {
    printf("=== Тесты: дерево и хеш через IDictionary ===\n");
    testDictionariesSameBehaviour();
    testHashDictionaryCapacity();
    printf("=== Все тесты пройдены! ===\n\n");
}
