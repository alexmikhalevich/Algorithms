#ifndef GAS_STATIONS
#define GAS_STATIONS

#include "cpathfinder.hpp"

namespace GasStations
{
    class CPath;
    class CEdge;
    class CGasStations;

    class CPath
    {
    private:
        size_t p_amount;
    public:
        CPath(const size_t amount = 0) : p_amount(amount) {}
        CPath add(CEdge* edge);
        bool operator <(const CPath& path);
        size_t get_path_length() const;
    };

    class CEdge : public CAbstractEdge<CPath>
    {
    private:
        CEdge();
        size_t e_weight;
    public:
        CEdge(const int source_id, const int destination_id, const size_t weight) : CAbstractEdge<CPath>(source_id, destination_id), e_weight(weight) {}
        friend class CPath;
    };

    class CGasStations
    {
    private:
        CGasStations();
        std::vector<std::vector<CEdge*> > gs_edges;
    public:
        CGasStations(const int vertice_amount) : gs_edges(vertice_amount) {}
        std::vector<CEdge*> get(const int vertice_id) const;
        void add(const size_t alpha_id, const size_t beta_id, const size_t alpha_weight, const size_t beta_weight);
    };

    size_t CPath::get_path_length() const
    {
        return p_amount;
    }

    CPath CPath::add(CEdge* edge)
    {
        return CPath(p_amount + edge->e_weight);
    }

    bool CPath::operator <(const CPath& path)
    {
        return p_amount < path.p_amount;
    }

    std::vector<CEdge*> CGasStations::get(const int vertice_id) const
    {
        return gs_edges.at(vertice_id);
    }

    void CGasStations::add(const size_t alpha_id, const size_t beta_id, const size_t alpha_weight, const size_t beta_weight)
    {
        gs_edges[alpha_id].push_back(new CEdge(alpha_id, beta_id, alpha_weight));
        gs_edges[beta_id].push_back(new CEdge(beta_id, alpha_id, beta_weight));
    }
} //GasStations

#endif //GAS_STATIONS
