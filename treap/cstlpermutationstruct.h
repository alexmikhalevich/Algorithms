#ifndef CSTLPERMUTATIONSTRUCT_H
#define CSTLPERMUTATIONSTRUCT_H
#include <vector>
#include <algorithm>
#include "ipermutationstructure.h"

class CStlPermutationStruct : public virtual IPermutationStructure
{
private:
    std::vector<int> inputData;
public:
    CStlPermutationStruct(const std::vector<int>& input);
    ~CStlPermutationStruct();
    virtual void insert(const int value, const std::size_t index, const int maxrand = RAND_MAX);
    virtual void assign(const int value, const std::size_t index);
    virtual long long subsegment_sum(const std::size_t left, const std::size_t right);
    virtual void next_permutation(const std::size_t left, const std::size_t right);
    virtual std::size_t size() const;
    virtual void print() const;
};

#endif // CSTLPERMUTATIONSTRUCT_H
