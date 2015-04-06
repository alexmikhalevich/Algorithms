#include <assert.h>
#include <iostream>
#include "ctreap.h"

CTreap::CTreap()
{
    tr_root = NULL;
    tr_root->vert_left_child = NULL;
    tr_root->vert_right_child = NULL;
    tr_elements = 1;
}

CTreap::CTreap(Vertice* root, Vertice* leftChild, Vertice* rightChild)
{
    tr_root = root;
    tr_root->vert_left_child = leftChild;
    tr_root->vert_right_child = rightChild;
    tr_elements = 1;
}

CTreap::~CTreap()
{

}

inline void CTreap::Update(Vertice*& vert)
{
    if(!vert)
        return;

    Push(vert);
    vert->vert_leftmost = vert;
    vert->vert_rightmost = vert;

    if(vert->vert_left_child)
        vert->vert_leftmost = vert->vert_left_child->vert_leftmost;
    if(vert->vert_right_child)
        vert->vert_rightmost = vert->vert_right_child->vert_rightmost;

    vert->vert_size = 1 + GetSize(vert->vert_right_child) + GetSize(vert->vert_left_child);
    vert->vert_sum = vert->vert_value + GetSubsegmentSum(vert->vert_left_child) + GetSubsegmentSum(vert->vert_right_child);

    vert->vert_decreasing_length = GetDecreasingSuffixLength(vert->vert_right_child);
    if(GetDecreasingSuffixLength(vert->vert_right_child) == GetSize(vert->vert_right_child))
    {
        if(GetValue(GetLeftmostVert(vert->vert_right_child)) <= vert->vert_value)
        {
            ++(vert->vert_decreasing_length);
            if(GetValue(GetRightmostVert(vert->vert_left_child)) >= vert->vert_value)
                vert->vert_decreasing_length += GetDecreasingSuffixLength(vert->vert_left_child);
        }
    }

    vert->vert_increasing_length = GetIncreasingSuffixLength(vert->vert_left_child);
    if(GetIncreasingSuffixLength(vert->vert_left_child) == GetSize(vert->vert_left_child))
    {
        if(GetValue(GetRightmostVert(vert->vert_left_child)) <= vert->vert_value)
        {
            ++(vert->vert_increasing_length);
            if(GetValue(GetLeftmostVert(vert->vert_right_child)) >= vert->vert_value)
                vert->vert_increasing_length += GetIncreasingSuffixLength(vert->vert_right_child);
        }
    }
}

inline std::size_t CTreap::GetSize(Vertice* vert)
{
    return (vert ? vert->vert_size : 0);
}

inline long long CTreap::GetSubsegmentSum(Vertice* vert)
{
    return (vert ? vert->vert_sum : 0);
}

inline std::size_t CTreap::GetDecreasingSuffixLength(Vertice* vert)
{
    return (vert ? (vert->vert_reversed ? vert->vert_increasing_length : vert->vert_decreasing_length) : 0);
}

inline std::size_t CTreap::GetIncreasingSuffixLength(Vertice* vert)
{
    return (vert ? (vert->vert_reversed ? vert->vert_decreasing_length : vert->vert_increasing_length) : 0);
}

inline int CTreap::GetValue(Vertice* vert)
{
    return (vert ? vert->vert_value : std::numeric_limits<int>::min());
}

inline Vertice* CTreap::GetLeftmostVert(Vertice* vert)
{
    return (vert ? vert->vert_leftmost : NULL);
}

inline Vertice* CTreap::GetRightmostVert(Vertice* vert)
{
    return (vert ? vert->vert_rightmost : NULL);
}

inline void CTreap::Push(Vertice* vert)
{
    if(!vert)
        return;

    if(vert->vert_left_child)
        vert->vert_left_child->vert_reversed ^= vert->vert_reversed;

    if(vert->vert_right_child)
        vert->vert_right_child->vert_reversed ^= vert->vert_reversed;

    if(vert->vert_reversed)
    {
        vert->vert_reversed = false;
        std::swap(vert->vert_left_child, vert->vert_right_child);
        std::swap(vert->vert_decreasing_length, vert->vert_increasing_length);
    }
}

void CTreap::Merge(Vertice*& resultTreapRoot, Vertice* leftSubtreeRoot, Vertice* rightSubtreeRoot)
{
    if(!leftSubtreeRoot || !rightSubtreeRoot)
        resultTreapRoot =  leftSubtreeRoot ? leftSubtreeRoot : rightSubtreeRoot;

    else if(leftSubtreeRoot->vert_priority > rightSubtreeRoot->vert_priority)
    {
        Push(leftSubtreeRoot);
        Push(rightSubtreeRoot);
        Merge(leftSubtreeRoot->vert_right_child, leftSubtreeRoot->vert_right_child, rightSubtreeRoot);
        resultTreapRoot = leftSubtreeRoot;
    }

    else
    {
        Push(leftSubtreeRoot);
        Push(rightSubtreeRoot);
        Merge(rightSubtreeRoot->vert_left_child, leftSubtreeRoot, rightSubtreeRoot->vert_left_child);
        resultTreapRoot = rightSubtreeRoot;
    }

    CTreap::Update(resultTreapRoot);
}

void CTreap::Split(Vertice* rootTreapRoot, Vertice*& leftSubtreeRoot, Vertice*& rightSubtreeRoot, const std::size_t key)
{
    if(!rootTreapRoot)
    {
        leftSubtreeRoot = NULL;
        rightSubtreeRoot = NULL;
        return;
    }

    if(GetSize(rootTreapRoot->vert_left_child) >= key)
    {
        Split(rootTreapRoot->vert_left_child, leftSubtreeRoot, rootTreapRoot->vert_left_child, key);
        rightSubtreeRoot = rootTreapRoot;
    }
    else
    {
        Split(rootTreapRoot->vert_right_child, rootTreapRoot->vert_right_child, rightSubtreeRoot, key - GetSize(rootTreapRoot->vert_left_child) - 1);
        leftSubtreeRoot = rootTreapRoot;
    }

    CTreap::Update(rootTreapRoot);
}

