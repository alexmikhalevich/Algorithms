/*
 * The implementation of the Malhotra-Kumar-Maheshwari algorithm
 */

#ifndef CMKMALGORITHM
#define CMKMALGORITHM

#include "imaxflowalgorithm.hpp"

template<class CapacityType, class Vertex>
class CMKMAlgorithm : public IMaxFlowAlgorithm<CapacityType, Vertex> {
    class CVertexExtension : public IVertexExtension {
    public:
        CVertexExtension();

        ~CVertexExtension() {
            if(ve_potential)
                delete ve_potential;
        }

    private:
        CapacityType* ve_potential;
    };

public:
    CMKMAlgorithm();
    CapacityType* getMaxFlowValue();
    std::vector<Vertex>* getMaxFlow();
    ~CMKMAlgorithm();
};

template<class CapacityType, class Vertex>
CapacityType* CMKMAlgorithm<CapacityType, Vertex>::getMaxFlowValue() {

}

template<class CapacityType, class Vertex>
std::vector<Vertex>* CMKMAlgorithm<CapacityType, Vertex>::getMaxFlow() {

}

#endif // CMKMALGORITHM

