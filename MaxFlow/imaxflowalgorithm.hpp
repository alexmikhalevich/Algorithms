#ifndef IMAXFLOWALGORITHM
#define IMAXFLOWALGORITHM

#include <vector>
#include "ialgorithm.h"
#include "ccompare.hpp"

template<class CapacityType, class CompareFunction>
class IMaxFlowAlgorithm : public IAlgorithm<CompareFunction> {
public:
       virtual CapacityType* getMaxFlowCapacity()  = 0;
       virtual std::vector<std::size_t>* getMaxFlow() = 0;
};

#endif // IMAXFLOWALGORITHM

