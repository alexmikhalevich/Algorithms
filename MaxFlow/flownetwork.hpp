#ifndef CFLOWNETWORK_HPP
#define CFLOWNETWORK_HPP

#include <iostream>
#include <assert.h>
#include "imaxflowalgorithm.hpp"

template<class CapacityType, class CompareFunction>
class CFlowNetwork {
public:
       class NetVertex;
       class NetEdge;
private:
       std::vector<std::vector<NetEdge*> > fn_edges;
       std::vector<NetVertex*> fn_vertices;
       CompareFunction fn_compareFunc;
public:
       class NetVertex {
       private:
              IVertexExtension* nv_extension;
       public:
              NetVertex() {
                     nv_extension = NULL;
              }

              ~NetVertex() {
                     if(nv_extension)
                            delete nv_extension;
              }

              void initExtension(IVertexExtension* extension) {
                     nv_extension = extension;
              }

              IVertexExtension* getExtension() const {
                     return  nv_extension;
              }
       };

       class NetEdge {                           //TODO: remove edge extension, using it may cause undefined behavior
       private:
              CapacityType* ne_capacity;
              CapacityType* ne_flow;
              IEdgeExtension* ne_extension;
       public:
              NetEdge(CapacityType* capacity, CapacityType* flow) : ne_capacity(capacity), ne_flow(flow) {
                     ne_extension = NULL;
              }

              ~NetEdge() {
                     if(ne_extension != NULL)
                            delete ne_extension;
              }

              CapacityType getCapacity() const {
                     return *ne_capacity;
              }

              CapacityType getFlow() const {
                     return *ne_flow;
              }

              void setCapacity(CapacityType capacity) {
                     ne_capacity = &capacity;
              }

              void setFlow(CapacityType flow) {
                     ne_flow = &flow;
              }

              void initExtension(IEdgeExtension* extension) {
                     ne_extension = extension;
              }

              IEdgeExtension* getExtension() const {
                     return ne_extension;
              }
       };

       CFlowNetwork(CompareFunction func) {
              fn_compareFunc = func;
       }

       CapacityType* getMaxFlowCapacity(IMaxFlowAlgorithm<CapacityType, CompareFunction, NetVertex, NetEdge>* algorithm) {
              return algorithm->getMaxFlowCapacity(fn_edges, fn_vertices, fn_compareFunc);
       }

       std::vector<NetVertex>* getMaxFlow(IMaxFlowAlgorithm<CapacityType, CompareFunction, NetVertex, NetEdge>* algorithm) {
              return algorithm->getMaxFlow(fn_edges, fn_vertices, fn_compareFunc);
       }

       virtual void read(std::istream& stream) {
              int numOfVertices, numOfEdges;
              stream >> numOfVertices >> numOfEdges;

              numOfVertices += 2;         //2 for source and sink
              fn_vertices.resize(numOfVertices);
              fn_edges.resize(numOfVertices);
              for(std::size_t i = 0; i < numOfVertices; ++i) {
                     fn_vertices[i] = NULL;
                     fn_edges[i].resize(numOfVertices);
                     for(std::size_t j = 0; j < numOfVertices; ++j) {
                            fn_edges[i][j] = NULL;
                     }
              }

              for(std::size_t i = 0; i < numOfEdges; ++i) {
                     std::size_t sourceVertIndex, destinationVertIndex;
                     CapacityType* edgeCapacity = new CapacityType;
                     stream >> sourceVertIndex >> destinationVertIndex;

                     assert(sourceVertIndex <= numOfVertices);
                     assert(destinationVertIndex <= numOfVertices);

                     stream >> *edgeCapacity;
                     fn_edges[sourceVertIndex][destinationVertIndex] = new NetEdge(edgeCapacity, new CapacityType);
                     if(fn_vertices.at(sourceVertIndex) == NULL)
                            fn_vertices[sourceVertIndex] = new NetVertex();
                     if(fn_vertices.at(destinationVertIndex) == NULL)
                            fn_vertices[destinationVertIndex] = new NetVertex();

                     assert(fn_vertices.at(sourceVertIndex) != NULL);
                     assert(fn_vertices.at(destinationVertIndex) != NULL);
              }

              fn_vertices[0] = new NetVertex();
              *(fn_vertices.end() - 1) = new NetVertex();

              for(std::size_t i = 0; i < numOfVertices; ++i) {
                     fn_edges[0][i] = new NetEdge(NULL, NULL);
                     fn_edges[i][0] = new NetEdge(NULL, NULL);
              }
       }
};
#endif // CFLOWNETWORK_HPP
