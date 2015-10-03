#ifndef IALGORITHM
#define IALGORITHM

#include <vector>
#include <istream>

template<class CompareFunction>
class IAlgorithm {
public:
       virtual void applyAlgorithm(const std::vector<std::vector<int> >& edges, CompareFunction func, std::istream& stream) = 0;
};

#endif // IALGORITHM

