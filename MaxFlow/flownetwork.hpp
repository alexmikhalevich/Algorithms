#ifndef CFLOWNETWORK_HPP
#define CFLOWNETWORK_HPP

#include "imaxflowalgorithm.hpp"
#include <iostream>

template<typename VertexKeyType, typename CapacityType>
class CFlowNetwork {
public:
    class NetVertex {
    public:
        NetVertex() {
            nv_vertexExtension = NULL;
        }

        ~NetVertex() {
            if(nv_vertexExtension)
                delete nv_vertexExtension;
        }

        void InitExtension(IVertexExtension* extension) {
            nv_vertexExtension = extension;
        }

        VertexKeyType* getKey() const {
            return nv_key;
        }

    private:
        VertexKeyType* nv_key;
        IVertexExtension* nv_vertexExtension;
    };

    class NetEdge {
    public:
        NetEdge();

        CapacityType* getCapacity() const {
            return ne_capacity;
        }

        CapacityType* getFlow() const {
            return ne_flow;
        }
    private:
        CapacityType* ne_capacity;
        CapacityType* ne_flow;
    };

     CapacityType* getMaxFlowCapacity(IMaxFlowAlgorithm<CapacityType, NetVertex>*) {}
     std::vector<NetVertex*>* getMaxFlow(IMaxFlowAlgorithm<CapacityType, NetVertex>*) {}
};

#endif // CFLOWNETWORK_HPP

