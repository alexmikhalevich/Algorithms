#ifndef CSKEWHEAP_H
#define CSKEWHEAP_H

#include "iheap.h"

template<class T>
struct SkewNode
{
    T key;
    SkewNode<T>* leftNode;
    SkewNode<T>* rightNode;
    SkewNode(const T& k, SkewNode<T>* l = NULL, SkewNode<T>* r = NULL) : key(k), leftNode(l), rightNode(r) {}
};

template<class T, class CompareFunction>
class CSkewHeap : public virtual IHeap<T, CompareFunction>
{
private:
    CCompare<T, CompareFunction>* sh_compare;
    SkewNode<T>* sh_rootNode;
    SkewNode<T>* _Merge(SkewNode<T>* root_1, SkewNode<T>* root_2);
public:
    explicit CSkewHeap(const CompareFunction compFunc);
    ~CSkewHeap();
    virtual void Insert(const T& key);
    virtual T GetMin() const;
    virtual T ExtractMin();
    virtual void Meld(IHeap<T, CompareFunction>* heap);
    virtual bool Empty() const;
};

template<class T, class CompareFunction>
CSkewHeap<T, CompareFunction>::CSkewHeap(const CompareFunction compFunc)
{
    sh_rootNode = NULL;
    sh_compare = new CCompare<T, CompareFunction>(compFunc);
}

template<class T, class CompareFunction>
CSkewHeap<T, CompareFunction>::~CSkewHeap()
{
    delete sh_compare;
    delete sh_rootNode;
}

template<class T, class CompareFunction>
bool CSkewHeap<T, CompareFunction>::Empty() const
{
    return (sh_rootNode == NULL) ? true : false;
}

template<class T, class CompareFunction>
void CSkewHeap<T, CompareFunction>::Insert(const T& key)
{
    SkewNode<T>* head = new SkewNode<T>(key);
    sh_rootNode = _Merge(sh_rootNode, head);
}

template<class T, class CompareFunction>
void CSkewHeap<T, CompareFunction>::Meld(IHeap<T, CompareFunction>* heap)
{
    CSkewHeap<T, CompareFunction>* skewHeap = dynamic_cast<CSkewHeap<T, CompareFunction>* >(heap);
    if(skewHeap == NULL)
        throw CExEmptyHeap();

    sh_rootNode = _Merge(sh_rootNode, skewHeap->sh_rootNode);
    return;
}

template<class T, class CompareFunction>
T CSkewHeap<T, CompareFunction>::ExtractMin()
{
    T minValue = sh_rootNode->key;
    sh_rootNode = _Merge(sh_rootNode->leftNode, sh_rootNode->rightNode);
    return minValue;
}

template<class T, class CompareFunction>
T CSkewHeap<T, CompareFunction>::GetMin() const
{
    return sh_rootNode->key;
}

template<class T, class CompareFunction>
SkewNode<T>* CSkewHeap<T, CompareFunction>::_Merge(SkewNode<T>* root_1, SkewNode<T>* root_2)
{
    SkewNode<T>* firstRoot = root_1;
    SkewNode<T>* secondRoot = root_2;

    if(firstRoot == NULL)
        return secondRoot;

    else if(secondRoot == NULL)
        return firstRoot;

    if(sh_compare->Less(firstRoot->key, secondRoot->key))
    {
        SkewNode<T>* tempHeap = firstRoot->rightNode;
        firstRoot->rightNode = firstRoot->leftNode;
        firstRoot->leftNode = _Merge(secondRoot, tempHeap);
        return firstRoot;
    }
    else
    {
        SkewNode<T>* tempHeap = secondRoot->rightNode;
        secondRoot->rightNode = secondRoot->leftNode;
        secondRoot->leftNode = _Merge(firstRoot, tempHeap);
        return secondRoot;
    }
}

#endif // CSKEWHEAP_H
