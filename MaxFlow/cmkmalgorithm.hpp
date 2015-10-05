/*
 * The implementation of the Malhotra-Kumar-Maheshwari algorithm
 */

#ifndef CMKMALGORITHM
#define CMKMALGORITHM

#include "imaxflowalgorithm.hpp"

template<class CapacityType, class CompareFucntion>
class CMKMAlgorithm : public IMaxFlowAlgorithm<CapacityType, CompareFucntion> {
public:
       CMKMAlgorithm() {}
       ~CMKMAlgorithm() {}

       CapacityType getMaxFlowCapacity() {
       }

       std::vector<std::size_t>* getMaxFlow() {
       }
};

#endif // CMKMALGORITHM

