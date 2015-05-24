#include <functional>
#include <vector>
#include <queue>
#include <algorithm>

enum EVerticeState
{
    NOT_VISITED,
    VISITED,
    BEST
};

class CExNoPath
{
public:
    virtual void print_error() = 0;
};

template<class Path>
class CVertice
{
private:
    int v_id;
    Path* v_best_path;
    EVerticeState v_state;
    CVertice* v_sibling;
public:
    CVertice(const int id) : v_id(id), v_best_path(NULL), v_state(NOT_VISITED), v_sibling(NULL) {}
    int get_id() const;
};

template<class Path>
class CAbstractEdge
{
private:
    CAbstractEdge();
    CVertice<Path>* ae_source;
    CVertice<Path>* ae_destination;
public:
    CAbstractEdge(const int source_id, const int destination_id) :
        ae_source(new CVertice<Path>(source_id)), ae_destination(new CVertice<Path>(destination_id)) {}
    CVertice<Path>* get_source() const;
    CVertice<Path>* get_destination() const;
};

template<class Edge, class Path, class GetEdgesFunction>
class CPathFinder
{
private:
    CPathFinder();
    GetEdgesFunction pf_get_edges;
    CVertice<Path>* pf_destination;
    CVertice<Path>* pf_source;
    std::queue<CVertice<Path>* > pf_queue;
    std::vector<CVertice<Path>* > pf_path;
    std::vector<CVertice<Path>* > get_path() const;
public:
    CPathFinder(GetEdgesFunction get_edges) : pf_get_edges(get_edges), pf_destination(NULL), pf_source(NULL) {}
    Path find_path(CVertice<Path>* source, CVertice<Path>* destination);
};

template<class Path>
int CVertice<Path>::get_id() const
{
    return v_id;
}

template<class Path>
CVertice<Path>* CAbstractEdge<Path>::get_source() const
{
    return ae_source;
}

template<class Path>
CVertice<Path>* CAbstractEdge<Path>::get_destination() const
{
    return ae_destination;
}

template<class Edge, class Path, class GetEdgesFunction>
Path CPathFinder<Edge, Path, GetEdgesFunction>::find_path(CVertice<Path>* source, CVertice<Path>* destination)
{
    pf_source = source;
    pf_destination = destination;
    pf_queue.push(source);

    while(destination->v_state == BEST || !pf_queue.empty())
    {
        CVertice<Path>* vert = pf_queue.front();
        pf_queue.pop();
        if(vert->v_state == BEST)
            continue;
        vert->v_state = BEST;
        for(const Edge& edge : pf_get_edges(vert))
        {
            CVertice<Path>* tmpVert = edge.source;
            if(tmpVert->v_state == BEST)
                break;

            Path path = edge.destination->v_best_path.add(edge);
            if(vert->v_state == NOT_VISITED || path < edge.source->v_best_path)
            {
                vert->v_state = VISITED;
                vert->v_best_path = path;
                vert->v_sibling = edge.destination;
                pf_queue.push(edge.source);
            }
        }
        if(destination->v_state == NOT_VISITED)
            throw CExNoPath();
    }

    return destination->v_best_path;
}

template<class Edge, class Path, class GetEdgesFunction>
std::vector<CVertice<Path>* > CPathFinder<Edge, Path, GetEdgesFunction>::get_path() const
{
    CVertice<Path>* vert = pf_destination;
    while(vert->v_sibling)
    {
        pf_path.push_back(vert);
        vert = vert->v_sibling;
    }
    pf_path.push_back(pf_source);
    std::reverse(pf_path.begin(), pf_path.end());

    return pf_path;
}
