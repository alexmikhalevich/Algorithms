/*
 * The implementation of the Malhotra-Kumar-Maheshwari algorithm
 */

#ifndef CMKMALGORITHM
#define CMKMALGORITHM

#include "imaxflowalgorithm.hpp"

template<class CapacityType, class CompareFucntion, class Vertex, class Edge>
class CMKMAlgorithm : public IMaxFlowAlgorithm<CapacityType, CompareFucntion, Vertex, Edge> {
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
       CMKMAlgorithm() {}
       ~CMKMAlgorithm() {}

       CapacityType* getMaxFlowCapacity(std::vector<std::vector<Edge*> > edges,
                                        std::vector<Vertex*> vertices, CompareFucntion func) {

       }

       std::vector<Vertex>* getMaxFlow(std::vector<std::vector<Edge*> > edges,
                                       std::vector<Vertex*> vertices, CompareFucntion func) {

       }
};

#endif // CMKMALGORITHM

