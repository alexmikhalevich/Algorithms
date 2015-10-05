#ifndef IALGORITHM
#define IALGORITHM

#include <vector>
#include <istream>

template<class CompareFunction>
class IAlgorithm {
public:
       virtual void applyAlgorithm(const std::size_t vertices_size, const std::size_t edges_size, CompareFunction func, std::istream& stream) = 0;
};

#endif // IALGORITHM

