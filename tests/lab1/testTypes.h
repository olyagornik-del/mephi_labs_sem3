#ifndef LABS_TESTTYPES_H
#define LABS_TESTTYPES_H

#include <cstddef> // std::size_t
#include <functional> // std::hash

// общие типы для тестов.
// Point — прообраз будущей координаты Coord из ЛР-2
struct Point {
    int x;
    int y;
    // для дерева: сначала по x, потом по y
    bool operator<(const Point &other) const {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }
    // для хеш-таблицы
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

// хеш точки: смешиваем x и y, чтобы (1,0) и (0,1) обычно попадали в разные корзины
struct PointHash {
    std::size_t operator()(const Point &p) const {
        return std::hash<int>()(p.x) * 31 + std::hash<int>()(p.y);
    }
};

#endif //LABS_TESTTYPES_H
