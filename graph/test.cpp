#include <functional>
#include "bus.hpp"
#include "corientedgraph.hpp"
#include "gtest/gtest.h"

TEST(CPathFinderTest, bus_problem)
{
    BusProblem::CSchedule* schedule = new BusProblem::CSchedule(4);
    schedule->add(new BusProblem::CEdge(0, 1, 400, 500));
    schedule->add(new BusProblem::CEdge(1, 2, 600, 630));
    schedule->add(new BusProblem::CEdge(0, 3, 800, 1000));
    auto pathFinder = init<BusProblem::CEdge, BusProblem::CPath,
            std::vector<BusProblem::CEdge*> (BusProblem::CSchedule::*)(const int) const>(&BusProblem::CSchedule::get, schedule);
    pathFinder->find_path(new CVertice<BusProblem::CPath>(0), new CVertice<BusProblem::CPath>(2));
}

TEST(CPathFinderTest, oriented_graph)
{
    OrientedGraph::COrientedGraph* graph = new OrientedGraph::COrientedGraph(5);
    graph->add(new OrientedGraph::CEdge(1, 2, 1));
    graph->add(new OrientedGraph::CEdge(2, 3, 1));
    graph->add(new OrientedGraph::CEdge(3, 5, 1));
    graph->add(new OrientedGraph::CEdge(5, 4, 1));
    graph->add(new OrientedGraph::CEdge(1, 4, 1));
    auto pathFinder = init<OrientedGraph::CEdge, OrientedGraph::CPath,
            std::vector<OrientedGraph::CEdge*> (OrientedGraph::COrientedGraph::*)(const int) const>(&OrientedGraph::COrientedGraph::get, graph);
    auto path = pathFinder->find_path(new CVertice<OrientedGraph::CPath>(0), new CVertice<OrientedGraph::CPath>(2));
    EXPECT_EQ(path->get_path_length(), 3);
    EXPECT_THROW(pathFinder->find_path(new CVertice<OrientedGraph::CPath>(5), new CVertice<OrientedGraph::CPath>(1)), CExNoPath);
}
