#include <functional>
#include <vector>
#include <queue>

enum EVerticeState
{
    VISITED,
    NOT_VISITED
};

template<class Path>
class CVertice
{
private:
    Path* v_best_path;
    EVerticeState v_state;
public:
    CVertice();
};

template<class Path>
class CAbstractEdge
{
private:
    CVertice<Path>* ae_source;
    CVertice<Path>* ae_destination;
public:
    CAbstractEdge(CVertice* source, CVertice* destination) : ae_source(source), ae_destination(destination) {}
};

template<class Edge, class Path, class GetEdgesFunction>
class CPathFinder
{
private:
    GetEdgesFunction pf_get_edges;
    std::queue<CVertice<Path> > pf_queue;
    std::vector<CVertice<Path> > get_path() const;
public:
    Path find_path(CVertice<Path>* source, CVertice<Path>* destination);

};

template<class Path>
CVertice::CVertice()
{
    v_state = NOT_VISITED;
}

template<class Edge, class Path, class GetEdgesFunction>
Path CPathFinder::find_path(CVertice<Path>* source, CVertice<Path>* destination)
{
    while(!pf_queue.empty())
    {

    }



    g_path.clear();

    CGraph<V, E, PathFunction>::Vertice* vertice = g_rootVertice;
    while(vertice)
    {
        if(vertice->v_id == source)
            break;
        vertice = vertice->v_sibling;
    }
    assert(vertice != NULL);

    vertice->v_shortestPath = 0;
    while(vertice)
    {
        vertice->isVisited = true;
        g_path.push_back(vertice->v_id);

        if(vertice->v_id == destination)
            return vertice->v_shortestPath;

        for(auto i : vertice->v_edges)
        {
            std::min(vertice->v_edges.at(i)->e_destination->v_shortestPath,
                     vertice->v_shortestPath + vertice->v_edges.at(i)->e_weight);
        }

        CGraph<V, E, PathFunction>::Vertice* tempVert = g_rootVertice;
        unsigned int minPath = std::numeric_limits<unsigned int>::max();
        while(tempVert)
        {
            if(tempVert->v_shortestPath < minPath && !tempVert->v_isVisited)
            {
                vertice = tempVert;
                minPath = tempVert->v_shortestPath;
            }
            tempVert = tempVert->v_sibling;
        }
        vertice = tempVert;
    }
}
