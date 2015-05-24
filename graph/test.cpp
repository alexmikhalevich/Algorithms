#include <functional>
#include "bus.hpp"
#include "corientedgraph.hpp"
#include "gtest/gtest.h"

TEST(CPathFinderTest, bus_problem)
{
    BusProblem::CSchedule* schedule = new BusProblem::CSchedule(4);
    schedule->add(new BusProblem::CEdge(0, 1, 400, 100, 200));
    schedule->add(new BusProblem::CEdge(1, 2, 600, 30, 200));
    schedule->add(new BusProblem::CEdge(0, 3, 800, 200, 400));
    auto pathFinder = init<BusProblem::CEdge, BusProblem::CPath,
            std::vector<BusProblem::CEdge*> (BusProblem::CSchedule::*)(const int) const>(&BusProblem::CSchedule::get, schedule);
    EXPECT_NO_THROW(pathFinder->find_path(0, 2));
}

TEST(CPathFinderTest, oriented_graph)
{
    OrientedGraph::COrientedGraph* graph = new OrientedGraph::COrientedGraph(5);
    graph->add(new OrientedGraph::CEdge(0, 1, 1));
    graph->add(new OrientedGraph::CEdge(1, 2, 1));
    graph->add(new OrientedGraph::CEdge(2, 4, 1));
    graph->add(new OrientedGraph::CEdge(4, 3, 1));
    graph->add(new OrientedGraph::CEdge(0, 3, 1));
    auto pathFinder = init<OrientedGraph::CEdge, OrientedGraph::CPath,
            std::vector<OrientedGraph::CEdge*> (OrientedGraph::COrientedGraph::*)(const int) const>(&OrientedGraph::COrientedGraph::get, graph);
    auto path = pathFinder->find_path(0, 4);
    EXPECT_EQ(path.get_path_length(), 3);
    EXPECT_THROW(pathFinder->find_path(4, 0), CExNoPath);
}
