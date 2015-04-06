#ifndef CBUCKET_H
#define CBUCKET_H
#include "ihashset.h"

class CBucket : public virtual IHashTable
{
private:
    virtual unsigned int Hash(const unsigned int key) const;
    bool CheckRepetitionsSecondPhase() const;
    std::vector<unsigned int> bckt_bucket;
    long long bckt_coeffA = 0;
    long long bckt_coeffB = 0;
    const unsigned int emptyCell = std::numeric_limits<unsigned int>::max();
public:
    CBucket();
    ~CBucket();
    virtual void GenerateRandomCoeffs();
    virtual void Init(const std::vector<unsigned int> &keys);
    virtual bool Has(const unsigned int key) const;
    virtual void Erase(const unsigned int key);
    virtual bool IsPossibleKey(const unsigned int key) const;
    virtual long long Size() const;
    virtual void Set(const unsigned int key);
    void Push(const unsigned int value);
    void GetBucketData(std::vector<unsigned int> &bucketData) const;
    void Clear();

    void Print();
};

#endif // CBUCKET_H
