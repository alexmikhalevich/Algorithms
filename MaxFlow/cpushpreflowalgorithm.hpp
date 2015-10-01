/*
 * The implementation of push-preflow algorithm
 */

#ifndef CPUSHPREFLOWALGORITHM_H
#define CPUSHPREFLOWALGORITHM_H

#include <iostream>
#include <assert.h>
#include <limits>
#include "imaxflowalgorithm.hpp"

template<class CapacityType, class CompareFunction, class Vertex, class Edge>
class CPushPreflowAlgorithm : public IMaxFlowAlgorithm<CapacityType, CompareFunction, Vertex, Edge> {
private:
       CCompare<CapacityType, CompareFunction>* ppa_comparator;
       std::vector<std::vector<Edge*> > ppa_residualEdges;
       std::vector<Vertex*> ppa_residualVertices;

       void Push(const std::size_t firstVertexId, const std::size_t secondVertexId) {
              assert(ppa_comparator != NULL);

              CapacityType minPossibleFlow = ppa_comparator->less(*(dynamic_cast<CVertexExtension*>(ppa_residualVertices[firstVertexId]->getExtension())->getExcessFlow()),
                                                  *(ppa_residualEdges[firstVertexId][secondVertexId]->getCapacity())) ?
                                                  *(dynamic_cast<CVertexExtension*>(ppa_residualVertices[firstVertexId]->getExtension())->getExcessFlow())
                                                   : *(ppa_residualEdges[firstVertexId][secondVertexId]->getCapacity());
              dynamic_cast<CVertexExtension*>(ppa_residualVertices[firstVertexId]->getExtension())->setExcessFlow(
                                   dynamic_cast<CVertexExtension*>(ppa_residualVertices[firstVertexId]->getExtension())->getExcessFlow() - minPossibleFlow);
              dynamic_cast<CVertexExtension*>(ppa_residualVertices[secondVertexId]->getExtension())->setExcessFlow(
                                   dynamic_cast<CVertexExtension*>(ppa_residualVertices[secondVertexId]->getExtension())->getExcessFlow() - minPossibleFlow);
              ppa_residualEdges[firstVertexId][secondVertexId]->setCapacity(ppa_residualEdges[firstVertexId][secondVertexId]->getCapacity() - minPossibleFlow);
              ppa_residualEdges[secondVertexId][firstVertexId]->setCapacity(ppa_residualEdges[secondVertexId][firstVertexId]->getCapacity() + minPossibleFlow);
       }

       void Relabel(const std::size_t vertexId) {
              int minHeight = std::numeric_limits<int>::max();

              for(std::size_t i = 0; i < ppa_residualVertices.size(); ++i) {
                     if(ppa_residualEdges[vertexId][i] &&
                                   (dynamic_cast<CVertexExtension*>(ppa_residualVertices[i]->getExtension()))->getHeight() < minHeight) {
                            minHeight = dynamic_cast<CVertexExtension*>(ppa_residualVertices[i]->getExtension())->getHeight();
                     }
              }
              dynamic_cast<CVertexExtension*>(ppa_residualVertices[vertexId]->getExtension())->setHeight(minHeight + 1);
       }

       void Discharge(const std::size_t vertexId) {
              std::size_t vId = 0;
              while(dynamic_cast<CVertexExtension*>(ppa_residualVertices[vertexId]->getExtension())->getExcessFlow() > 0) {
                     if(ppa_residualEdges[vertexId][vId]
                                   && (dynamic_cast<CVertexExtension*>(ppa_residualVertices[vertexId]->getExtension()))->getHeight()
                                   == (dynamic_cast<CVertexExtension*>(ppa_residualVertices[vId]->getExtension()))->getHeight() + 1) {
                            Push(vertexId, vId);
                            vId = 0;
                            continue;
                     }
                     ++vId;
                     if(vId == ppa_residualVertices.size()) {
                            Relabel(vertexId);
                            vId = 0;
                     }
              }
       }

       class CVertexExtension : public IVertexExtension {
       private:
              std::size_t ve_vertexHeight;
              CapacityType* ve_excessFlow;
       public:
              CVertexExtension() {}

              ~CVertexExtension() {
                     if(ve_excessFlow)
                            delete ve_excessFlow;
              }

              std::size_t getHeight() const {
                     return ve_vertexHeight;
              }

              void setHeight(std::size_t height) {
                     ve_vertexHeight = height;
              }

              CapacityType getExcessFlow() const {
                     return ve_excessFlow;
              }

              void setExcessFlow(CapacityType* excesFlow) {
                     ve_excessFlow = excesFlow;
              }
       };

public:
       CPushPreflowAlgorithm() {
              ppa_comparator = NULL;
       }
       ~CPushPreflowAlgorithm() {}

       CapacityType* getMaxFlowCapacity(std::vector<std::vector<Edge*> > edges,                   //TODO: vectors should be given by pointer?
                                        std::vector<Vertex*> vertices, CompareFunction func) {
              ppa_comparator = new CCompare<CapacityType, CompareFunction>(func);
              ppa_residualVertices = vertices;                                 //NOTE: is not a real copy, vertices should be copied too for the rel copy

              for(std::size_t i = 0; i < ppa_residualVertices.size(); ++i) {
                     CVertexExtension* extension = new CVertexExtension();
                     ppa_residualVertices[i]->initExtension(extension);
              }

              ppa_residualEdges.resize(ppa_residualVertices.size());
              for(std::size_t i = 0; i < ppa_residualVertices.size(); ++i) {
                     ppa_residualEdges[i].resize(ppa_residualVertices.size());
                     for(std::size_t j = 0; j < ppa_residualVertices.size(); ++j) {
                            if(edges[i][j] != NULL) {                                      //FIXME: if edges[i][j] != NULL && edges[j][i] != NULL ???
                                   ppa_residualEdges[i][j] = new Edge(edges[i][j]->getCapacity(), edges[i][j]->getFlow());
                                   ppa_residualEdges[j][i] = new Edge(edges[i][j]->getCapacity(), edges[i][j]->getFlow());
                            }
                     }
              }

              for(std::size_t i = 0; i < ppa_residualVertices.size(); ++i) {
                     dynamic_cast<CVertexExtension*>(ppa_residualVertices[i]->getExtension())->setExcessFlow(ppa_residualEdges[0][i]->getCapacity());
                     ppa_residualEdges[i][0]->setCapacity(ppa_residualEdges[i][0]->getCapacity() + ppa_residualEdges[0][i]->getCapacity());
              }
              dynamic_cast<CVertexExtension*>(ppa_residualVertices[0]->getExtension())->setHeight(0);

              delete ppa_comparator;
       }

       std::vector<Vertex>* getMaxFlow(std::vector<std::vector<Edge*> > edges,
                                       std::vector<Vertex*> vertices, CompareFunction func) {

       }
};

#endif // CPUSHPREFLOWALGORITHM_H
