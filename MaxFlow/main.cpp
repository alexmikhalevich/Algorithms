#include <fstream>
#include "flownetwork.hpp"
#include "crelabeltofrontalgorithm.hpp"

int main() {
       std::ifstream in("input.dat");
       CFlowNetwork* net = new CFlowNetwork();
       CRelabelToFrontAlgorithm<int, std::less<int> >* algorithm = new CRelabelToFrontAlgorithm<int, std::less<int> >();
       net->read(in);
       net->applyAlgorithm(algorithm, std::less<int>(), in);
       std::cout << algorithm->getMaxFlowCapacity() << std::endl;

       delete net;
       delete algorithm;

       return 0;
}
