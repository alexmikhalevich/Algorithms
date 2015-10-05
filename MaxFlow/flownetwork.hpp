#ifndef CFLOWNETWORK_HPP
#define CFLOWNETWORK_HPP

#include <iostream>
#include <assert.h>
#include <vector>
#include "ialgorithm.h"

class CFlowNetwork {
private:
       std::vector<std::vector<int> > fn_edges;

public:
       CFlowNetwork() {}

       template<class CompareFunction>
       void applyAlgorithm(IAlgorithm<CompareFunction>* algorithm, CompareFunction func, std::istream& stream) {
              algorithm->applyAlgorithm(fn_edges.size(), func, stream);
       }

       void read(std::istream& stream) {
              std::size_t numOfVertices, numOfEdges;
              stream >> numOfVertices >> numOfEdges;

              fn_edges.resize(numOfVertices);
              for(std::size_t i = 0; i < numOfVertices; ++i) {
                     fn_edges[i].resize(numOfVertices);
                     for(std::size_t j = 0; j < numOfVertices; ++j) {
                            fn_edges[i][j] = -1;
                     }
              }

              for(std::size_t i = 0; i < numOfEdges; ++i) {
                     std::size_t sourceVertIndex, destinationVertIndex;
                     stream >> sourceVertIndex >> destinationVertIndex;

                     assert(sourceVertIndex <= numOfVertices);
                     assert(destinationVertIndex <= numOfVertices);                        //TODO: replace asserts with exceptions

                     fn_edges[sourceVertIndex][destinationVertIndex] = 1;
              }
       }
};
#endif // CFLOWNETWORK_HPP
