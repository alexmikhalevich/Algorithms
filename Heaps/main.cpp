#include "ctest.hpp"
#include <functional>
#include <iomanip>

std::vector<int> GenerateRandomData(const int dataSize)
{
    srand(time(NULL));
    std::vector<int> randomData;
    for(int i = 0; i < dataSize; ++i)
        randomData.push_back(std::rand() % std::numeric_limits<int>::max());
    return randomData;
}

int main()
{
    CTestBinomialHeap<int, std::less<int> >* binomialTest = new CTestBinomialHeap<int, std::less<int> >(std::less<int>());
    CTestSkewHeap<int, std::less<int> >* skewTest = new CTestSkewHeap<int, std::less<int> >(std::less<int>());
    CTestLeftistHeap<int, std::less<int> >* leftistTest = new CTestLeftistHeap<int, std::less<int> >(std::less<int>());

    std::vector<int> randomData = GenerateRandomData(BIG_DATA);

    std::setprecision(6);
    std::cout << "--------BINOMIAL TEST----------" << std::endl;
    binomialTest->ApplyTest(BIG_TEST, randomData);
    std::cout << "---------END OF TEST-----------" << std::endl;

    std::cout << "----------SKEW TEST------------" << std::endl;
    skewTest->ApplyTest(BIG_TEST, randomData);
    std::cout << "---------END OF TEST-----------" << std::endl;

    std::cout << "---------LEFTIST TEST----------" << std::endl;
    leftistTest->ApplyTest(BIG_TEST, randomData);
    std::cout << "---------END OF TEST-----------" << std::endl;

    delete binomialTest;
    delete skewTest;
    delete leftistTest;

    return 0;
}

