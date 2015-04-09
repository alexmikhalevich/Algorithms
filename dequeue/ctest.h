#ifndef CTEST_H
#define CTEST_H
#include <vector>
#include <iterator>
#include <algorithm>
#include <deque>
#include <chrono>
#include <ctime>
#include <random>
#include "gtest/gtest.h"
#include "cdeque.hpp"

class CTest : public ::testing::Test
{
protected:
    CDeque<int> deque;
    std::vector<int> testArr;
    void SetUp();
};

#endif // CTEST_H
