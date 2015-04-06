#ifndef CPERFECTHASHSET_H
#define CPERFECTHASHSET_H

#include "cbucket.h"

class CPerfectHashSet : public virtual IHashTable
{
private:    
    std::vector<CBucket> phs_hashTable;
    virtual unsigned int Hash(const unsigned int key) const;
    bool CheckRepetitionsFirstPhase();
    virtual void GenerateRandomCoeffs();
    unsigned long long CountSumSquaresLengths() const;
    std::vector<unsigned int> phs_keys;
    long long phs_coeffA = 0;
    long long phs_coeffB = 0;
    unsigned long long phs_numOfKeys = 0;

public:
    CPerfectHashSet();
    ~CPerfectHashSet();
    void Clear();
    virtual void Init(const std::vector<unsigned int> &keys);
    virtual bool Has(const unsigned int key) const;
    virtual void Erase(const unsigned int key);
    virtual bool IsPossibleKey(const unsigned int key) const;
    virtual long long Size() const;
    virtual void Set(const unsigned int key);
};

#endif // CPERFECTHASHSET_H
