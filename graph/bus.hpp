#include "cpathfinder.hpp"

class CPath;
class CEdge;
class CSchedule;

class CPath
{
private:
    CPath();
    size_t p_arrival_time;
public:
    CPath(const size_t arrival) : p_arrival_time(arrival) {}
    CPath add(CEdge* edge);
    bool operator <(const CPath& path);
};

class CEdge : public CAbstractEdge<CPath>
{
private:
    CEdge();
    size_t e_departure;
    size_t e_arrival;
public:
    CEdge(const int source_id, const int destination_id, const size_t departure, const size_t arrival) :
        CAbstractEdge(source_id, destination_id), e_departure(departure), e_arrival(arrival) {}
    friend class CPath;
};

class CSchedule
{
private:
    std::vector<std::vector<CEdge*> > s_edges;
public:
    std::vector<std::vector<CEdge*> > get() const;
    void add(CEdge* edge);
};

CPath CPath::add(CEdge* edge) //TODO
{
    size_t departure = edge->e_departure;
}

bool CPath::operator <(const CPath& path)
{
    return p_arrival_time < path.p_arrival_time;
}

std::vector<std::vector<CEdge*> > CSchedule::get() const
{
    return s_edges;
}

void CSchedule::add(CEdge* edge)
{
    s_edges[edge->get_source()->get_id()].push_back(edge);
}
