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

class CPath
{
private:
    size_t p_arrival_time;
public:
    CPath(const size_t arrival) : p_arrival_time(arrival) {}
    CPath add(CEdge* edge);
    bool operator <(const CPath& path);
};

CPath CPath::add(CEdge* edge) //TODO
{
    size_t departure = edge->e_departure;
}

bool CPath::operator <(const CPath& path)
{
    return p_arrival_time < path.p_arrival_time;
}

//TODO: add schedule class
