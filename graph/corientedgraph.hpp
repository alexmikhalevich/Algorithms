#include "cpathfinder.hpp"

/*
 *the example of the oriented graph implementation
 */

class CPath;
class CEdge;
class CGraph;

class CEdge : public CAbstractEdge<CPath>
{
private:
    CEdge();
    size_t e_weight;
public:
    CEdge(size_t source_id, size_t destination_id, size_t weight) : CAbstractEdge(source_id, destination_id), e_weight(weight) {}
    friend class CPath;
};

class CPath
{
private:
    size_t p_length;
public:
    CPath(const size_t length = 0) : p_length(length) {}
    CPath add(CEdge* edge);
    bool operator <(const CPath& path);
};

class COrientedGraph
{
private:
    std::vector<std::vector<CEdge*> > g_edges;
public:
    std::vector<std::vector<CEdge*> > get() const;
    void add(CEdge* edge);
};

CPath CPath::add(CEdge* edge)
{
    return CPath(p_length + edge->e_weight);
}

bool CPath::operator <(const CPath& path)
{
    return p_length < path.p_length;
}

std::vector<std::vector<CEdge*> > COrientedGraph::get() const
{
    return g_edges;
}

void COrientedGraph::add(CEdge* edge)
{
    g_edges[edge->get_source()->get_id()].push_back(edge);
}
