#include <iostream>
#include <fstream>
#include "cpushpreflowalgorithm.hpp"
#include "cmkmalgorithm.hpp"
#include "flownetwork.hpp"

int main() {
       std::ifstream in("input.dat");
       CFlowNetwork<int, std::less<int> >* net = new CFlowNetwork<int, std::less<int> >(std::less<int>());
       net->read(in);
       net->getMaxFlowCapacity(new CPushPreflowAlgorithm<int, std::less<int>,
                               CFlowNetwork<int, std::less<int> >::NetVertex, CFlowNetwork<int, std::less<int> >::NetEdge>);

       return 0;
}
