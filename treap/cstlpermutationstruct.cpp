#include <iostream>
#include "cstlpermutationstruct.h"

CStlPermutationStruct::CStlPermutationStruct(const std::vector<int>& input)
{
    inputData = input;
}

CStlPermutationStruct::~CStlPermutationStruct()
{
    inputData.clear();
}

void CStlPermutationStruct::assign(const int value, const std::size_t index)
{
    inputData[index - 1] = value;
}

void CStlPermutationStruct::insert(const int value, const std::size_t index, const int maxrand)
{
    std::vector<int>::iterator position = inputData.begin() + index - 1;
    inputData.insert(position, value);
}

void CStlPermutationStruct::next_permutation(const std::size_t left, const std::size_t right)
{
    std::vector<int>::iterator leftIterator = inputData.begin() + left - 1;
    std::vector<int>::iterator rightIterator = inputData.begin() + right - 1;

    std::next_permutation(leftIterator, rightIterator);
}

long long CStlPermutationStruct::subsegment_sum(const std::size_t left, const std::size_t right)
{
    std::vector<int>::iterator leftIterator = inputData.begin() + left - 1;
    std::vector<int>::iterator rightIterator = inputData.begin() + right - 1;

    long long result = 0;
    for(; leftIterator <= rightIterator; ++leftIterator)
        result += *leftIterator;

    return result;
}

std::size_t CStlPermutationStruct::size() const
{
    return inputData.size();
}

void CStlPermutationStruct::print() const
{
    for(std::size_t i = 0; i < inputData.size(); ++i)
        std::cout << inputData.at(i) << " ";
    std::cout << std::endl;
}
