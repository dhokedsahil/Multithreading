#ifndef SAHIL_HAZARD_POINTERS
#define SAHIL_HAZARD_POINTERS

#include <vector>

template <class T> class HPRecType;
template <class T> class HP;

template <class T>
class HazardPointersContainer
{
public:
    int threads;//n
    std::vector<HPRecType<T>> HazardPointers;
    HazardPointersContainer(int threads, int pointersPerThread);
};

template <class T>
class HPRecType
{
public:
    int threshold = 50;
    int pointersPerThread;//K
    HazardPointersContainer<T>* baseContainer;
    std::vector<HP<T>> pointers;
    std::vector<T*> rlists;
    HPRecType(HazardPointersContainer<T>* baseContainer, int pointersPerThread);
    void scan();
};

template <class T>
class HP
{
public:
    HPRecType<T>* baseHPRecType;
    T* pointer;
    HP(HPRecType<T>* baseHPRecType);
    void retireNode();
};

#endif