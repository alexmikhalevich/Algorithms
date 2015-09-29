#ifndef IMAXFLOWALGORITHM
#define IMAXFLOWALGORITHM

#include <vector>
#include "ivertexextension.h"

template<class CapacityType, class Vertex>
class IMaxFlowAlgorithm {
public:
    virtual CapacityType* getMaxFlowValue()  = 0;
    virtual std::vector<Vertex>* getMaxFlow() = 0;
};

#endif // IMAXFLOWALGORITHM