void CTreap::Print(Vertice* rootVertice) const
{
    if(!rootVertice)
        return;

    Push(rootVertice);
    Print(rootVertice->vert_left_child);
    std::cout << rootVertice->vert_value << " ";
    Print(rootVertice->vert_right_child);
}

void CTreap::Set(Vertice*& rootVertice, const int value, const std::size_t index)
{
    if(!rootVertice)
        return;

    Push(rootVertice);

    if(index == GetSize(rootVertice->vert_left_child) + 1)
        rootVertice->vert_value = value;
    else if(index <= GetSize(rootVertice->vert_left_child))
        Set(rootVertice->vert_left_child, value, index);
    else
        Set(rootVertice->vert_right_child, value, index - GetSize(rootVertice->vert_left_child) - 1);

    CTreap::Update(rootVertice);
}

void CTreap::BinarySearch(Vertice*& vert, Vertice* targetVert)
{
    assert(vert != 0);
    Push(vert);

    if (vert->vert_value > targetVert->vert_value)
    {
        if(vert->vert_right_child == 0)
            std::swap(vert->vert_value, targetVert->vert_value);
        else
            BinarySearch(vert->vert_right_child, targetVert);
    }
    else
    {
        if(vert->vert_left_child == 0)
            std::swap(vert->vert_value, targetVert->vert_value);
        else
            BinarySearch(vert->vert_left_child, targetVert);
    }

    CTreap::Update(vert);
}

void CTreap::insert(const int value, const std::size_t index, const int maxrand)
{
    Vertice* leftSubtreeRoot = NULL;
    Vertice* rightSubtreeRoot = NULL;

    Split(tr_root, leftSubtreeRoot, rightSubtreeRoot, index - 1);
    Merge(leftSubtreeRoot, leftSubtreeRoot, new Vertice(value, 0, maxrand));
    Merge(tr_root, leftSubtreeRoot, rightSubtreeRoot);

    ++tr_elements;
}

void CTreap::next_permutation(const std::size_t left, const std::size_t right)
{
    Vertice* leftSubtreeRoot = NULL;
    Vertice* resultSubtreeRoot = NULL;
    Vertice* rightSubtreeRoot = NULL;

    /*
     *cut out the subsegment [left, right]
     */

    Split(tr_root, leftSubtreeRoot, resultSubtreeRoot, left - 1);
    Split(resultSubtreeRoot, resultSubtreeRoot, rightSubtreeRoot, right - left + 1);

    /*
     *check if the subsegment is descending suffix
     */

    if(GetDecreasingSuffixLength(resultSubtreeRoot) == GetSize(resultSubtreeRoot))
    {
        Merge(resultSubtreeRoot, resultSubtreeRoot, rightSubtreeRoot);
        Merge(tr_root, leftSubtreeRoot, resultSubtreeRoot);
        return;
    }

    /*
     *cut out the longest descending suffix
     */

    Vertice* tempVertice = NULL;
    Vertice* subtreeRoot = NULL;

    Split(resultSubtreeRoot, resultSubtreeRoot, subtreeRoot, GetSize(resultSubtreeRoot) - GetDecreasingSuffixLength(resultSubtreeRoot));
    Split(resultSubtreeRoot, resultSubtreeRoot, tempVertice, GetSize(resultSubtreeRoot) - 1);

    /*
     *swap with the first element in suffix which is bigger than tempVertice
     */

    BinarySearch(subtreeRoot, tempVertice);

    subtreeRoot->vert_reversed = true;

    Merge(resultSubtreeRoot, resultSubtreeRoot, tempVertice);
    Merge(resultSubtreeRoot, resultSubtreeRoot, subtreeRoot);
    Merge(resultSubtreeRoot, resultSubtreeRoot, rightSubtreeRoot);
    Merge(tr_root, leftSubtreeRoot, resultSubtreeRoot);

    return;
}

long long CTreap::subsegment_sum(const std::size_t left, const std::size_t right)
{
    Vertice* leftSubtreeRoot = NULL;
    Vertice* rightSubtreeRoot = NULL;
    Vertice* tempSubtreeRoot = NULL;

    Split(tr_root, leftSubtreeRoot, rightSubtreeRoot, left - 1);
    Split(rightSubtreeRoot, rightSubtreeRoot, tempSubtreeRoot, right - left + 1);

    long long result = GetSubsegmentSum(rightSubtreeRoot);

    Merge(rightSubtreeRoot, rightSubtreeRoot, tempSubtreeRoot);
    Merge(tr_root, leftSubtreeRoot, rightSubtreeRoot);

    return result;
}

void CTreap::assign(const int value, const std::size_t index)
{
    Set(tr_root, value, index);
}

std::size_t CTreap::size() const
{
    return tr_elements;
}

CTreap* CTreap::build_treap(std::vector<int>& inputData, const int maxrand)
{
    srand(time(NULL));

    CTreap* resultTreap;

    std::cout << "Inserting element " << inputData.at(0) << std::endl;
    resultTreap = new CTreap(new Vertice(inputData.at(0), 0, maxrand), NULL, NULL);

    for(std::size_t i = 2; i <= inputData.size(); ++i)
    {
        std::cout << "Inserting element " << inputData.at(i - 1) << std::endl;
        resultTreap->insert(inputData.at(i - 1), i);
    }

    return resultTreap;
}

void CTreap::print() const
{
    Print(tr_root);
    std::cout << std::endl;
}
