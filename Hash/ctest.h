#ifndef CTEST_H
#define CTEST_H
#include <random>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "ihashset.h"
#include "cexception.h"

#define MICROSCOPIC_TEST_LENGTH 8
#define TINY_TEST_LENGTH 64
#define SMALL_TEST_LENGTH 256
#define STANDART_TEST_LENGTH 512
#define EXTENDED_TEST_LENGTH 1024
#define DBL_EXTENDED_TEST_LENGTH 2048
#define HUGE_TEST_LENGTH 1000000

class CTest
{
public:
    static const unsigned int MAX_UINT = std::numeric_limits<unsigned int>::max() - 1;
    std::vector<unsigned int> tst_keys;
    virtual void GenerateTest(const unsigned int testLength) = 0;
    virtual void ApplyTest(IHashTable& ht, const unsigned int testLength) = 0;
};

class CRandomNoCollisionsTest : public virtual CTest
{
private:
    virtual void GenerateTest(const unsigned int testLength);
public:
    CRandomNoCollisionsTest();
    virtual void ApplyTest(IHashTable &ht, const unsigned int testLength);
};

class CShufflesNoCollisionsTest : public virtual CTest
{
private:
    virtual void GenerateTest(const unsigned int testLength);
public:
    CShufflesNoCollisionsTest();
    virtual void ApplyTest(IHashTable &ht, const unsigned int testLength);
};

class CCollisionsTest : public virtual CTest
{
private:
    virtual void GenerateTest(const unsigned int testLength);
    int coltst_collisionProbability = 50;
public:
    CCollisionsTest();
    virtual void ApplyTest(IHashTable &ht, const unsigned int testLength);
    void SetCollisionProbability(const unsigned int probability);
};

class CPossibleKeyTest : public virtual CTest
{
private:
    virtual void GenerateTest(const unsigned int testLength);
    std::vector<unsigned int> pkt_alienKeys;
public:
    CPossibleKeyTest();
    virtual void ApplyTest(IHashTable &ht, const unsigned int testLength);
};

class CHasInsertEraseTest : public virtual CTest
{
private:
    virtual void GenerateTest(const unsigned int testLength);
public:
    CHasInsertEraseTest();
    virtual void ApplyTest(IHashTable &ht, const unsigned int testLength);
};

#endif // CTEST_H
