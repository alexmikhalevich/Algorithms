#ifndef IHASHSET_H
#define IHASHSET_H
#include <vector>
#include <limits>
#include <cmath>
#include <random>
#include <ctime>
#include <iostream>
#include "cexception.h"

class IHashTable
{
private:
    virtual void GenerateRandomCoeffs() = 0;

public:
    const long long PRIME = 4294967311;

    virtual unsigned int Hash(const unsigned int key) const = 0;
    virtual void Init(const std::vector<unsigned int> &keys) = 0;
    virtual bool Has(const unsigned int key) const = 0;
    virtual void Erase(const unsigned int key) = 0;
    virtual bool IsPossibleKey(const unsigned int key) const = 0;
    virtual void Set(const unsigned int key) = 0;
    virtual long long Size() const = 0;
};
#endif // IHASHSET_H
