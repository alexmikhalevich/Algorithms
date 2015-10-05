#include <fstream>
#include "flownetwork.hpp"
#include "crelabeltofrontalgorithm.hpp"

int main() {
       std::ifstream in("input.dat");
       CFlowNetwork* net = new CFlowNetwork();
       net->read(in);
       net->applyAlgorithm(new CRelabelToFrontAlgorithm<int, std::less<int> >, std::less<int>(), in);

       delete net;

       return 0;
}
