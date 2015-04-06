#ifndef CTEST_H
#define CTEST_H

#include "cexception.h"
#include "cbinomialheap.hpp"
#include "cskewheap.hpp"
#include "cleftistheap.hpp"
#include <ctime>
#include <chrono>

#define SMALL_TEST 10
#define MEDIUM_TEST 1000
#define BIG_TEST 100000
#define SMALL_DATA SMALL_TEST * 10
#define MEDIUM_DATA MEDIUM_TEST * 10
#define BIG_DATA BIG_TEST * 10

template<class T, class CompareFunction>
class CTest
{
private:
    virtual void _AddHeap(const T& key) = 0;
    virtual void _Insert(const unsigned int heapIndex, const T& key) = 0;
    virtual void _Meld(const unsigned int firstHeapIndex, const unsigned int secondHeapIndex) = 0;
    virtual T _ExtractMin(const unsigned int heapIndex) = 0;
public:
    virtual void ApplyTest(const int numOfHeaps, const std::vector<T>& values) = 0;
};

template<class T, class CompareFunction>
class CTestBinomialHeap : public virtual CTest<T, CompareFunction>
{
private:
    CompareFunction tbh_compareFunction;
    std::vector<CBinomialHeap<T, CompareFunction>*> tbh_heaps;
    virtual void _AddHeap(const T& key);
    virtual void _Insert(const unsigned int heapIndex, const T& key);
    virtual void _Meld(const unsigned int firstHeapIndex, const unsigned int secondHeapIndex);
    virtual T _ExtractMin(const unsigned int heapIndex);
public:
    CTestBinomialHeap(const CompareFunction compFunc) : tbh_compareFunction(compFunc){}
    virtual ~CTestBinomialHeap();
    virtual void ApplyTest(const int numOfHeaps, const std::vector<T>& values);
};

template<class T, class CompareFunction>
class CTestLeftistHeap : public virtual CTest<T, CompareFunction>
{
private:
    CompareFunction tlh_compareFunction;
    std::vector<CLeftistHeap<T, CompareFunction>*> tlh_heaps;
    virtual void _AddHeap(const T& key);
    virtual void _Insert(const unsigned int heapIndex, const T& key);
    virtual void _Meld(const unsigned int firstHeapIndex, const unsigned int secondHeapIndex);
    virtual T _ExtractMin(const unsigned int heapIndex);
public:
    CTestLeftistHeap(const CompareFunction compFunc) : tlh_compareFunction(compFunc){}
    virtual ~CTestLeftistHeap();
    virtual void ApplyTest(const int numOfHeaps, const std::vector<T>& values);
};

template<class T, class CompareFunction>
class CTestSkewHeap : public virtual CTest<T, CompareFunction>
{
private:
    CompareFunction tsh_compareFunction;
    std::vector<CSkewHeap<T, CompareFunction>*> tsh_heaps;
    virtual void _AddHeap(const T& key);
    virtual void _Insert(const unsigned int heapIndex, const T& key);
    virtual void _Meld(const unsigned int firstHeapIndex, const unsigned int secondHeapIndex);
    virtual T _ExtractMin(const unsigned int heapIndex);
public:
    CTestSkewHeap(const CompareFunction compFunc) : tsh_compareFunction(compFunc){}
    virtual ~CTestSkewHeap();
    virtual void ApplyTest(const int numOfHeaps, const std::vector<T>& values);
};

template<class T, class CompareFunction>
void CTestBinomialHeap<T, CompareFunction>::_AddHeap(const T &key)
{
    CBinomialHeap<T, CompareFunction>* heap = new CBinomialHeap<T, CompareFunction>(tbh_compareFunction);
    heap->Insert(key);
    tbh_heaps.push_back(heap);
}

template<class T, class CompareFunction>
void CTestLeftistHeap<T, CompareFunction>::_AddHeap(const T &key)
{
    CLeftistHeap<T, CompareFunction>* heap = new CLeftistHeap<T, CompareFunction>(tlh_compareFunction);
    heap->Insert(key);
    tlh_heaps.push_back(heap);
}

template<class T, class CompareFunction>
void CTestSkewHeap<T, CompareFunction>::_AddHeap(const T &key)
{
    CSkewHeap<T, CompareFunction>* heap = new CSkewHeap<T, CompareFunction>(tsh_compareFunction);
    heap->Insert(key);
    tsh_heaps.push_back(heap);
}

