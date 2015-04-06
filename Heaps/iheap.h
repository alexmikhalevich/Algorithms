#ifndef IHEAP_H
#define IHEAP_H

#include <cstdlib>
#include <iterator>
#include <vector>
#include <limits>
#include "cexception.h"
#include "ccompare.hpp"

template<class T, class CompareFunction>
class IHeap
{
public:
    virtual void Insert(const T& key) = 0;
    virtual T ExtractMin() = 0;
    virtual T GetMin() const = 0;
    virtual void Meld(IHeap<T, CompareFunction>* heap) = 0;
    virtual bool Empty() const = 0;
    virtual ~IHeap(){}
};

#endif // IHEAP_H
