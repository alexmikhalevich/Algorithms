#ifndef IMAXFLOWALGORITHM
#define IMAXFLOWALGORITHM

#include <vector>
#include "ivertexextension.h"
#include "iedgeextension.h"
#include "ccompare.hpp"

template<class CapacityType, class CompareFunction, class Vertex, class Edge>
class IMaxFlowAlgorithm {
public:
       virtual CapacityType* getMaxFlowCapacity(std::vector<std::vector<Edge*> > edges,
                                                std::vector<Vertex*> vertices, CompareFunction func)  = 0;
       virtual std::vector<Vertex>* getMaxFlow(std::vector<std::vector<Edge*> > edges,
                                               std::vector<Vertex*> vertices, CompareFunction func) = 0;
};

#endif // IMAXFLOWALGORITHM

