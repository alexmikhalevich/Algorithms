#ifndef IPERMUTATIONSTRUCTURE_H
#define IPERMUTATIONSTRUCTURE_H
#include <cstdlib>

class IPermutationStructure
{
public:
    virtual void insert(const int value, const std::size_t index, const int maxrand = RAND_MAX) = 0;
    virtual void assign(const int value, const std::size_t index) = 0;
    virtual long long subsegment_sum(const std::size_t left, const std::size_t right) = 0;
    virtual void next_permutation(const std::size_t left, const std::size_t right) = 0;
    virtual std::size_t size() const = 0;
    virtual void print() const = 0;
    virtual ~IPermutationStructure() {}
};

#endif // IPERMUTATIONSTRUCTURE_H
