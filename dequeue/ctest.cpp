#include "ctest.h"

#define REPETITIONS 20
#define COMMANDS 1000000

void CTest::SetUp()
{
    testArr = {2, 5, 4, 7, 8, 9, 12, 13, 14, 18, 32, 23, 98, 56, 33};
    for(std::vector<int>::iterator iter = testArr.begin(); iter < testArr.end(); ++iter)
        deque.push_back(*iter);
}

TEST_F(CTest, std_iterator)
{
    EXPECT_TRUE(std::equal(testArr.begin(), testArr.end(), deque.begin()));
}

TEST_F(CTest, std_reverse_iterator)
{
    EXPECT_TRUE(std::equal(testArr.rbegin(), testArr.rend(), deque.rbegin()));
}

TEST_F(CTest, std_sort)
{
    std::sort(deque.begin(), deque.end(), std::less<int>());
    std::sort(testArr.begin(), testArr.end(), std::less<int>());
    EXPECT_TRUE(std::equal(testArr.begin(), testArr.end(), deque.begin()));
}

TEST_F(CTest, pop_back)
{
    while(!testArr.empty())
    {
        EXPECT_FALSE(deque.empty());
        EXPECT_EQ(*(deque.end() - 1), *(testArr.end() - 1));
        EXPECT_NO_THROW(deque.pop_back());
        testArr.pop_back();
        EXPECT_EQ(deque.size(), testArr.size());
    }
    EXPECT_EQ(deque.size(), 0);
    EXPECT_TRUE(deque.empty());
}

TEST_F(CTest, pop_front)
{
    for(std::size_t i = 0; i < testArr.size(); ++i)
    {
        EXPECT_FALSE(deque.empty());
        EXPECT_EQ(deque.at(0), testArr.at(i));
        EXPECT_EQ(deque.size(), testArr.size() - i);
        EXPECT_NO_THROW(deque.pop_front());
    }
    EXPECT_EQ(deque.size(), 0);
    EXPECT_TRUE(deque.empty());
}

TEST_F(CTest, random_access)
{
    for(std::size_t i = 0; i < deque.size(); ++i)
        EXPECT_EQ(deque[i], testArr[i]);
    EXPECT_EQ(*(deque.begin()), *(testArr.begin()));
    EXPECT_EQ(*(deque.end() - 1), *(testArr.end() - 1));
}

TEST(Test, push_front)
{
    CDeque<int> deque;
    for(std::size_t i = 0; i < 30; ++i)
    {
        EXPECT_EQ(deque.size(), i);
        EXPECT_NO_THROW(deque.push_front(i));
        EXPECT_EQ(deque.at(0), i);
    }
}

TEST(Test, amortized_working_time)
{
    std::srand(std::time(NULL));

    std::deque<int> testDeque;
    CDeque<int> deque;

    for(int i = 0; i < REPETITIONS; ++i)
    {
        std::clock_t time = std::clock();
        for(int j = 0; j < COMMANDS; ++j)
        {
            switch(rand() % 4)
            {
                case 0:                              //push_back
                {
                    int num = rand();
                    deque.push_back(num);
                    testDeque.push_back(num);
                    break;
                }
                case 1:                              //push_front
                {
                    int num = rand();
                    deque.push_front(num);
                    testDeque.push_front(num);
                    break;
                }
                case 2:                              //pop_back
                {
                    if(testDeque.empty())
                        break;
                    testDeque.pop_back();
                    deque.pop_back();
                    break;
                }
                case 3:                              //pop_front
                {
                    if(testDeque.empty())
                        break;
                    testDeque.pop_front();
                    deque.pop_front();
                    break;
                }
            }
        }
        std::cout << std::fixed << std::setprecision(3) << (double)(std::clock() - time) / (double)CLOCKS_PER_SEC << std::endl;
    }
}
