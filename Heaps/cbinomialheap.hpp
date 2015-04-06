#ifndef CBINOMIALQUEUE_H
#define CBINOMIALQUEUE_H

#include "iheap.h"

template<class T>
struct BinomialNode
{
    T value;
    int degree;
    BinomialNode<T>* parent;
    BinomialNode<T>* leftChild;
    BinomialNode<T>* rightSibling;
    BinomialNode(const T& val, BinomialNode<T>* p = NULL, BinomialNode<T>* lc = NULL, BinomialNode<T>* rs = NULL) : value(val), degree(0),
        parent(p), leftChild(lc), rightSibling(rs){}
};

template<class T, class CompareFunction>
class CBinomialHeap : public virtual IHeap<T, CompareFunction>
{
private:
    BinomialNode<T>* bh_rootNode;
    BinomialNode<T>* bh_rootNodeReversed;
    CCompare<T, CompareFunction>* bh_compare;
    BinomialNode<T>* _MergeRootLists(BinomialNode<T>* firstRootList, BinomialNode<T>* secondRootList);
    BinomialNode<T>* _UnionBinomialHeaps(BinomialNode<T>* firstRootList, BinomialNode<T>* secondRootList);
    void _LinkTrees(BinomialNode<T>* firstTree, BinomialNode<T>* secondTree);
    void _RevertList(BinomialNode<T>* node);


public:
    explicit CBinomialHeap(const CompareFunction compFunc);
    virtual ~CBinomialHeap();
    virtual void Insert(const T& key);
    virtual void Meld(IHeap<T, CompareFunction>* heap);
    virtual T ExtractMin();
    virtual T GetMin() const;
    virtual bool Empty() const;
};

template<class T, class CompareFunction>
CBinomialHeap<T, CompareFunction>::CBinomialHeap(const CompareFunction compFunc)
{
    bh_rootNode = NULL;
    bh_rootNodeReversed = NULL;
    bh_compare = new CCompare<T, CompareFunction>(compFunc);
}

template<class T, class CompareFunction>
CBinomialHeap<T, CompareFunction>::~CBinomialHeap()
{
    delete bh_rootNode;
    delete bh_rootNodeReversed;
    delete bh_compare;
}

