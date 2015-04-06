#include <iostream>
#include <fstream>
#include <vector>
#include "chashset.h"
#include "ctest.h"

int main()
{
    CPerfectHashSet hashSet;
    CBucket bucket;

    CRandomNoCollisionsTest randomNoCollisionsTest;
    CCollisionsTest collisionsTest;
    CShufflesNoCollisionsTest shufflesNoCollisionsTest;
    CPossibleKeyTest possibleKeyTest;
    CHasInsertEraseTest hasInsertEraseTest;

    std::cout << "-----------CPerfectHashSet tests-----------" << std::endl;
    randomNoCollisionsTest.ApplyTest(hashSet, HUGE_TEST_LENGTH);
    hashSet.Clear();

    collisionsTest.SetCollisionProbability(70);
    collisionsTest.ApplyTest(hashSet, STANDART_TEST_LENGTH);
    hashSet.Clear();

    shufflesNoCollisionsTest.ApplyTest(hashSet, MICROSCOPIC_TEST_LENGTH);
    hashSet.Clear();

    possibleKeyTest.ApplyTest(hashSet, STANDART_TEST_LENGTH);
    hashSet.Clear();

    hasInsertEraseTest.ApplyTest(hashSet, HUGE_TEST_LENGTH);
    hashSet.Clear();
    std::cout << "---------------End of tests----------------" << std::endl;

    std::cout << std::endl;

    std::cout << "-----------CBucket tests-----------" << std::endl;
    randomNoCollisionsTest.ApplyTest(bucket, STANDART_TEST_LENGTH);
    bucket.Clear();

    collisionsTest.SetCollisionProbability(50);
    collisionsTest.ApplyTest(bucket, STANDART_TEST_LENGTH);
    bucket.Clear();

    shufflesNoCollisionsTest.ApplyTest(bucket, MICROSCOPIC_TEST_LENGTH);
    bucket.Clear();

    possibleKeyTest.ApplyTest(bucket, STANDART_TEST_LENGTH);
    bucket.Clear();

    hasInsertEraseTest.ApplyTest(bucket, STANDART_TEST_LENGTH);
    bucket.Clear();
    std::cout << "------------End of tests------------" << std::endl;

    return 0;
}