template<class T, class CompareFunction>
void CTestBinomialHeap<T, CompareFunction>::_Insert(const unsigned int heapIndex, const T& key)
{
    tbh_heaps.at(heapIndex)->Insert(key);
}

template<class T, class CompareFunction>
void CTestLeftistHeap<T, CompareFunction>::_Insert(const unsigned int heapIndex, const T& key)
{
    tlh_heaps.at(heapIndex)->Insert(key);
}

template<class T, class CompareFunction>
void CTestSkewHeap<T, CompareFunction>::_Insert(const unsigned int heapIndex, const T& key)
{
    tsh_heaps.at(heapIndex)->Insert(key);
}

template<class T, class CompareFunction>
void CTestBinomialHeap<T, CompareFunction>::_Meld(const unsigned int firstHeapIndex, const unsigned int secondHeapIndex)
{
    tbh_heaps.at(firstHeapIndex)->Meld(tbh_heaps.at(secondHeapIndex));
    delete tbh_heaps[secondHeapIndex];
    tbh_heaps[secondHeapIndex] = *(tbh_heaps.end() - 1);
    tbh_heaps.erase(tbh_heaps.end() - 1);
}

template<class T, class CompareFunction>
void CTestLeftistHeap<T, CompareFunction>::_Meld(const unsigned int firstHeapIndex, const unsigned int secondHeapIndex)
{
    tlh_heaps.at(firstHeapIndex)->Meld(tlh_heaps.at(secondHeapIndex));
    delete tlh_heaps[secondHeapIndex];
    tlh_heaps[secondHeapIndex] = *(tlh_heaps.end() - 1);
    tlh_heaps.erase(tlh_heaps.end() - 1);
}

template<class T, class CompareFunction>
void CTestSkewHeap<T, CompareFunction>::_Meld(const unsigned int firstHeapIndex, const unsigned int secondHeapIndex)
{
    tsh_heaps.at(firstHeapIndex)->Meld(tsh_heaps.at(secondHeapIndex));
    delete tsh_heaps[secondHeapIndex];
    tsh_heaps[secondHeapIndex] = *(tsh_heaps.end() - 1);
    tsh_heaps.erase(tsh_heaps.end() - 1);
}

template<class T, class CompareFunction>
T CTestBinomialHeap<T, CompareFunction>::_ExtractMin(const unsigned int heapIndex)
{
    return tbh_heaps.at(heapIndex)->ExtractMin();
}

template<class T, class CompareFunction>
T CTestLeftistHeap<T, CompareFunction>::_ExtractMin(const unsigned int heapIndex)
{
    return tlh_heaps.at(heapIndex)->ExtractMin();
}

template<class T, class CompareFunction>
T CTestSkewHeap<T, CompareFunction>::_ExtractMin(const unsigned int heapIndex)
{
    return tsh_heaps.at(heapIndex)->ExtractMin();
}

template<class T, class CompareFunction>
void CTestBinomialHeap<T, CompareFunction>::ApplyTest(const int numOfHeaps, const std::vector<T>& values)
{
    std::srand(time(NULL));

    for(unsigned int i = 0; i < numOfHeaps; ++i)
        tbh_heaps.push_back(new CBinomialHeap<T ,CompareFunction>(tbh_compareFunction));

    try
    {
        time_t time = clock();
        for(unsigned int i = 0; i < values.size(); ++i)
            _Insert(std::rand() % tbh_heaps.size(), values.at(i));
        time = clock() - time;
        std::cout << "Insert(..) done successfully in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

        time = clock();
        for(unsigned int i = 0; i < tbh_heaps.size(); ++i)
        {
            if(!tbh_heaps.at(i)->Empty())
                /*std::cout << "Heap " << i << ": " << */_ExtractMin(i)/* << std::endl*/;
//            else
//                std::cout << "Heap " << i << "is empty" << std::endl;
        }
        time = clock() - time;
        std::cout << "ExtractMin(..) done successfully in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

        const unsigned int firstHeapIndex = rand() % tbh_heaps.size();
        const unsigned int secondHeapIndex = rand() % tbh_heaps.size();

        time = clock();
        _Meld(firstHeapIndex, secondHeapIndex);
        time = clock() - time;
        std::cout << "Heaps " << firstHeapIndex << " and " << secondHeapIndex << " are successfully melded in " <<
                  1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
    }
    catch(CException& e)
    {
        e.PrintException();
    }
}

