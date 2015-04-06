#include "cbinomialheap.h"

template<class T, class CompareFunction>
CBinomialHeap<T, CompareFunction>::CBinomialHeap()
{
    bh_rootNode = NULL;
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::Insert(const T key)
{
    Node<T> node = NULL;
    node.child = NULL;
    node.parent = NULL;
    node.sibling = NULL;
    node.degree = 0;
    node.value = key;
    bh_rootNode = this->UnionNodes(bh_rootNode, node);
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::Merge(CBinomialHeap<T, CompareFunction> &heap_1, CBinomialHeap<T, CompareFunction> &heap_2)
{

}

template<class T, class CompareFunction>
Node<T> CBinomialHeap<T, CompareFunction>::Search(const Node<T> node, const T key) const
{
    Node<T> resultNode = NULL;

    if(node.value == key)
    {
        resultNode = node;
        return resultNode;
    }

    if(node.child != 0 && resultNode == NULL)
        resultNode = this->Search(node.child, key);
    if(node.sibling != 0 && resultNode == NULL)
        resultNode = this->Search(node.sibling, key);
    return resultNode;
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::DecreaseKey(Node<T> &node, const T key, const T value)
{
    Node<T> tempNode, parentNode, currentNode;
    tempNode = this->Search(node, key);

    if(tempNode == NULL)
        throw CExInvalidKey();
    if(value > tempNode.value)
        throw CExInvalidKey();

    tempNode.value = value;
    currentNode = tempNode;
    parentNode = tempNode.parent;

    while(parentNode != NULL && compare.Less(currentNode.value < parentNode.value))
    {
        T tempValue = currentNode.value;
        currentNode.value = parentNode.value;
        parentNode.value = tempValue;

        currentNode = parentNode;
        parentNode = parentNode.parent;
    }
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::Revert(Node<T> &node)
{
    if(node.sibling != NULL)
    {
        this->Revert(node);
        node.sibling.sibling = node;
    }
    else
        bh_rootNodeReversed = node;
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::ExtractMin(Node<T> newHeapRoot)
{
    this->bh_rootNodeReversed = NULL;
    Node<T> siblingNode = NULL, tempNode = NULL, node = newHeapRoot;
    if(newHeapRoot == NULL)
        throw CExNoExtraction();

    T minValue = node.value;
    siblingNode = node;
    while(siblingNode.sibling != NULL)
    {
        if(compare.Less(siblingNode.sibling.value, minValue))
        {
            minValue = siblingNode.sibling.value;
            tempNode = siblingNode;
            node = siblingNode.sibling;
        }
        siblingNode = siblingNode.sibling;
    }

    if(tempNode == NULL && node.sibling == NULL)
        newHeapRoot = NULL;
    else if(tempNode == NULL)
        newHeapRoot = node.sibling;
    else if(tempNode.sibling == NULL)
        tempNode = NULL;
    else
        tempNode.sibling = node.sibling;

    if(node.child != NULL)
    {
        this->Revert(node.child);
        node.child.sibling = NULL;
    }

    this->bh_rootNode = this->UnionNodes(newHeapRoot, this->bh_rootNodeReversed);
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::Delete(const Node<T> &node, const T key, const T minValue)
{
    if(node == NULL)
        throw CExEmptyHeap();
    this->DecreaseKey(node, key, minValue);
    this->ExtractMin(this->bh_rootNode);
}

template<class T, class CompareFunction>
void CBinomialHeap<T, CompareFunction>::Link(Node<T> &parent, Node<T> &child)
{
    child.parent = parent;
    child.sibling = parent.child;
    parent.child = child;
    ++parent.degree;
}

template<class T, class CompareFunction>
Node<T> CBinomialHeap<T, CompareFunction>::UnionNodes(const Node<T> &node_1, const Node<T> &node_2)
{
    Node<T> node = this->MergeNodes(node_1, node_2);
    if(node == NULL)
        return node;

    Node<T> previousNode = NULL, nextNode = node.child, tempNode = node;

    while(nextNode != NULL)
    {
        if(tempNode.degree != nextNode.degree || (nextNode.sibling != NULL &&
                                              nextNode.sibling.degree == tempNode.degree))
        {
            previousNode = tempNode;
            tempNode = nextNode;
        }
        else
        {
            if(compare.LessEqual(tempNode.value, nextNode.value))
            {
                tempNode.sibling = nextNode.sibling;
                this->Link(nextNode, tempNode);
            }
            else
            {
                if(previousNode == NULL)
                    node = nextNode;
                else
                    previousNode.sibling = nextNode;
                this->Link(tempNode, nextNode);
                tempNode = nextNode;
            }
        }
        nextNode = tempNode.sibling;
    }

    return node;
}

template<class T, class CompareFunction>
Node<T> CBinomialHeap<T, CompareFunction>::MergeNodes(const Node<T> &node_1, const Node<T> &node_2)
{
    Node<T> nodeY = node_1;
    Node<T> nodeZ = node_2;
    Node<T> nodeA = NULL;
    Node<T> nodeB = NULL;
    Node<T> resultNode = NULL;

    if(nodeY != NULL)
    {
        if(nodeZ != NULL)
        {
            if(nodeY.degree <= nodeZ.degree)
                resultNode = nodeY;
            else
                resultNode = nodeZ;
        }
        else
            resultNode = nodeY;
    }
    else
        resultNode = nodeZ;

    while(nodeY != NULL && nodeZ != NULL)
    {
        if(nodeY.degree < nodeZ.degree)
            nodeY = nodeY.sibling;
        else if(nodeY.degree == nodeZ.degree)
        {
            nodeA = nodeY.sibling;
            nodeY.sibling = nodeZ;
            nodeY = nodeA;
        }
        else
        {
            nodeB = nodeZ.sibling;
            nodeZ.sibling = nodeY;
            nodeZ = nodeB;
        }
    }

    return resultNode;
}
