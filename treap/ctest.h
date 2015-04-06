#ifndef CTEST_H
#define CTEST_H
#include <random>
#include <ctime>
#include <iostream>
#include <chrono>
#include "ipermutationstructure.h"

enum EFunctions
{
    INSERT,
    ASSIGN,
    SUBSEGMENT_SUM,
    NEXT_PERMUTATION
};

class CTest
{
private:
    int tst_numOfCommands;
    int tst_maxrand;
public:
    CTest(const int numOfCommands, const int maxrand);
    void apply_test(IPermutationStructure* permStruct);
    void apply_test(IPermutationStructure* permStructFirst, IPermutationStructure* permStructSecond, EFunctions func);
};

#endif // CTEST_H
