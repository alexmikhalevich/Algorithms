#include <functional>
#include <vector>
#include <queue>
#include <algorithm>

//TODO: destructors

enum EVerticeState
{
    NOT_VISITED,
    VISITED,
    BEST
};

class CExNoPath
{
};

template<class Path>
class CVertice
{
private:
    int v_id;
    Path* v_best_path;
    EVerticeState v_state;
    CVertice<Path>* v_sibling;
public:
    CVertice(const int id) : v_id(id), v_best_path(NULL), v_state(NOT_VISITED), v_sibling(NULL) {}
    int get_id() const;
    EVerticeState get_state() const;
    CVertice<Path>* get_sibling() const;
    Path* get_best_path() const;
    void set_state(EVerticeState state);
    void set_best_path(Path* path);
    void set_sibling(CVertice<Path>* sibling);
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

template<class Edge, class Path, class GetEdgesFunction, class GraphClass>
class CPathFinder
{
private:
    CPathFinder();
    GetEdgesFunction pf_get_edges;
    CVertice<Path>* pf_destination;
    CVertice<Path>* pf_source;
    GraphClass* pf_object;
    std::queue<CVertice<Path>* > pf_queue;
    std::vector<CVertice<Path>* > pf_path;
    std::vector<CVertice<Path>* > get_path() const;
public:
    CPathFinder(GetEdgesFunction get_edges, GraphClass* object) : pf_get_edges(get_edges), pf_destination(NULL), pf_source(NULL), pf_object(object) {}
    Path* find_path(CVertice<Path>* source, CVertice<Path>* destination);
};

template<class Edge, class Path, class GetEdgesFunction, class GraphClass>
CPathFinder<Edge, Path, GetEdgesFunction, GraphClass>* init(GetEdgesFunction func, GraphClass* object)
{
    CPathFinder<Edge, Path, GetEdgesFunction, GraphClass>* pathFinder = new CPathFinder<Edge, Path, GetEdgesFunction, GraphClass>(func, object);
    return pathFinder;
}

template<class Path>
int CVertice<Path>::get_id() const
{
    return v_id;
}

template<class Path>
EVerticeState CVertice<Path>::get_state() const
{
    return v_state;
}

template<class Path>
CVertice<Path>* CVertice<Path>::get_sibling() const
{
    return v_sibling;
}

template<class Path>
Path* CVertice<Path>::get_best_path() const
{
    return v_best_path;
}

template<class Path>
void CVertice<Path>::set_state(EVerticeState state)
{
    v_state = state;
}

template<class Path>
void CVertice<Path>::set_best_path(Path* path)
{
    v_best_path = path;
}

template<class Path>
void CVertice<Path>::set_sibling(CVertice<Path>* sibling)
{
    v_sibling = sibling;
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

template<class Edge, class Path, class GetEdgesFunction, class GraphClass>
Path* CPathFinder<Edge, Path, GetEdgesFunction, GraphClass>::find_path(CVertice<Path>* source, CVertice<Path>* destination)
{
    pf_source = source;
    pf_destination = destination;
    pf_queue.push(source);

    while(destination->get_state() != BEST || !pf_queue.empty())
    {
        CVertice<Path>* vert = pf_queue.front();
        pf_queue.pop();
        if(vert->get_state() == BEST)
            continue;
        vert->set_state(BEST);
        for(Edge* edge : (pf_object->*pf_get_edges)(vert->get_id()))
        {
            CVertice<Path>* tmpVert = edge->get_source();
            if(tmpVert->get_state() == BEST)
                break;

            Path path = edge->get_destination()->get_best_path()->add(edge);
            if(vert->get_state() == NOT_VISITED || path < *(edge->get_source()->get_best_path()))
            {
                vert->set_state(VISITED);
                vert->set_best_path(&path);
                vert->set_sibling(edge->get_destination());
                pf_queue.push(edge->get_source());
            }
        }
        if(destination->get_state() == NOT_VISITED)
            throw CExNoPath();
    }

    return destination->get_best_path();
}

template<class Edge, class Path, class GetEdgesFunction, class GraphClass>
std::vector<CVertice<Path>* > CPathFinder<Edge, Path, GetEdgesFunction, GraphClass>::get_path() const
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
