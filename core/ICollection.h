#ifndef LAB3_ICOLLECTION_H
#define LAB3_ICOLLECTION_H

template<class T>
class ICollection {
public:
    virtual T Get(int index) const = 0;
    virtual int GetCount() const = 0;
    virtual ICollection<T>* Clone() const = 0;

    virtual ~ICollection() = default;
};

#endif //LAB3_ICOLLECTION_H