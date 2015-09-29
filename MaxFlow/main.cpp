#include <iostream>
#include "cpushpreflowalgorithm.hpp"
#include "cmkmalgorithm.hpp"
#include "flownetwork.hpp"

int main() {
    CFlowNetwork<int, int>* net = new CFlowNetwork<int, int>();
    net->getMaxFlow(new CPushPreflowAlgorithm<int, CFlowNetwork<int, int>::NetVertex>);

    return 0;
}

