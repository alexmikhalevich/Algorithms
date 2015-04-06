#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <cstdio>
#include "ctreap.h"
#include "cstlpermutationstruct.h"
#include "ctest.h"
#include "cexception.h"

using namespace std;

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);

    if(argc == 2)
    {
        ifstream inputStream(argv[1]);
        if(!inputStream.is_open())
            std::cerr << "Unable to open input file; aborting" << std::endl;
        else
        {
            std::vector<int> inputData;
            CTreap* treap = NULL;
            bool isBuilt = false;
            while(!inputStream.eof())
            {
                int temp = 0;
                inputStream >> temp;
                inputData.push_back(temp);
            }
            inputStream.close();

            CStlPermutationStruct* permStruct = new CStlPermutationStruct(inputData);

            while(true)
            {
                try
                {
                    string command = "";
                    std::cout << "(treap): ";
                    std::cin >> command;

                    if(command == "quit")
                    {
                        std::cout << "Quitting" << std::endl;
                        break;
                    }
                    else if(command == "show-input")
                    {
                        for(unsigned int i = 0; i < inputData.size(); ++i)
                            std::cout << inputData.at(i) << " ";
                        std::cout << std::endl;
                    }
                    else if(command == "build")
                    {
                        clock_t time = clock();
                        treap = CTreap::build_treap(inputData);
                        time = clock() - time;
                        std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                        isBuilt = true;
                    }
                    else if(command == "insert")
                    {
                        if(!isBuilt)
                        {
                            std::cerr << "EE: The treap was not built yet" << std::endl;
                            std::cout << "Building treap..." << std::endl;
                            clock_t time = clock();
                            treap = CTreap::build_treap(inputData);
                            time = clock() - time;
                            std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                            isBuilt = true;
                        }

                        std::size_t position = 0;
                        int key = 0;
                        std::cout << "Enter position: ";
                        std::cin >> position;
                        std::cout << "Enter key: ";
                        std::cin >> key;
                        permStruct->insert(key, position);
                        clock_t time = clock();
                        treap->insert(key, position, RAND_MAX);
                        time = clock() - time;
                        std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                    }
                    else if(command == "next-permutation")
                    {
                        if(!isBuilt)
                        {
                            std::cerr << "EE: The treap was not built yet" << std::endl;
                            std::cout << "Building treap..." << std::endl;
                            clock_t time = clock();
                            treap = CTreap::build_treap(inputData);
                            time = clock() - time;
                            std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                            isBuilt = true;
                        }

                        std::size_t leftBound;
                        std::size_t rightBound;
                        std::cout << "Enter the left bound: ";
                        std::cin >> leftBound;
                        std::cout << "Enter the right bound: ";
                        std::cin >> rightBound;
                        clock_t time = clock();

                        treap->next_permutation(leftBound, rightBound);

                        time = clock() - time;
                        std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                    }
                    else if(command == "subsegment-sum")
                    {
                        if(!isBuilt)
                        {
                            std::cerr << "EE: The treap was not built yet" << std::endl;
                            std::cout << "Building treap..." << std::endl;
                            clock_t time = clock();
                            treap = CTreap::build_treap(inputData);
                            time = clock() - time;
                            std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                            isBuilt = true;
                        }

                        std::size_t keyA, keyB;
                        std::cout << "Enter the left segment border: ";
                        std::cin >> keyA;
                        std::cout << "Enter the right segment border: ";
                        std::cin >> keyB;
                        clock_t time = clock();
                        std::cout << "Treap: " << treap->subsegment_sum(keyA, keyB) << std::endl;
                        std::cout << "Stl:   " << permStruct->subsegment_sum(keyA, keyB) << std::endl;
                        time = clock() - time;
                        std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                    }
                    else if(command == "replace")
                    {
                        if(!isBuilt)
                        {
                            std::cerr << "EE: The treap was not built yet" << std::endl;
                            std::cout << "Building treap..." << std::endl;
                            clock_t time = clock();
                            treap = CTreap::build_treap(inputData);
                            time = clock() - time;
                            std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                            isBuilt = true;
                        }

                        std::size_t position = 0;
                        int key = 0;
                        std::cout << "Enter position: ";
                        std::cin >> position;
                        std::cout << "Enter key: ";
                        std::cin >> key;
                        permStruct->assign(key, position);
                        clock_t time = clock();
                        treap->assign(key, position);
                        time = clock() - time;
                        std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                    }
                    else if(command == "show-treap")
                    {
                        if(!isBuilt)
                        {
                            std::cerr << "EE: The treap was not built yet" << std::endl;
                            std::cout << "Building treap..." << std::endl;
                            clock_t time = clock();
                            treap = CTreap::build_treap(inputData);
                            time = clock() - time;
                            std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                            isBuilt = true;
                        }

                        std::cout << "treap: ";
                        treap->print();
                        std::cout << "stl: ";
                        permStruct->print();
                        std::cout << std::endl;
                    }
                    else if(command == "test")
                    {
                        if(!isBuilt)
                        {
                            std::cerr << "EE: The treap was not built yet" << std::endl;
                            std::cout << "Building treap..." << std::endl;
                            clock_t time = clock();
                            treap = CTreap::build_treap(inputData);
                            time = clock() - time;
                            std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                            isBuilt = true;
                        }

                        CTest* test;

                        int commandsNum;
                        std::string maxrand;
                        std::cout << "Enter num of commands: ";
                        std::cin >> commandsNum;
                        std::cout << "Enter maximal priority (type RAND_MAX for maximal possible value): ";
                        std::cin >> maxrand;

                        if(std::strcmp(maxrand.c_str(), "RAND_MAX") == 0)
                            test = new CTest(commandsNum, RAND_MAX);
                        else
                            test = new CTest(commandsNum, atoi(maxrand.c_str()));

                        test->apply_test(treap);

                        char answer;

                        TESTING_QUESTION:
                        std::cout << "Do you want to test stl realisation? (y, n): ";
                        std::cin >> answer;
                        if(answer == 'y')
                            test->apply_test(new CStlPermutationStruct(inputData));
                        else if(answer == 'n')
                            continue;
                        else
                        {
                            std::cout << "EE: Unknown answer" << std::endl;
                            goto TESTING_QUESTION;
                        }

                        delete test;

                    }
                    else if(command == "compare")
                    {
                        if(!isBuilt)
                        {
                            std::cerr << "EE: The treap was not built yet" << std::endl;
                            std::cout << "Building treap..." << std::endl;
                            clock_t time = clock();
                            treap = CTreap::build_treap(inputData);
                            time = clock() - time;
                            std::cout << "Successfully done in " << 1000.0 * time / CLOCKS_PER_SEC << " ms" << std::endl;
                            isBuilt = true;
                        }

                        CTest* test;

                        int commandsNum;
                        std::string maxrand, function;
                        std::cout << "Enter num of commands: ";
                        std::cin >> commandsNum;
                        std::cout << "Enter function to test: ";
                        std::cin >> function;
                        std::cout << "Enter maximal priority (type RAND_MAX for maximal possible value): ";
                        std::cin >> maxrand;

                        if(std::strcmp(maxrand.c_str(), "RAND_MAX") == 0)
                            test = new CTest(commandsNum, RAND_MAX);
                        else
                            test = new CTest(commandsNum, atoi(maxrand.c_str()));

                        CStlPermutationStruct* stlStruct = new CStlPermutationStruct(inputData);

                        if(std::strcmp(function.c_str(), "insert") == 0)
                            test->apply_test(treap, stlStruct, EFunctions::INSERT);
                        else if(std::strcmp(function.c_str(), "assign") == 0)
                            test->apply_test(treap, stlStruct, EFunctions::ASSIGN);
                        else if(std::strcmp(function.c_str(), "next-permutation") == 0)
                            test->apply_test(treap, stlStruct, EFunctions::NEXT_PERMUTATION);
                        else if(std::strcmp(function.c_str(), "subsegment-sum") == 0)
                            test->apply_test(treap, stlStruct, EFunctions::SUBSEGMENT_SUM);
                    }
                    else if(command == "help")
                    {
                        std::cout << "Available commands:\n"
                                  << "quit, show-input, build-tree, insert, next-permutation, subsegment-sum, change-element, help, test, compare"
                                  << std::endl;
                    }
                    else
                        throw CExUnknownCommand();
                }
                catch(CException& e)
                {
                    e.PrintError();
                }
            }
        }
    }
    else
        std::cerr << "No filename specified" << std::endl;

    return 0;
}

