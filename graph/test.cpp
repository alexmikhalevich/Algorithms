#include <functional>
#include "bus.hpp"
//#include "corientedgraph.hpp"
#include "gtest/gtest.h"

TEST(CPathFinderTest, BusProblem)
{
    CSchedule* schedule = new CSchedule(4);
    schedule->add(new CEdge(0, 1, 400, 500));
    schedule->add(new CEdge(1, 2, 600, 630));
    schedule->add(new CEdge(0, 3, 800, 1000));
    auto pathFinder = init<CEdge, CPath, std::vector<CEdge*> (CSchedule::*)(const int) const>(&CSchedule::get, schedule);
    pathFinder->find_path(new CVertice<CPath>(0), new CVertice<CPath>(2));
}

//TEST(CPathFinderTest, OrientedGraph)
//{
//    COrientedGraph* graph = new COrientedGraph();
//    graph->add(new COrientedEdge(1, 2, 1));
//    graph->add(new COrientedEdge(2, 3, 1));
//    graph->add(new COrientedEdge(3, 5, 1));
//    graph->add(new COrientedEdge(5, 4, 1));
//    graph->add(new COrientedEdge(1, 4, 1));
//    //TODO: CPathFinder
//}
