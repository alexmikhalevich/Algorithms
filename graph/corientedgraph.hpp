#ifndef CGRAPH_HPP
#define CGRAPH_HPP
#include <vector>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <assert.h>
#include <fstream>
#include <utility>

/*
 *V - the amount of vertices
 *E - the amount of edges
 *
 *Memory for the structure:
 *O(V+E)
 */

template<class V, class E, class PathFunction>
class CGraph
{
public:
    class Vertice;
    class Edge;
    CGraph() : g_rootVertice(NULL) {}
    void init();
    void init_bus_problem(std::string &inputFile);
    int dijkstra(const V& source, const V& destination, PathFunction pathFunc);
    std::vector<V> get_path() const;
    CGraph<V, E, PathFunction>::Edge* get_edge(const V& sourceId, const V& destId) const;
    unsigned int get_edge_weight(const V& sourceId, const V& destId) const;
private:
    CGraph<V, E, PathFunction>::Vertice* g_rootVertice;
    std::vector<CGraph<V, E, PathFunction>::Edge*> g_path;
};

template<class V, class E, class PathFunction>
class CGraph<V, E, PathFunction>::Vertice
{
private:
    Vertice();
    V v_id;
    unsigned int v_shortestPath;
    CGraph<V, E, PathFunction>::Vertice* v_sibling;
    std::vector<CGraph<V, E, PathFunction>::Edge*> v_edges;
    bool v_isVisited;
public:
    Vertice(V id) : v_id(id), v_shortestPath(std::numeric_limits<unsigned int>::max()),
        v_sibling(NULL), v_isVisited(false) {}
    friend class CGraph;
};

template<class V, class E, class PathFunction>
class CGraph<V, E, PathFunction>::Edge
{
private:
    Edge();
    unsigned int e_weight;
    CGraph<V, E, PathFunction>::Vertice* e_source;
    CGraph<V, E, PathFunction>::Vertice* e_destination;
public:
    Edge(const int weight, CGraph<V, E, PathFunction>::Vertice* source, CGraph<V, E, PathFunction>::Vertice* destination) :
        e_weight(weight), e_source(source), e_destination(destination) {}
    friend class CGraph;
};

template<class V, class E, class PathFunction>
void CGraph<V, E, PathFunction>::init() //O(VE)
{
    int numOfVertices = 0;
    std::cout << "Enter the amount of vertices: ";
    std::cin >> numOfVertices;

    V firstId;
    std::cout << "Enter id for the vertice " << 1 << ": ";
    std::cin >> firstId;
    g_rootVertice = new CGraph<V, E, PathFunction>::Vertice(firstId);

    CGraph<V, E, PathFunction>::Vertice* lastVertice = g_rootVertice; //NOTE 1: possible bug here if lastVertice and g_rootVertice point to different objects
    for(int i = 1; i < numOfVertices; ++i)
    {
        V id;
        std::cout << "Enter id for the vertice " << i + 1 << ": ";
        std::cin >> id;
        CGraph<V, E, PathFunction>::Vertice* vertice = new CGraph<V, E, PathFunction>::Vertice(id);
        lastVertice->v_sibling = vertice;                           //see note 1
    }

    int numOfEdges = 0;
    std::cout << "Enter the amount of edges: ";
    std::cin >> numOfEdges;
    for(int i = 0; i < numOfEdges; ++i)
    {
        CGraph<V, E, PathFunction>::Vertice* vert = g_rootVertice;
        CGraph<V, E, PathFunction>::Vertice* source = NULL;
        CGraph<V, E, PathFunction>::Vertice* destination = NULL;
        V id_from, id_dest;
        E weight;
        std::cout << "Enter id 's of the source/destination vertices and the edge weight: ";
        std::cin >> id_from >> id_dest >> weight;

        while(vert)
        {
            if(vert->v_id == id_from)
            {
                source = vert;
                if(destination)
                    break;
            }
            if(vert->v_id == id_dest)
            {
                destination = vert;
                if(source)
                    break;
            }
            vert = vert->v_sibling;
        }
        assert(source != NULL);
        assert(destination != NULL);

        typename CGraph<V, E, PathFunction>::Edge* edge = new CGraph<V, E, PathFunction>::Edge(weight, source, destination);
        source->v_edges.push_back(edge);
        source = NULL;
        destination = NULL;
    }
}

template<class V, class E, class PathFunction>
void CGraph<V, E, PathFunction>::init_bus_problem(std::string& inputFile) //O(VE)
{
    std::ifstream in(inputFile);

    int numOfVertices = 0;
    in >> numOfVertices;

    V firstId;
    in >> firstId;
    g_rootVertice = new CGraph<V, E, PathFunction>::Vertice(firstId);

    CGraph<V, E, PathFunction>::Vertice* lastVertice = g_rootVertice; //see note 1
    for(int i = 1; i < numOfVertices; ++i)
    {
        V id;
        in >> id;
        CGraph<V, E, PathFunction>::Vertice* vertice = new CGraph<V, E, PathFunction>::Vertice(id);
        lastVertice->v_sibling = vertice;                           //see note 1
    }

    int numOfEdges = 0;
    in >> numOfEdges;
    for(int i = 0; i < numOfEdges; ++i)
    {
        CGraph<V, E, PathFunction>::Vertice* vert = g_rootVertice;
        CGraph<V, E, PathFunction>::Vertice* source = NULL;
        CGraph<V, E, PathFunction>::Vertice* destination = NULL;
        V id_from, id_dest;
        std::pair<int, int> weight;
        int weight_1, weight_2;
        in >> id_from >> id_dest >> weight_1 >> weight_2;
        weight = std::make_pair(weight_1, weight_2);

        while(vert)
        {
            if(vert->v_id == id_from)
            {
                source = vert;
                if(destination)
                    break;
            }
            if(vert->v_id == id_dest)
            {
                destination = vert;
                if(source)
                    break;
            }
            vert = vert->v_sibling;
        }
        assert(source != NULL);
        assert(destination != NULL);

        typename CGraph<V, E, PathFunction>::Edge* edge = new CGraph<V, E, PathFunction>::Edge(weight, source, destination);
        source->v_edges.push_back(edge);
        source = NULL;
        destination = NULL;
    }
}

template<class V, class E, class PathFunction> //TODO: add path function supporting
int CGraph<V, E, PathFunction>::dijkstra(const V& source, const V& destination, PathFunction pathFunc) //TODO: check complexity
{
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
//            vertice->v_edges.at(i)->e_destination->v_shortestPath = pathFunc();
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

    return -1;
}

template<class V, class E, class PathFunction>
typename CGraph<V, E, PathFunction>::Edge* CGraph<V, E, PathFunction>::get_edge(const V& sourceId, const V& destId) const //O(V+E)
{
    CGraph<V, E, PathFunction>::Vertice* vert = g_rootVertice;
    while(vert)
    {
        if(vert->v_id == sourceId)
            for(std::size_t i = 0; i < vert->v_edges.size(); ++i)
                if(vert->v_edges.at(i)->e_destination->v_id == destId)
                    return vert->v_edges.at(i);
        vert = vert->sibling;
    }
    if(!vert)
        return NULL;
}

template<class V, class E, class PathFunction>
unsigned int CGraph<V, E, PathFunction>::get_edge_weight(const V& sourceId, const V& destId) const //O(V+E)
{
    CGraph<V, E, PathFunction>::Edge* edge = get_edge(sourceId, destId);
    return edge->e_weight();
}

template<class V, class E, class PathFunction>
std::vector<V> CGraph<V, E, PathFunction>::get_path() const
{
    return g_path;
}


#endif // CGRAPH_HPP
