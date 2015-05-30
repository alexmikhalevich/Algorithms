#ifndef CPATHFINDER_HPP
#define CPATHFINDER_HPP

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
    Path v_best_path;
    EVerticeState v_state;
    int v_sibling_id;
public:
    CVertice(const int id) : v_id(id), v_state(NOT_VISITED), v_sibling_id(0) {}
    int get_id() const;
    EVerticeState get_state() const;
    int get_sibling_id() const;
    Path get_best_path() const;
    void set_state(EVerticeState state);
    void set_best_path(Path path);
    void set_sibling(int sibling_id);
};

template<class Path>
class CAbstractEdge
{
private:
    CAbstractEdge();
    int ae_source_id;
    int ae_destination_id;
public:
    CAbstractEdge(const int source_id, const int destination_id) :
        ae_source_id(source_id), ae_destination_id(destination_id) {}
    int get_source_id() const;
    int get_destination_id() const;
};

template<class Edge, class Path, class GetEdgesFunction, class GraphClass>
class CPathFinder
{
private:
    GetEdgesFunction pf_get_edges;
    int pf_destination_id;
    int pf_source_id;
    GraphClass* pf_object;
    std::queue<int> pf_queue;
    std::vector<int> pf_path;
    std::vector<CVertice<Path>* > pf_vertices;
public:
    CPathFinder(GetEdgesFunction get_edges, GraphClass* object) : pf_get_edges(get_edges), pf_destination_id(0), pf_source_id(0), pf_object(object) {}
    Path find_path(const int source_id, const int destination_id);
    std::vector<CVertice<Path>* > get_path();
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
int CVertice<Path>::get_sibling_id() const
{
    return v_sibling_id;
}

template<class Path>
Path CVertice<Path>::get_best_path() const
{
    return v_best_path;
}

template<class Path>
void CVertice<Path>::set_state(EVerticeState state)
{
    v_state = state;
}

template<class Path>
void CVertice<Path>::set_best_path(Path path)
{
    v_best_path = path;
}

template<class Path>
void CVertice<Path>::set_sibling(int sibling_id)
{
    v_sibling_id = sibling_id;
}

template<class Path>
int CAbstractEdge<Path>::get_source_id() const
{
    return ae_source_id;
}

template<class Path>
int CAbstractEdge<Path>::get_destination_id() const
{
    return ae_destination_id;
}

template<class Edge, class Path, class GetEdgesFunction, class GraphClass>
Path CPathFinder<Edge, Path, GetEdgesFunction, GraphClass>::find_path(const int source_id, const int destination_id)
{
    pf_vertices.clear();
    pf_queue.push(source_id);
    pf_source_id = source_id;
    pf_destination_id = destination_id;

    if(std::max(source_id, destination_id) >= pf_vertices.size())
        pf_vertices.resize(std::max(source_id, destination_id) + 1);

    pf_vertices[destination_id] = new CVertice<Path>(destination_id);
    while(pf_vertices.at(destination_id)->get_state() != BEST && !pf_queue.empty())
    {
        int vert_id = pf_queue.front();
        pf_queue.pop();
        if(vert_id >= pf_vertices.size())
            pf_vertices.resize(vert_id + 1);
        if(pf_vertices[vert_id] == NULL)
            pf_vertices[vert_id] = new CVertice<Path>(vert_id);
        if(pf_vertices.at(vert_id)->get_state() == BEST)
            continue;
        pf_vertices[vert_id]->set_state(BEST);

        for(Edge* edge : (pf_object->*pf_get_edges)(vert_id))
        {
            if(edge->get_destination_id() >= pf_vertices.size())
                pf_vertices.resize(edge->get_destination_id() + 1);
            if(pf_vertices[edge->get_destination_id()] == NULL)
                pf_vertices[edge->get_destination_id()] = new CVertice<Path>(edge->get_destination_id());
            CVertice<Path>* tmpVert = pf_vertices[edge->get_destination_id()];
            if(tmpVert->get_state() == BEST)
                continue;

            Path path = pf_vertices[edge->get_source_id()]->get_best_path().add(edge);
            if(tmpVert->get_state() == NOT_VISITED || path < pf_vertices[edge->get_destination_id()]->get_best_path())
            {
                tmpVert->set_state(VISITED);
                tmpVert->set_best_path(path);
                tmpVert->set_sibling(edge->get_source_id());
                pf_queue.push(edge->get_destination_id());
            }
            tmpVert = NULL;
        }
    }

    if(pf_vertices[destination_id]->get_state() == NOT_VISITED)
        throw CExNoPath();

    return pf_vertices[destination_id]->get_best_path();
}

template<class Edge, class Path, class GetEdgesFunction, class GraphClass>
std::vector<CVertice<Path>* > CPathFinder<Edge, Path, GetEdgesFunction, GraphClass>::get_path()
{
    int vert_id = pf_destination_id;

    while(pf_vertices[vert_id]->get_sibling())
    {
        pf_path.push_back(vert_id);
        vert_id = pf_vertices[vert_id]->get_sibling()->get_id();
    }
    pf_path.push_back(pf_source_id);
    std::reverse(pf_path.begin(), pf_path.end());

    return pf_path;
}

#endif //CPATHFINDER_HPP
