#ifndef LABS_IDICTIONARY_H
#define LABS_IDICTIONARY_H

// интерфейс словаря (ключ -> значение): только список того, что словарь умеет, без кода.
// реализации: TreeDictionary (AVL-дерево), HashDictionary (хеш-таблица).
// кто пользуется словарём через IDictionary*, не знает, что у него внутри — так и переключаемся
template <class K, class V>
class IDictionary {
public:
    // ключ уже есть — AlreadyExists
    virtual void Add(const K &key, const V &value) = 0;
    // ключа нет — NotFound
    virtual V Get(const K &key) const = 0;
    virtual bool ContainsKey(const K &key) const = 0;
    // ключа нет — NotFound
    virtual void Remove(const K &key) = 0;

    // сколько записей лежит сейчас
    virtual int GetCount() const = 0;
    // сколько записей влезет без перестройки внутренней структуры
    virtual int GetCapacity() const = 0;

    // виртуальный, чтобы delete через IDictionary* удалил и то, что внутри реализации
    virtual ~IDictionary() = default;
};

#endif //LABS_IDICTIONARY_H
