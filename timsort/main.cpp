#include <string>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <cstring>
#include "timsort.hpp"

#define SMALL_ARRAY_SIZE 47
#define BIG_ARRAY_SIZE_1 56
#define BIG_ARRAY_SIZE_2 120
#define BIG_ARRAY_SIZE_3 11345
#define BIG_ARRAY_SIZE_4 54890
#define BIG_ARRAY_SIZE_5 100000
#define PARTIALLY_SORTED_ARRAY_SIZE_1 20
#define PARTIALLY_SORTED_ARRAY_SIZE_2 40
#define PARTIALLY_SORTED_ARRAY_SIZE_3 80
#define PARTIALLY_SORTED_ARRAY_SIZE_4 128
#define PARTIALLY_SORTED_ARRAY_SIZE_5 1024
#define PARTIALLY_SORTED_ARRAY_SIZE_6 4096
#define NUM_OF_ARRAYS_1 2
#define NUM_OF_ARRAYS_2 4
#define NUM_OF_ARRAYS_3 10
#define NUM_OF_ARRAYS_4 100
#define NUM_OF_ARRAYS_5 1000
#define NUM_OF_ARRAYS_6 10000
#define MAX_NUM 1000
#define MAX_STRING_LENGTH 128

bool StringCompare(std::string const &sX, std::string const &sY)
{
    if(sX.length() < sY.length())
        return true;
    else
        return false;
}

void PrintTime(clock_t const start, clock_t const end, std::ofstream& out, std::string const &sort)
{
    unsigned int result = end - start;
    out << "Working time (" << sort << "): " << result << "ms" << std::endl;
}

void GenerateRandomIntArray(std::vector<int> &resultVector, unsigned const int size)
{
    for(unsigned int i = 0; i < size; ++i)
    {
        int temp = rand() % MAX_NUM;
        resultVector.push_back(temp);
    }
}

void GenerateRandomStringArray(std::vector<std::string> &resultVector, unsigned const int size)
{
    std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for(unsigned int j = 0; j < size; ++j)
    {
        std::string tempString;
        for(int i = 0; i < rand() % MAX_STRING_LENGTH; ++i)
            tempString.push_back(alphabet.at(rand() % alphabet.length()));
        resultVector.push_back(tempString);
    }
}

void GeneratePartiallySortedStringArray(std::vector<std::string> &resultVector, unsigned const int size)
{
    GenerateRandomStringArray(resultVector, size);
    std::sort(resultVector.begin(), resultVector.end(), StringCompare);

    for(std::vector<std::string>::iterator start = resultVector.begin(); start != resultVector.end(); ++start)
        if((rand() % 100 <= 20)/*probability of swapping 20%*/ && start != (resultVector.end() - 1))
            std::iter_swap(start, start + 1);
}

void GeneratePartiallySortedIntArray(std::vector<int> &resultVector, unsigned const int size)
{
    const int halfSize = size/2;

    int start = rand() % MAX_NUM;
    for(int i = 0; i < halfSize; ++i)
        resultVector.push_back(start + i);

    start = rand() % MAX_NUM;
    for(unsigned int i = halfSize; i < size; ++i)
        resultVector.push_back(start - i);

    for(std::vector<int>::iterator start = resultVector.begin(); start != resultVector.end(); ++start)
        if((rand()%100 <= 10)/*probability of swapping 10%*/ && start != (resultVector.end() - 1))
            std::iter_swap(start, start + 1);
}

void GenerateArraysSequence(std::vector<std::vector<int> > &resultVector, unsigned const int size)
{
    int arraySizes[] = {PARTIALLY_SORTED_ARRAY_SIZE_1,
                        PARTIALLY_SORTED_ARRAY_SIZE_2,
                        PARTIALLY_SORTED_ARRAY_SIZE_3,
                        PARTIALLY_SORTED_ARRAY_SIZE_4,
                        PARTIALLY_SORTED_ARRAY_SIZE_5,
                        PARTIALLY_SORTED_ARRAY_SIZE_6};

    for(unsigned int i = 0; i < size; ++i)
    {
        std::vector<int> tempVector;
        GeneratePartiallySortedIntArray(tempVector, arraySizes[rand() % 6]);
        resultVector.push_back(tempVector);
    }
}

