#include <functional>
#include "bus.hpp"
#include "gtest/gtest.h"

TEST(CPathFinderTest, BusProblem)
{
    CSchedule* schedule = new CSchedule();
    schedule->add(new CEdge(0, 1, 400, 500));
    schedule->add(new CEdge(1, 2, 600, 630));
    schedule->add(new CEdge(0, 4, 800, 1000));
//    CPathFinder<CEdge, CPath, std::function<std::vector<CEdge*>()> >* =
//            new CPathFinder<CEdge, CPath, std::function<std::vector<CEdge*>()> >(std::bind(&CSchedule::get,
//                                                                                           schedule, std::placeholders::_1));
}