template<class T, class CompareFunction>
void CTestLeftistHeap<T, CompareFunction>::ApplyTest(const int numOfHeaps, const std::vector<T>& values)
{
    std::srand(time(NULL));

    for(unsigned int i = 0; i < numOfHeaps; ++i)
        tlh_heaps.push_back(new CLeftistHeap<T ,CompareFunction>(tlh_compareFunction));

    try
    {
        time_t time = clock();
        for(unsigned int i = 0; i < values.size(); ++i)
            _Insert(std::rand() % tlh_heaps.size(), values.at(i));
        time = clock() - time;
        std::cout << "Insert(..) done successfully in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

        time = clock();
        for(unsigned int i = 0; i < tlh_heaps.size(); ++i)
        {
            if(!tlh_heaps.at(i)->Empty())
                /*std::cout << "Heap " << i << ": " << */_ExtractMin(i)/* << std::endl*/;
//            else
//                std::cout << "Heap " << i << "is empty" << std::endl;
        }
        time = clock() - time;
        std::cout << "ExtractMin(..) done successfully in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

        const unsigned int firstHeapIndex = rand() % tlh_heaps.size();
        const unsigned int secondHeapIndex = rand() % tlh_heaps.size();

        time = clock();
        _Meld(firstHeapIndex, secondHeapIndex);
        time = clock() - time;
        std::cout << "Heaps " << firstHeapIndex << " and " << secondHeapIndex << " are successfully melded in " <<
                     1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
    }
    catch(CException& e)
    {
        e.PrintException();
    }
}

template<class T, class CompareFunction>
void CTestSkewHeap<T, CompareFunction>::ApplyTest(const int numOfHeaps, const std::vector<T>& values)
{
    std::srand(time(NULL));

    for(unsigned int i = 0; i < numOfHeaps; ++i)
        tsh_heaps.push_back(new CSkewHeap<T ,CompareFunction>(tsh_compareFunction));
    try
    {
        time_t time = clock();
        for(unsigned int i = 0; i < values.size(); ++i)
            _Insert(std::rand() % tsh_heaps.size(), values.at(i));
        time = clock() - time;
        std::cout << "Insert(..) done successfully in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

        time = clock();
        for(unsigned int i = 0; i < tsh_heaps.size(); ++i)
        {
            if(!tsh_heaps.at(i)->Empty())
                /*std::cout << "Heap " << i << ": " << */_ExtractMin(i)/* << std::endl*/;
//            else
//                std::cout << "Heap " << i << "is empty" << std::endl;
        }
        time = clock() - time;
        std::cout << "ExtractMin(..) done successfully in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

        const unsigned int firstHeapIndex = rand() % tsh_heaps.size();
        const unsigned int secondHeapIndex = rand() % tsh_heaps.size();

        time = clock();
        _Meld(firstHeapIndex, secondHeapIndex);
        time = clock() - time;
        std::cout << "Heaps " << firstHeapIndex << " and " << secondHeapIndex << " are successfully melded in " <<
                     1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
    }
    catch(CException& e)
    {
        e.PrintException();
    }
}

template<class T, class CompareFunction>
CTestBinomialHeap<T, CompareFunction>::~CTestBinomialHeap()
{
    for(unsigned int i = 0; i < tbh_heaps.size(); ++i)
        tbh_heaps.at(i)->~CBinomialHeap();
}

template<class T, class CompareFunction>
CTestSkewHeap<T, CompareFunction>::~CTestSkewHeap()
{
    for(unsigned int i = 0; i < tsh_heaps.size(); ++i)
        tsh_heaps.at(i)->~CSkewHeap();
}

template<class T, class CompareFunction>
CTestLeftistHeap<T, CompareFunction>::~CTestLeftistHeap()
{
    for(unsigned int i = 0; i < tlh_heaps.size(); ++i)
        tlh_heaps.at(i)->~CLeftistHeap();
}

#endif // CTEST_H