void TestIntArraySort(std::ofstream& out, unsigned int const size, bool const isRandom = true)
{
    CTimsortParamsDefault defaultParameters;
    if(isRandom)
    {
        clock_t timeBeginTimsort, timeEndTimsort, timeBeginStdsort, timeEndStdsort = 0;

        out << "Random array of " << size << " elements" << std::endl;
        std::vector<int> inputVector;
        GenerateRandomIntArray(inputVector, size);
        std::vector<int> cloneVector = inputVector;

        timeBeginTimsort = clock();
        Timsort<std::vector<int>::iterator, std::less<int> >(inputVector.begin(), inputVector.end(), std::less<int>(), &defaultParameters);
        timeEndTimsort = clock();

        timeBeginStdsort = clock();
        std::sort(cloneVector.begin(), cloneVector.end(), std::less<int>());
        timeEndStdsort = clock();

        PrintTime(timeBeginTimsort, timeEndTimsort, out, "Timsort");
        PrintTime(timeBeginStdsort, timeEndStdsort, out, "std::sort");
        out << std::endl << std::endl;
    }
    else
    {
        clock_t timeBeginTimsort, timeEndTimsort, timeBeginStdsort, timeEndStdsort = 0;

        out << "Array of " << size << " partially sorted arrays" << std::endl;
        std::vector<std::vector<int> > arrayVector;
        GenerateArraysSequence(arrayVector, size);
        for(std::vector<std::vector<int> >::iterator start = arrayVector.begin(); start != arrayVector.end(); ++start)
        {
            out << "Partially sorted array number " << (start - arrayVector.begin()) + 1 << " of "
                << start->size() << " elements" << std::endl;

            std::vector<int> cloneVector = *start;
            timeBeginTimsort = clock();
            Timsort<std::vector<int>::iterator, std::less<int> >(start->begin(), start->end(), std::less<int>(), &defaultParameters);
            timeEndTimsort = clock();

            timeBeginStdsort = clock();
            std::sort(cloneVector.begin(), cloneVector.end(), std::less<int>());
            timeEndStdsort = clock();

            PrintTime(timeBeginTimsort, timeEndTimsort, out, "Timsort");
            PrintTime(timeBeginStdsort, timeEndStdsort, out, "std::sort");
            out << std::endl << std::endl;
        }
    }
}

void TestStringArraySort(std::ofstream& out, unsigned int const size, bool const isRandom = true)
{
    CTimsortParamsDefault defaultParameters;

    if(isRandom)
    {
        clock_t timeBeginTimsort, timeEndTimsort, timeBeginStdsort, timeEndStdsort = 0;

        out << "Random string array of " << size << " elements" << std::endl;
        std::vector<std::string> inputVector;
        GenerateRandomStringArray(inputVector, size);
        std::vector<std::string> cloneVector = inputVector;

        timeBeginTimsort = clock();
        Timsort<std::vector<std::string>::iterator, bool (*)(std::string const&, std::string const&)>(inputVector.begin(),
                                                                                                      inputVector.end(),
                                                                                                      StringCompare, &defaultParameters);
        timeEndTimsort = clock();

        timeBeginStdsort = clock();
        std::sort(cloneVector.begin(), cloneVector.end(), StringCompare);
        timeEndStdsort = clock();

        PrintTime(timeBeginTimsort, timeEndTimsort, out, "Timsort");
        PrintTime(timeBeginStdsort, timeEndStdsort, out, "std::sort");
        out << std::endl << std::endl;
    }
    else
    {
        clock_t timeBeginTimsort, timeEndTimsort, timeBeginStdsort, timeEndStdsort = 0;

        out << "Partially sorted string array of " << size << " elements" << std::endl;
        std::vector<std::string> inputVector;
        GeneratePartiallySortedStringArray(inputVector, size);
        std::vector<std::string> cloneVector = inputVector;

        timeBeginTimsort = clock();
        Timsort<std::vector<std::string>::iterator, bool (*)(std::string const&, std::string const&)>(inputVector.begin(),
                                                                                                      inputVector.end(),
                                                                                                      StringCompare, &defaultParameters);
        timeEndTimsort = clock();

        timeBeginStdsort = clock();
        std::sort(cloneVector.begin(), cloneVector.end(), StringCompare);
        timeEndStdsort = clock();

        PrintTime(timeBeginTimsort, timeEndTimsort, out, "Timsort");
        PrintTime(timeBeginStdsort, timeEndStdsort, out, "std::sort");
        out << std::endl << std::endl;
    }
}

int main()
{
    srand(time(NULL));
    std::ofstream out_numbers("output_numbers.txt");
    std::ofstream out_strings("output_strings.txt");

    //std::sort beats Timsort (~2 times)
    TestIntArraySort(out_numbers, SMALL_ARRAY_SIZE);
    TestIntArraySort(out_numbers, BIG_ARRAY_SIZE_1);
    TestIntArraySort(out_numbers, BIG_ARRAY_SIZE_2);
    TestIntArraySort(out_numbers, BIG_ARRAY_SIZE_3);
    TestIntArraySort(out_numbers, BIG_ARRAY_SIZE_4);
    TestIntArraySort(out_numbers, BIG_ARRAY_SIZE_5);

    TestIntArraySort(out_numbers, NUM_OF_ARRAYS_1, false);
    TestIntArraySort(out_numbers, NUM_OF_ARRAYS_2, false);
    TestIntArraySort(out_numbers, NUM_OF_ARRAYS_3, false);
    TestIntArraySort(out_numbers, NUM_OF_ARRAYS_4, false);
    TestIntArraySort(out_numbers, NUM_OF_ARRAYS_5, false); //Timsort beats std::sort (~3 times)
    TestIntArraySort(out_numbers, NUM_OF_ARRAYS_6, false); //Timsort beats std::sort (~4 times)

    //std::sort beats Timsort (~1.2 times)
    TestStringArraySort(out_strings, SMALL_ARRAY_SIZE);
    TestStringArraySort(out_strings, BIG_ARRAY_SIZE_1);
    TestStringArraySort(out_strings, BIG_ARRAY_SIZE_2);

    //Timsort beats std::sort (~3 times)
    TestStringArraySort(out_strings, SMALL_ARRAY_SIZE, false);
    TestStringArraySort(out_strings, BIG_ARRAY_SIZE_1, false);
    TestStringArraySort(out_strings, BIG_ARRAY_SIZE_2, false);

    return 0;
}
