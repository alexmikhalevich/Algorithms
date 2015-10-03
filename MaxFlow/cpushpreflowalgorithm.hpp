/*
 * The implementation of push-preflow algorithm
 * Assuming vertices.begin - source, vertices.end - sink
 */

#ifndef CPUSHPREFLOWALGORITHM_H
#define CPUSHPREFLOWALGORITHM_H

#include <iostream>
#include <assert.h>
#include <limits>
#include <istream>
#include "imaxflowalgorithm.hpp"
#include "cbaseedge.h"
#include "cbasevertex.h"

template<class CapacityType, class CompareFunction>
class CPushPreflowAlgorithm : public IMaxFlowAlgorithm<CapacityType, CompareFunction> {
private:
       class CVertex : public CBaseVertex {
       private:
              std::size_t v_id;
              std::size_t v_height;
              CapacityType* v_excessFlow;
       public:
              CVertex(const std::size_t id) : CBaseVertex () {
                     v_id = id;
                     v_height = 0;
                     v_excessFlow = new CapacityType();
              }

              ~CVertex() {
                     if(v_excessFlow)
                            delete v_excessFlow;
              }

              std::size_t getId() const {
                     return v_id;
              }

              void setExcessFlow(const CapacityType& flow) {
                     *v_excessFlow = flow;
              }

              CapacityType& getExcessFlow() const {
                     return &v_excessFlow;
              }

              void setHeight(const std::size_t height) {
                     v_height = height;
              }

              std::size_t getHeight() const {
                     return v_height;
              }
       };

       class CEdge : public CBaseEdge {
       private:
              CapacityType* e_flow;
              CapacityType* e_capacity;
       public:
              CEdge(const std::size_t source, const std::size_t destination, CapacityType* flow, CapacityType* capacity)
                     : CBaseEdge(source, destination) {
                     e_flow = flow;
                     e_capacity =  capacity;
              }

              void setFlow(const CapacityType& flow) {
                     e_flow = &flow;
              }

              CapacityType& getFlow() const {
                     return &e_flow;
              }

              void setCapacity(const CapacityType& capacity) {
                     *e_capacity = capacity;
              }

              CapacityType& getCapacity() const {
                     return *e_capacity;
              }
       };

       CCompare<CapacityType, CompareFunction>* ppa_comparator;
       std::vector<std::vector<CEdge*> > ppa_residualForwardEdges;
       std::vector<std::vector<CEdge*> > ppa_residualBackwardEdges;
       std::vector<CVertex*> ppa_residualVertices;

       void Push(const std::size_t firstVertexId, const std::size_t secondVertexId) {
              CapacityType minPossibleFlow = ppa_comparator->less(ppa_residualVertices[firstVertexId]->getExcessFlow(),
                                                                  ppa_residualForwardEdges[firstVertexId][secondVertexId]->getCapacity())
                            ? ppa_residualVertices[firstVertexId]->getExcessFlow()
                            : ppa_residualForwardEdges[firstVertexId][secondVertexId]->getCapacity();

              ppa_residualVertices[firstVertexId]->setExcessFlow(ppa_residualVertices[firstVertexId]->getExcessFlow() - minPossibleFlow);
              ppa_residualVertices[secondVertexId]->setExcessFlow(ppa_residualVertices[secondVertexId]->getExcessFlow()  +  minPossibleFlow);

              ppa_residualForwardEdges[firstVertexId][secondVertexId] -= minPossibleFlow;
              ppa_residualBackwardEdges[secondVertexId][firstVertexId] += minPossibleFlow;
       }

       void Relabel(const std::size_t vertexId) {
              std::size_t minimum = std::numeric_limits<std::size_t>::max();

              for(std::size_t i = 0; i < ppa_residualVertices.size(); ++i) {
                     if(ppa_residualForwardEdges[vertexId][i] && (ppa_residualVertices[vertexId]->getHeight() < minimum))
                            minimum = ppa_residualVertices[vertexId]->getHeight();
              }

              ppa_residualVertices[vertexId]->setHeight(minimum + 1);
       }

       void Discharge(const std::size_t vertexId) {
              std::size_t vId = 0;
              while(ppa_comparator->less(0, ppa_residualVertices[vertexId]->getExcessFlow())) {
                     if(ppa_residualForwardEdges[vertexId][vId] && (ppa_residualVertices[vertexId] == ppa_residualVertices[vId] + 1)) {
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

       void Init(const std::vector<std::vector<int> >& edges, std::istream& stream) {
              ppa_residualForwardEdges.resize(edges.size());
              ppa_residualBackwardEdges.resize(edges.size());
              ppa_residualVertices.resize(edges.size());

              for(std::size_t i = 0; i < edges.size(); ++i) {
                     ppa_residualForwardEdges[i].resize(edges.size());
                     ppa_residualBackwardEdges[i].resize(edges.size());
                     ppa_residualVertices[i] = NULL;
                     for(std::size_t j = 0; j < edges.size(); ++j) {
                            ppa_residualForwardEdges[i][j] = NULL;
                            ppa_residualBackwardEdges[i][j] = NULL;
                     }
              }

              for(std::size_t i = 0; i < edges.size(); ++i) {
                     std::size_t sourceVertex, destinationVertex;
                     CapacityType capacity;
                     stream >> sourceVertex >> destinationVertex >> capacity;

                     assert(sourceVertex != destinationVertex);                            //TODO: replace with exceptions
                     assert(sourceVertex >= 0);
                     assert(destinationVertex >= 0);
                     assert(sourceVertex < ppa_residualVertices.size());
                     assert(destinationVertex < ppa_residualVertices.size());

                     ppa_residualForwardEdges[sourceVertex][destinationVertex] = new CEdge(sourceVertex, destinationVertex, new CapacityType, &capacity);
                     ppa_residualBackwardEdges[destinationVertex][sourceVertex] = new CEdge(destinationVertex, sourceVertex, new CapacityType,  &capacity);
                     if(ppa_residualVertices[sourceVertex] == NULL)
                            ppa_residualVertices[sourceVertex] = new CVertex(sourceVertex);
                     if(ppa_residualVertices[destinationVertex] == NULL)
                            ppa_residualVertices[destinationVertex] = new CVertex(destinationVertex);
              }

              for(std::size_t i = 1; i < ppa_residualVertices.size(); ++i) {               //i != 0 - ignoring source
                     ppa_residualVertices[i]->setExcessFlow(ppa_residualForwardEdges[0][i]->getCapacity());
                     ppa_residualBackwardEdges[i][0]->setCapacity(ppa_residualForwardEdges[0][i]->getCapacity()
                                   + ppa_residualBackwardEdges[i][0]->getCapacity());
              }
              ppa_residualVertices[0]->setHeight(ppa_residualVertices.size());
       }
public:
       CPushPreflowAlgorithm() {
              ppa_comparator = NULL;
       }

       ~CPushPreflowAlgorithm() {
              if(ppa_comparator)
                     delete ppa_comparator;
       }

       void applyAlgorithm(const std::vector<std::vector<int> >& edges, CompareFunction func, std::istream& stream) {                        //TODO: maybe pass just size of edges vector here
              ppa_comparator = new CCompare<CapacityType, CompareFunction>(func);
              Init(edges, stream);
       }

       CapacityType* getMaxFlowCapacity() {
       }

       std::vector<std::size_t>* getMaxFlow() {
       }
};

#endif // CPUSHPREFLOWALGORITHM_H