template<class T, class CompareFunction>
bool CBinomialHeap<T, CompareFunction>::Empty() const
{
    return (bh_rootNode == NULL) ? true : false;
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::_RevertList(BinomialNode<T>* node)
{
    if(node->rightSibling != NULL)
    {
        _RevertList(node);
        node->rightSibling->rightSibling = node;
    }
    else
        bh_rootNodeReversed = node;
}

/*
 *makes the first tree child of the second tree
 */
template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::_LinkTrees(BinomialNode<T>* firstTree, BinomialNode<T>* secondTree)
{
    firstTree->parent = secondTree;
    firstTree->rightSibling = secondTree->leftChild;
    secondTree->leftChild = firstTree;
    ++(secondTree->degree);
}

/*
 *merges two root lists, keeping the roots in order of the increasing degrees
 */
template<class T, class CompareFunction>
BinomialNode<T>* CBinomialHeap<T, CompareFunction>::_MergeRootLists(BinomialNode<T>* firstRootList,
                                                                    BinomialNode<T>* secondRootList)
{
    if(firstRootList == NULL)
        return secondRootList;
    else if(secondRootList == NULL)
        return firstRootList;

    BinomialNode<T>* resRootList = NULL;
    BinomialNode<T>* curFirstRootList = firstRootList;
    BinomialNode<T>* curSecondRootList = secondRootList;

    if(curFirstRootList->degree <= curSecondRootList->degree)
        resRootList = curFirstRootList;
    else if(curFirstRootList->degree > curSecondRootList->degree)
        resRootList = curSecondRootList;

    while(curFirstRootList != NULL && curSecondRootList != NULL)
    {
        if(curFirstRootList->degree < curSecondRootList->degree)
            curFirstRootList = curFirstRootList->rightSibling;
        else if(curFirstRootList->degree == curSecondRootList->degree)
        {
            BinomialNode<T>* tempNode = curFirstRootList->rightSibling;
            curFirstRootList->rightSibling = curSecondRootList;
            curFirstRootList = tempNode;
        }
        else
        {
            BinomialNode<T>* tempNode = curSecondRootList->rightSibling;
            curSecondRootList->rightSibling = curFirstRootList;
            curSecondRootList = tempNode;
        }
    }

    return resRootList;
}

template<class T, class CompareFunction>
BinomialNode<T>* CBinomialHeap<T, CompareFunction>::_UnionBinomialHeaps(BinomialNode<T>* firstRootList,
                                                                        BinomialNode<T>* secondRootList)
{
    BinomialNode<T>* resultList = _MergeRootLists(firstRootList, secondRootList);
    if(resultList == NULL)
        return resultList;

    BinomialNode<T>* prevNode = NULL;
    BinomialNode<T>* node = resultList;
    BinomialNode<T>* nextNode = resultList->rightSibling;

    while(nextNode != NULL)
    {
        if((node->degree != nextNode->degree) ||
                (nextNode->rightSibling != NULL && nextNode->rightSibling->degree == node->degree))
        {
            prevNode = node;
            node = nextNode;
        }
        else
        {
            if(bh_compare->LessEqual(node->value, nextNode->value))
            {
                node->rightSibling = nextNode->rightSibling;
                _LinkTrees(nextNode, node);
            }
            else
            {
                if(prevNode == NULL)
                    resultList = nextNode;
                else
                    prevNode->rightSibling = nextNode;
                _LinkTrees(node, nextNode);
                node = nextNode;
            }
        }

        nextNode = node->rightSibling;
    }

    return resultList;
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::Meld(IHeap<T, CompareFunction>* heap)
{
    CBinomialHeap<T, CompareFunction>* binHeap = dynamic_cast<CBinomialHeap<T, CompareFunction>* >(heap);
    if(binHeap == NULL)
        throw CExEmptyHeap();

    BinomialNode<T>* newRootNode = _UnionBinomialHeaps(bh_rootNode, binHeap->bh_rootNode);
    bh_rootNode = newRootNode;
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::Insert(const T& key)
{
    BinomialNode<T>* node = new BinomialNode<T>(key);

    bh_rootNode = _UnionBinomialHeaps(node, bh_rootNode);
}

/*
 *search for the root x with the smallest value
 */
template<class T, class CompareFunction>
T CBinomialHeap<T, CompareFunction>::ExtractMin()
{
    if(bh_rootNode == NULL)
        throw CExEmptyHeap();

    BinomialNode<T>* root = NULL;
    BinomialNode<T>* prevRoot = NULL;
    BinomialNode<T>* curRoot = bh_rootNode;
    BinomialNode<T>* curPrevRoot = NULL;
    T minValue = std::numeric_limits<T>::max();

    while(curRoot != NULL)
    {
        /*
         *relaxing the current minimum
         */
        if(bh_compare->Less(curRoot->value, minValue))
        {
            minValue = curRoot->value;
            root = curRoot;
            prevRoot = curPrevRoot;
        }
        curPrevRoot = curRoot;
        curRoot = curRoot->rightSibling;
    }

    /*
     *removing the root x from the root list
     */
    if(prevRoot == NULL)
        bh_rootNode = root->rightSibling;
    else
        prevRoot->rightSibling = root->rightSibling;

    BinomialNode<T>* newRootNode;
    curRoot = root->leftChild;
    newRootNode = root->leftChild;
    while(curRoot != NULL)
    {
        curRoot->parent = NULL;
        curRoot = curRoot->rightSibling;
    }
    bh_rootNode = _UnionBinomialHeaps(bh_rootNode, newRootNode);

    return root->value;
}

template<class T, class CompareFunction>
T CBinomialHeap<T, CompareFunction>::GetMin() const
{
    BinomialNode<T>* curRoot = bh_rootNode;
    BinomialNode<T>* curPrevRoot = NULL;
    T minValue = std::numeric_limits<T>::max();

    while(curRoot != NULL)
    {
        if(bh_compare->Less(curRoot->value, minValue))
            minValue = curRoot->value;
        curPrevRoot = curRoot;
        curRoot = curRoot->rightSibling;
    }

    return minValue;
}

#endif // CBINOMIALQUEUE_H
