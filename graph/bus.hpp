#include "cpathfinder.hpp"

class CEdge : public CAbstractEdge
{
private:
    size_t e_departure;
    size_t e_arrival;
public:
    CEdge(const int source_id, const int destination_id, const size_t departure, const size_t arrival) :
        CAbstractEdge(source_id, destination_id), e_departure(departure), e_arrival(arrival) {}
};
