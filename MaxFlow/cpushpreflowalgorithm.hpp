/*
 * The implementation of push-preflow algorithm
 */

#ifndef CPUSHPREFLOWALGORITHM_H
#define CPUSHPREFLOWALGORITHM_H

#include "imaxflowalgorithm.hpp"

template<class CapacityType, class Vertex>
class CPushPreflowAlgorithm : public IMaxFlowAlgorithm<CapacityType, Vertex> {
    class CVertexExtension : public IVertexExtension {
    public:
        CVertexExtension();

        ~CVertexExtension() {
            if(ve_excessFlow)
                delete ve_excessFlow;
        }

    private:
        std::size_t ve_vertexHeight;
        CapacityType* ve_excessFlow;
    };

public:
    CPushPreflowAlgorithm();
    CapacityType* getMaxFlowValue();
    std::vector<Vertex>* getMaxFlow();
    ~CPushPreflowAlgorithm() {}
};

template<class CapacityType, class Vertex>
CPushPreflowAlgorithm<CapacityType, Vertex>::CPushPreflowAlgorithm() {

}

template<class CapacityType, class Vertex>
CapacityType* CPushPreflowAlgorithm<CapacityType, Vertex>::getMaxFlowValue() {

}

template<class CapacityType, class Vertex>
std::vector<Vertex>* CPushPreflowAlgorithm<CapacityType, Vertex>::getMaxFlow() {

}

#endif // CPUSHPREFLOWALGORITHM_H
