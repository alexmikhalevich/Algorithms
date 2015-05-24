#include <iostream>
#include "cpathfinder.hpp"

/*
 *the example of the oriented graph implementation
 */
namespace OrientedGraph
{
class CPath;
class CEdge;
class COrientedGraph;

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
    size_t get_path_length() const;
};

class COrientedGraph
{
private:
    std::vector<std::vector<CEdge*> > g_edges;
public:
    COrientedGraph(const int vertice_amount) : g_edges(vertice_amount) {}   //NOTE #1.2: possible solution for bug #1
    std::vector<CEdge*> get(const int vertice_id) const;
    void add(CEdge* edge);
};

size_t CPath::get_path_length() const
{
    return p_length;
}

CPath CPath::add(CEdge* edge)
{
    return CPath(p_length + edge->e_weight);
}

bool CPath::operator <(const CPath& path)
{
    return p_length < path.p_length;
}

std::vector<CEdge*> COrientedGraph::get(const int vertice_id) const
{
    return g_edges.at(vertice_id);
}

void COrientedGraph::add(CEdge* edge)
{
    g_edges[edge->get_source()->get_id()].push_back(edge); //BUG #1.2: out of range
}
} //OrientedGraph
