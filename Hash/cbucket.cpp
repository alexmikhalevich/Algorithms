#include "cbucket.h"

CBucket::CBucket()
{
}

CBucket::~CBucket()
{
}

void CBucket::Clear()
{
    bckt_bucket.clear();
}

void CBucket::GetBucketData(std::vector<unsigned int> &bucketData) const
{
    bucketData = bckt_bucket;
}

bool CBucket::CheckRepetitionsSecondPhase() const
{
    if(bckt_bucket.size() > 1)
        for(unsigned int j = 0; j < bckt_bucket.size() - 1; ++j)
            for(unsigned int k = j + 1; k < bckt_bucket.size(); ++k)
                if(bckt_bucket[j] == bckt_bucket[k] &&
                        (bckt_bucket[j] != std::numeric_limits<unsigned int>::max()))
                    return true;

    return false;
}

void CBucket::Init(const std::vector<unsigned int> &keys)
{
    if(keys.size() == 0)
        return;

    bckt_bucket = keys;
    if(this->CheckRepetitionsSecondPhase())
        throw CExKeyRepetition();

    bckt_bucket.resize(keys.size() * keys.size());

    bool isSucceed = true;
    do
    {
        isSucceed = true;
        this->GenerateRandomCoeffs();
        for(std::vector<unsigned int>::iterator iter = bckt_bucket.begin(); iter != bckt_bucket.end(); ++iter)
            *iter = std::numeric_limits<unsigned int>::max();

        for(unsigned int i = 0; i < keys.size(); ++i)
        {
            if(bckt_bucket[Hash(keys.at(i))] == std::numeric_limits<unsigned int>::max())
                bckt_bucket[Hash(keys.at(i))] = keys.at(i);
            else
            {
                isSucceed = false;
                break;
            }
        }
    } while(!isSucceed);
}

bool CBucket::IsPossibleKey(const unsigned int key) const
{
    for(unsigned int i = 0; i < bckt_bucket.size(); ++i)
        if(key == bckt_bucket.at(i))
            return true;
    return false;
}

long long CBucket::Size() const
{
    return bckt_bucket.size();
}

void CBucket::Set(const unsigned int key)
{
    if(this->Hash(key) >= bckt_bucket.size())
        throw CExOutOfRange();

    if(bckt_bucket[Hash(key)] != emptyCell)
    {
        if(bckt_bucket[Hash(key)] == key)
            throw CExKeyRepetition();
        else
            throw CExAlienKey();
    }

    bckt_bucket[Hash(key)] = key;
}

void CBucket::Erase(const unsigned int key)
{
    if(Hash(key) >= bckt_bucket.size())
        throw CExOutOfRange();

    bckt_bucket[Hash(key)] = std::numeric_limits<unsigned int>::max();
}

bool CBucket::Has(const unsigned int key) const
{
    if(Hash(key) >= bckt_bucket.size())
        throw CExOutOfRange();

    return (bckt_bucket[Hash(key)] == key) ? true : false;
}

unsigned int CBucket::Hash(const unsigned int key) const
{
    /*
     *a = 2^32 * a1 + b1
     */

    long long a1 = bckt_coeffA >> 32;
    long long b1 = bckt_coeffA - a1;

    /*
     *ax = 2^32 * a1 * key + b1*key
     *Hash function: ((ax+b)%PRIME)%n <=> (ax%PRIME + b%PRIME)%n <=> (((2^32*a1*key)%PRIME + (b1*b2)%PRIME) + b%PRIME)%n
     */

    return ((((((a1 * key) << 32) % PRIME) + (b1 * key) % PRIME) + bckt_coeffB) % PRIME) % bckt_bucket.size();
}

void CBucket::GenerateRandomCoeffs()
{
    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<long long> distribution(0, PRIME - 1);

    bckt_coeffA = distribution(generator);
    bckt_coeffB = distribution(generator);
}

void CBucket::Push(const unsigned int value)
{
    bckt_bucket.push_back(value);
}
