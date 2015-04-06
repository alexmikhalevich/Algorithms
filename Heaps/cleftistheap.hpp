#ifndef CLEFTISTHEAP_H
#define CLEFTISTHEAP_H

#include "iheap.h"

template<class T>
struct LeftistNode
{
    T value;
    int rank;
    LeftistNode<T>* leftNode;
    LeftistNode<T>* rightNode;
    LeftistNode(const T& val, LeftistNode<T>* ln = NULL, LeftistNode<T>* rn = NULL, int r = 0) : value(val), rank(r),
        leftNode(ln), rightNode(rn) {}
};

template<class T, class CompareFunction>
class CLeftistHeap : public virtual IHeap<T, CompareFunction>
{
private:
    CCompare<T, CompareFunction>* lh_compare;
    LeftistNode<T>* lh_rootNode;
    LeftistNode<T>* _Merge(LeftistNode<T>* node_1, LeftistNode<T>* node_2);
public:
    explicit CLeftistHeap(const CompareFunction compFunc);
    ~CLeftistHeap();
    virtual void Insert(const T& key);
    virtual T ExtractMin();
    virtual void Meld(IHeap<T, CompareFunction>* heap);
    virtual T GetMin() const;
    virtual bool Empty() const;
};

template<class T, class CompareFunction>
CLeftistHeap<T, CompareFunction>::CLeftistHeap(const CompareFunction compFunc)
{
    lh_rootNode = NULL;
    lh_compare = new CCompare<T, CompareFunction>(compFunc);
}

template<class T, class CompareFunction>
CLeftistHeap<T, CompareFunction>::~CLeftistHeap()
{
    delete lh_compare;
    delete lh_rootNode;
}

template<class T, class CompareFunction>
bool CLeftistHeap<T, CompareFunction>::Empty() const
{
    return (lh_rootNode == NULL) ? true : false;
}

template<class T, class CompareFunction>
void CLeftistHeap<T, CompareFunction>::Insert(const T& key)
{
    lh_rootNode = _Merge(new LeftistNode<T>(key), lh_rootNode);
}

template<class T, class CompareFunction>
T CLeftistHeap<T, CompareFunction>::ExtractMin()
{
    T minValue = lh_rootNode->value;
    lh_rootNode = _Merge(lh_rootNode->leftNode, lh_rootNode->rightNode);
    return minValue;
}

template<class T, class CompareFunction>
void CLeftistHeap<T, CompareFunction>::Meld(IHeap<T, CompareFunction>* heap)
{
    CLeftistHeap<T, CompareFunction>* leftistHeap = dynamic_cast<CLeftistHeap<T, CompareFunction>* >(heap);
    if(leftistHeap == NULL)
        throw CExEmptyHeap();

    lh_rootNode = _Merge(lh_rootNode, leftistHeap->lh_rootNode);
}

template<class T, class CompareFunction>
T CLeftistHeap<T, CompareFunction>::GetMin() const
{
    return lh_rootNode->value;
}

template<class T, class CompareFunction>
LeftistNode<T>* CLeftistHeap<T, CompareFunction>::_Merge(LeftistNode<T>* root_1, LeftistNode<T>* root_2)
{
    LeftistNode<T>* node_1 = root_1;
    LeftistNode<T>* node_2 = root_2;

    if(node_1 == NULL)
        return node_2;
    if(node_2 == NULL)
        return node_1;

    if(lh_compare->Greater(node_1->value, node_2->value))
    {
        LeftistNode<T>* tempNode = node_1;
        node_1 = node_2;
        node_2 = tempNode;
    }

    /*
     *the right branch is the shortest and not longer than log(n)
     */
    node_1->rightNode = _Merge(node_1->rightNode, node_2);

    /*
     *check if current heap is leftist
     *if not, make it leftist
     */
    if(node_1->leftNode == NULL)
    {
        node_1->leftNode = node_1->rightNode;
        node_1->rightNode = NULL;
    }
    else
    {
        if(node_1->leftNode->rank < node_1->rightNode->rank)
        {
            LeftistNode<T> *tempNode = node_1->leftNode;
            node_1->leftNode = node_1->rightNode;
            node_1->rightNode = tempNode;
        }

        /*
         *recalculate the distance to the nearest free position
         */
        node_1->rank = std::min(node_1->rightNode->rank, node_1->leftNode->rank) + 1;
    }

    return node_1;
}

#endif // CLEFTISTHEAP_H
