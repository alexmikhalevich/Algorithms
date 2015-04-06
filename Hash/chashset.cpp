#include "chashset.h"

CPerfectHashSet::CPerfectHashSet()
{
}

CPerfectHashSet::~CPerfectHashSet()
{
    this->Clear();
}

void CPerfectHashSet::Clear()
{
    for(unsigned int i = 0; i < phs_hashTable.size(); ++i)
        phs_hashTable[i].Clear();
    phs_hashTable.clear();
}

void CPerfectHashSet::GenerateRandomCoeffs()
{
    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<long long> distribution(0, PRIME - 1);

    phs_coeffA = distribution(generator);
    phs_coeffB = distribution(generator);
}

bool CPerfectHashSet::CheckRepetitionsFirstPhase()
{
    for(unsigned int i = 0; i < phs_hashTable.size(); ++i)
    {
        std::vector<unsigned int> bucketData;
        phs_hashTable[i].GetBucketData(bucketData);

        if(bucketData.size() == 2 || bucketData.size() == 3)
        {
            if((bucketData[0] == bucketData[1]) && (bucketData[0] != std::numeric_limits<unsigned int>::max()))
                return true;

            else if(bucketData.size() == 3 && (bucketData[0] == bucketData[2] || bucketData[1] == bucketData[2]) &&
                    (bucketData[2] != std::numeric_limits<unsigned int>::max()))
                return true;
        }

        else
            for(unsigned int j = 1; j < bucketData.size(); ++j)
                if(bucketData[j - 1] == bucketData[j] && (bucketData[j] != std::numeric_limits<unsigned int>::max()))
                    return true;
    }

    return false;
}

unsigned long long CPerfectHashSet::CountSumSquaresLengths() const
{
    unsigned long long result = 0;
    for(unsigned int i = 0; i < phs_hashTable.size(); ++i)
        result += phs_hashTable[i].Size() * phs_hashTable[i].Size();

    return result;
}

void CPerfectHashSet::Init(const std::vector<unsigned int> &keys)
{
    phs_numOfKeys = keys.size();
    phs_keys = keys;

    this->GenerateRandomCoeffs();

    phs_hashTable.clear();
    phs_hashTable.resize(phs_numOfKeys);
    for(unsigned int i = 0; i < phs_numOfKeys; ++i)
        phs_hashTable[this->Hash(phs_keys.at(i))].Push(phs_keys.at(i));

    if(this->CheckRepetitionsFirstPhase())
        throw CExKeyRepetition();

    while(CountSumSquaresLengths() > 3 * phs_numOfKeys)
    {
        this->Clear();

        this->GenerateRandomCoeffs();
        phs_hashTable.resize(phs_numOfKeys);
        for(unsigned int i = 0; i < phs_numOfKeys; ++i)
            phs_hashTable[this->Hash(phs_keys.at(i))].Push(phs_keys.at(i));
    }

    for(unsigned int i = 0; i < phs_hashTable.size(); ++i)
    {
        std::vector<unsigned int> bucketData;
        phs_hashTable[i].GetBucketData(bucketData);
        phs_hashTable[i].Init(bucketData);
    }
}

void CPerfectHashSet::Set(const unsigned int key)
{
    if(this->Hash(key) >= phs_hashTable.size())
        throw CExOutOfRange();

    phs_hashTable[Hash(key)].Set(key);
}

void CPerfectHashSet::Erase(const unsigned int key)
{
    if(Hash(key) >= phs_hashTable.size())
        throw CExOutOfRange();

    phs_hashTable[Hash(key)].Erase(key);
}

bool CPerfectHashSet::Has(const unsigned int key) const
{
    if(Hash(key) >= phs_hashTable.size())
        throw CExOutOfRange();

    else if(phs_hashTable[this->Hash(key)].Size() == 0)
        return false;

    return (phs_hashTable[Hash(key)].Has(key)) ? true : false;
}

bool CPerfectHashSet::IsPossibleKey(const unsigned int key) const
{
    for(unsigned int i = 0; i < phs_keys.size(); ++i)
        if(key == phs_keys.at(i))
            return true;

    return false;
}

unsigned int CPerfectHashSet::Hash(const unsigned int key) const
{
    /*
     *a = 2^32 * a1 + b1
     */

    long long a1 = phs_coeffA >> 32;
    long long b1 = phs_coeffA - a1;

    /*
     *ax = 2^32 * a1 * key + b1*key
     *Hash function: ((ax+b)%PRIME)%n <=> (ax%PRIME + b%PRIME)%n <=> (((2^32*a1*key)%PRIME + (b1*b2)%PRIME) + b%PRIME)%n
     */

    return ((((((a1 * key) << 32) % PRIME) + (b1 * key) % PRIME) + phs_coeffB) % PRIME) % phs_hashTable.size();
}

long long CPerfectHashSet::Size() const
{
    return phs_hashTable.size();
}
