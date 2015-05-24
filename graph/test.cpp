#include <functional>
#include "bus.hpp"
#include "corientedgraph.hpp"
#include "gtest/gtest.h"

TEST(CPathFinderTest, BusProblem)
{
    CSchedule* schedule = new CSchedule();
    schedule->add(new CEdge(0, 1, 400, 500));
    schedule->add(new CEdge(1, 2, 600, 630));
    schedule->add(new CEdge(0, 4, 800, 1000));
    //TODO: CPathFinder
//    CPathFinder<CEdge, CPath, std::function<std::vector<CEdge*>()> >* =
//            new CPathFinder<CEdge, CPath, std::function<std::vector<CEdge*>()> >(std::bind(&CSchedule::get,
//                                                                                           schedule, std::placeholders::_1));
}

TEST(CPathFinderTest, OrientedGraph)
{
    COrientedGraph* graph = new COrientedGraph();
    graph->add(new COrientedEdge(1, 2, 1));
    graph->add(new COrientedEdge(2, 3, 1));
    graph->add(new COrientedEdge(3, 5, 1));
    graph->add(new COrientedEdge(5, 4, 1));
    graph->add(new COrientedEdge(1, 4, 1));
    //TODO: CPathFinder
}
