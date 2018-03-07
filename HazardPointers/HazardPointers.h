#ifndef SAHIL_HAZARD_POINTERS
#define SAHIL_HAZARD_POINTERS

#include <vector>

template <class T>
class HazardPointersContainer
{
public:
    int threads;//n
    int pointersPerThread;//K
    HazardPointersContainer(int threads, int pointersPerThread);
    vector<HPRecType<T>> HazardPointers;
};

template <class T>
class HPRecType
{
public:
    HazardPointersContainer<T>* baseContainer;
    vector<HP<T>> pointers;
    vector<T*> rlists;
    void retireNode(T*);
    void scan();
};

template <class T>
class HP
{
public:
    T* pointer;
};

#endif