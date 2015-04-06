#include "ctest.h"


CShufflesNoCollisionsTest::CShufflesNoCollisionsTest()
{
}

CRandomNoCollisionsTest::CRandomNoCollisionsTest()
{
}

CCollisionsTest::CCollisionsTest()
{
}

CPossibleKeyTest::CPossibleKeyTest()
{
}

CHasInsertEraseTest::CHasInsertEraseTest()
{
}

void CRandomNoCollisionsTest::GenerateTest(const unsigned int testLength)
{
    tst_keys.clear();
    std::map<unsigned int, bool> usedKeys;
    srand(std::time(NULL));

    for(unsigned int i = 0; i < testLength; ++i)
    {
        unsigned int randNum = rand() % MAX_UINT;

        while(usedKeys.find(randNum) != usedKeys.end())
            randNum = rand() % MAX_UINT;

        usedKeys[randNum] = 1;
        tst_keys.push_back(randNum);
    }
}

void CRandomNoCollisionsTest::ApplyTest(IHashTable &ht, const unsigned int testLength)
{
    std::cout << "CRandomNoCollisionsTest: ";
    this->GenerateTest(testLength);
    try
    {
        clock_t time = clock();
        ht.Init(this->tst_keys);

        for(unsigned int i = 0; i < tst_keys.size(); ++i)
            if(!ht.Has(tst_keys.at(i)))
                throw CExInitError();
        time = clock() - time;
        std::cout << "passed successfully; time = " << 1000.0 * time / CLOCKS_PER_SEC << "ms" << std::endl;
    }
    catch(CException& e)
    {
        e.PrintError();
    }
}

void CShufflesNoCollisionsTest::GenerateTest(const unsigned int testLength)
{
    if(testLength > 10)
        throw CExBigTest();

    tst_keys.clear();
    std::map<unsigned int, bool> usedKeys;
    srand(std::time(NULL));

    for(unsigned int i = 0; i < testLength; ++i)
    {
        unsigned int randNum = rand() % MAX_UINT;

        while(usedKeys.find(randNum) != usedKeys.end())
            randNum = rand() % MAX_UINT;

        usedKeys.insert(std::pair<unsigned int, bool>(randNum, true));
        tst_keys.push_back(randNum);
    }
}

void CShufflesNoCollisionsTest::ApplyTest(IHashTable &ht, const unsigned int testLength)
{
    std::cout << "CShufflesNoCollisionsTest: ";

    this->GenerateTest(testLength);
    try
    {
        while(std::next_permutation(tst_keys.begin(), tst_keys.end()))
        {
            ht.Init(this->tst_keys);
            for(unsigned int i = 0; i < tst_keys.size(); ++i)
                if(!ht.Has(tst_keys.at(i)))
                    throw CExInitError();
        }
        std::cout << "passed successfully" << std::endl;
    }
    catch(CException& e)
    {
        e.PrintError();
    }
}

void CCollisionsTest::GenerateTest(const unsigned int testLength)
{
    srand(std::time(NULL));
    tst_keys.clear();

    tst_keys.push_back(rand() % MAX_UINT);
    for(unsigned int i = 1; i < testLength; ++i)
    {
        tst_keys.push_back(rand() % MAX_UINT);
        if(std::abs(rand()) % 100 <= coltst_collisionProbability)
        {
            tst_keys.push_back(tst_keys.at(i - 1));
            ++i;
        }
    }

    std::random_shuffle(tst_keys.begin(), tst_keys.end());
}

void CCollisionsTest::ApplyTest(IHashTable &ht, const unsigned int testLength)
{
    std::cout << "CCollisionsTest: ";
    this->GenerateTest(testLength);
    try
    {
        ht.Init(this->tst_keys);
        std::cout << "test failed" << std::endl;
    }
    catch(CException& e)
    {
        e.PrintError();
    }
}

void CCollisionsTest::SetCollisionProbability(const unsigned int probability)
{
    coltst_collisionProbability = probability;
}

void CPossibleKeyTest::GenerateTest(const unsigned int testLength)
{
    std::map<unsigned int, bool> usedKeys;
    srand(std::time(NULL));
    tst_keys.clear();

    for(unsigned int i = 0; i < testLength; ++i)
    {
        unsigned int randNum = rand() % MAX_UINT;

        while(usedKeys.find(randNum) != usedKeys.end())
            randNum = rand() % MAX_UINT;

        usedKeys.insert(std::pair<unsigned int, bool>(randNum, true));
        tst_keys.push_back(randNum);
    }

    for(unsigned int i = 0; i < testLength; ++i)
    {
        unsigned int alienKey = rand() % MAX_UINT;
        while(usedKeys.find(alienKey) != usedKeys.end())
            alienKey = rand() % MAX_UINT;

        pkt_alienKeys.push_back(alienKey);
    }
}

void CPossibleKeyTest::ApplyTest(IHashTable &ht, const unsigned int testLength)
{
    std::cout << "CPossibleKeyTest: ";
    this->GenerateTest(testLength);
    try
    {
        ht.Init(this->tst_keys);
        for(unsigned int i = 0; i < tst_keys.size(); ++i)
            if(!ht.IsPossibleKey(tst_keys.at(i)))
                throw CExPossibleKeyError();
        for(unsigned int i = 0; i < tst_keys.size(); ++i)
            if(ht.IsPossibleKey(pkt_alienKeys.at(i)))
                throw CExPossibleKeyError();
        std::cout << "passed successfully" << std::endl;
    }
    catch(CException& e)
    {
        e.PrintError();
    }
}

void CHasInsertEraseTest::GenerateTest(const unsigned int testLength)
{
    tst_keys.clear();
    std::map<unsigned int, bool> usedKeys;
    srand(std::time(NULL));

    for(unsigned int i = 0; i < testLength; ++i)
    {
        unsigned int randNum = rand() % MAX_UINT;

        while(usedKeys.find(randNum) != usedKeys.end())
            randNum = rand() % MAX_UINT;

        usedKeys[randNum] = 1;
        tst_keys.push_back(randNum);
    }
}

void CHasInsertEraseTest::ApplyTest(IHashTable &ht, const unsigned int testLength)
{
    std::cout << "CHasInsertEraseTest: ";
    this->GenerateTest(testLength);
    try
    {
        ht.Init(this->tst_keys);

        for(unsigned int i = 0; i < tst_keys.size(); ++i)
        {
            if(!ht.Has(tst_keys.at(i)))
                throw CExHasError();
            ht.Erase(tst_keys.at(i));
            if(ht.Has(tst_keys.at(i)))
                throw CExEraseError();
            ht.Set(tst_keys.at(i));
            if(!ht.Has(tst_keys.at(i)))
                throw CExInsertError();
        }

        std::cout << "passed successfully" << std::endl;
    }
    catch(CException& e)
    {
        e.PrintError();
    }
}
