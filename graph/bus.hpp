#ifndef BUS
#define BUS

#include "cpathfinder.hpp"

namespace BusProblem
{
    class CPath;
    class CEdge;
    class CSchedule;

    class CPath
    {
    private:
        size_t p_arrival_time;
    public:
        CPath(const size_t arrival = 0) : p_arrival_time(arrival) {}
        CPath add(CEdge* edge);
        bool operator <(const CPath& path);
        size_t get_path_length() const;
    };

    class CEdge : public CAbstractEdge<CPath>
    {
    private:
        size_t e_departure;
        size_t e_duration;
        size_t e_interval;
    public:
        CEdge(const int source_id, const int destination_id, const size_t departure, const size_t duration, const size_t interval) :
            CAbstractEdge(source_id, destination_id), e_departure(departure), e_duration(duration), e_interval(interval) {}
        friend class CPath;
    };

    class CSchedule
    {
    private:
        std::vector<std::vector<CEdge*> > s_edges;
    public:
        CSchedule(const int vertice_amount) : s_edges(vertice_amount) {}
        std::vector<CEdge*> get(const int vertice_id) const;
        void add(CEdge* edge);
    };

    size_t CPath::get_path_length() const
    {
        return p_arrival_time;
    }

    CPath CPath::add(CEdge* edge)
    {
        size_t departure = ((p_arrival_time - edge->e_departure) / edge->e_interval) * edge->e_interval;
        if(((p_arrival_time - departure) % edge->e_interval) != 0)
            departure += edge->e_interval;
        return CPath(departure + edge->e_duration);
    }

    bool CPath::operator <(const CPath& path)
    {
        return p_arrival_time < path.p_arrival_time;
    }

    std::vector<CEdge*> CSchedule::get(const int vertice_id) const
    {
        return s_edges.at(vertice_id);
    }

    void CSchedule::add(CEdge* edge)
    {
        s_edges[edge->get_source_id()].push_back(edge);
    }
} //BusProblem

#endif //BUS
