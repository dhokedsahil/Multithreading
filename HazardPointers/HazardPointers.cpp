#include "HazardPointers.h"
#include <unordered_set>

template <class T>
HazardPointersContainer<T>::HazardPointersContainer(int threads, int pointersPerThread)
{
    this->threads = threads;
    HazardPointers = std::vector<HPRecType<T>>(threads, HPRecType<T>(this, pointersPerThread));
}

template <class T>
HPRecType<T>::HPRecType(HazardPointersContainer<T>* baseContainer, int pointersPerThread)
{
    this->pointersPerThread = pointersPerThread;
    this->baseContainer = baseContainer;
    this->pointers = std::vector<HP<T>>(pointersPerThread, HP<T>(this));
}

template <class T>
void HPRecType<T>::scan()
{
    std::vector<T*> temp;
    rlists.swap(temp);
    std::unordered_set<T*> plist;
    //Insert all HPs of all threads to plist
    for(auto const &hpRecType : baseContainer->HazardPointers)
    {
        for(auto const &hp : hpRecType->pointers)
        {
            plist.insert(hp.pointer);
        }
    }

    for(auto &nodeptr : temp)
    {
        if(plist.find(nodeptr) != plist.end())
        {
            rlists.push_back(nodeptr);
        }
        else
        {
            delete nodeptr;
        }
    }
}

template <class T>
HP<T>::HP(HPRecType<T>* baseHPRecType)
{
    this->baseHPRecType = baseHPRecType;
    pointer = NULL;
}

template <class T>
void HP<T>::retireNode()
{
    baseHPRecType->rlists.push_back(pointer);
    pointer = NULL;
    if(baseHPRecType->rlists.size() > baseHPRecType->rlists->threshold)
    {        
        baseHPRecType->scan();
    }
}