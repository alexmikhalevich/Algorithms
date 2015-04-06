#include <assert.h>
#include "ctest.h"

CTest::CTest(const int numOfCommands, const int maxrand)
{
    tst_numOfCommands = numOfCommands;
    tst_maxrand = maxrand;
}

void CTest::apply_test(IPermutationStructure* permStruct)
{
    for(int i = 0; i < tst_numOfCommands; ++i)
    {
        int testNum = rand() % 4;
        switch(testNum)
        {
            case 0: //insert
            {
                std::cout << "TESTING: INSERT COMMAND" << std::endl;
                clock_t time = clock();
                permStruct->insert(rand(), rand() % permStruct->size(), tst_maxrand);
                time = clock() - time;
                std::cout << "DONE IN " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

                break;
            }
            case 1: //assign
            {
                std::cout << "TESTING: ASSIGN COMMAND" << std::endl;
                clock_t time = clock();
                permStruct->assign(rand(), rand() % permStruct->size());
                time = clock() - time;
                std::cout << "DONE IN " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

                break;
            }
            case 2: //subsegment_sum
            {
                std::cout << "TESTING: SUBSEGMENT_SUM COMMAND" << std::endl;
                clock_t time = clock();
                std::cout << permStruct->subsegment_sum(rand() % (permStruct->size() / 2),
                                          (rand() % (permStruct->size() / 2)) + (permStruct->size() / 2) - 1) << std::endl;
                time = clock() - time;
                std::cout << "DONE IN " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

                break;
            }
            case 3: //next_permutation
            {
                std::cout << "TESTING: NEXT_PERMUTATION COMMAND" << std::endl;
                clock_t time = clock();
                permStruct->next_permutation(rand() % (permStruct->size() / 2),
                                             (rand() % (permStruct->size() / 2)) + (permStruct->size() / 2) - 1);
                time = clock() - time;
                std::cout << "DONE IN " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;

                break;
            }
        }
    }
}

void CTest::apply_test(IPermutationStructure* permStructFirst, IPermutationStructure* permStructSecond, EFunctions func)
{
    for(int i = 0; i < tst_numOfCommands; ++i)
    {
        if(permStructFirst->size() != permStructSecond->size())
        {
            std::cerr << "EE: Different sizes" << std::endl;
            std::cout << "First size: " << permStructFirst->size() << std::endl;
            std::cout << "Second size: " << permStructSecond->size() << std::endl;
            return;
        }

        switch(func)
        {
            case EFunctions::INSERT:
            {
                std::cout << "TESTING: INSERT COMMAND" << std::endl;
                int insertData = rand();
                std::size_t position = rand() % permStructFirst->size() + 1;
                permStructFirst->insert(insertData, position, tst_maxrand);
                permStructSecond->insert(insertData, position, tst_maxrand);

                break;
            }
            case EFunctions::ASSIGN:
            {
                std::cout << "TESTING: ASSIGN COMMAND" << std::endl;
                int value = rand();
                std::size_t position = rand() % permStructFirst->size() + 1;
                permStructFirst->assign(value, position);
                permStructSecond->assign(value, position);

                break;
            }
            case EFunctions::SUBSEGMENT_SUM:
            {
                std::cout << "TESTING: SUBSEGMENT_SUM COMMAND: ";
                std::size_t left = rand() % (permStructFirst->size() / 2);
                std::size_t right = (rand() % (permStructFirst->size() / 2)) + (permStructFirst->size() / 2) - 1;
                if(left == 0) ++left;

                long long firstResult = permStructFirst->subsegment_sum(left, right);
                long long secondResult = permStructSecond->subsegment_sum(left, right);

                if(firstResult == secondResult)
                    std::cout << "OK" << std::endl;
                else
                {
                    std::cout << "Different answers: " << firstResult << " --- " << secondResult << std::endl;
                    std::cout << "Maybe your arrays are not equal?" << std::endl;
                    std::cout << "left = " << left << "; right = " << right << std::endl;
                    permStructFirst->print();
                    permStructSecond->print();
                }

                break;
            }
            case EFunctions::NEXT_PERMUTATION:
            {
                std::cout << "TESTING: NEXT_PERMUTATION COMMAND" << std::endl;
                std::size_t left = rand() % (permStructFirst->size() / 2);
                std::size_t right = (rand() % (permStructFirst->size() / 2)) + (permStructFirst->size() / 2) - 1;
                permStructFirst->next_permutation(left, right);
                permStructSecond->next_permutation(left, right);

                permStructFirst->print();
                permStructSecond->print();

                break;
            }
        }
    }
}
