#include <iostream>
#include <fstream>
#include "crelabeltofrontalgorithm.hpp"
#include "cmkmalgorithm.hpp"
#include "flownetwork.hpp"

int main() {
       std::ifstream in("input.dat");
       CFlowNetwork* net = new CFlowNetwork();
       net->read(in);
       net->applyAlgorithm(new CRelabelToFrontAlgorithm<int, std::less<int> >, std::less<int>(), in);

       delete net;

       return 0;
}
