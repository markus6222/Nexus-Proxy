// Checks for the grid A* used by /pathfinder. Build and run:
//   cl /EHsc /std:c++17 test_pathfinder.cpp && test_pathfinder.exe
// The out-of-range cases are the ones that crashed the proxy on shift-punch.
#include <cassert>
#include <cstdio>
#include "PathFinder_2.h"

int main() {
    // open 10x10 grid, 4-way movement
    {
        PathFinderxd pf(10, 10);
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });
        auto path = pf.aStar(0, 0, 3, 0);
        assert(path.size() == 4);                       // (0,0)->(3,0)
        assert(path.front() == std::make_pair(0, 0));
        assert(path.back() == std::make_pair(3, 0));
    }

    // a wall the search has to go around
    {
        PathFinderxd pf(10, 10);
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });
        for (int y = 0; y < 9; y++) pf.setBlocked(5, y);  // wall with a gap at y=9
        auto path = pf.aStar(0, 0, 9, 0);
        assert(!path.empty());
        for (auto& p : path) assert(!(p.first == 5 && p.second < 9));
    }

    // fully walled off -> no path, and no hang
    {
        PathFinderxd pf(10, 10);
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });
        for (int y = 0; y < 10; y++) pf.setBlocked(5, y);
        assert(pf.aStar(0, 0, 9, 0).empty());
    }

    // out-of-range start/finish must return empty rather than index the grid
    {
        PathFinderxd pf(10, 10);
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });
        assert(pf.aStar(-1, 0, 5, 5).empty());
        assert(pf.aStar(0, -1, 5, 5).empty());
        assert(pf.aStar(0, 0, 10, 5).empty());
        assert(pf.aStar(0, 0, 5, 10).empty());
        assert(pf.aStar(99999, 99999, 5, 5).empty());
        assert(pf.aStar(0, 0, 99999, 5).empty());
    }

    // degenerate grid (world not loaded yet) must not crash
    {
        PathFinderxd pf(0, 0);
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });
        assert(pf.aStar(0, 0, 0, 0).empty());
    }

    // neighbours never set -> no movement model, return empty instead of looping
    {
        PathFinderxd pf(10, 10);
        assert(pf.aStar(0, 0, 5, 5).empty());
    }

    // blocked destination is unreachable
    {
        PathFinderxd pf(10, 10);
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });
        pf.setBlocked(5, 5);
        assert(pf.aStar(0, 0, 5, 5).empty());
    }

    // setBlocked outside the grid is ignored, not a write out of bounds
    {
        PathFinderxd pf(10, 10);
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });
        pf.setBlocked(-5, -5);
        pf.setBlocked(1000, 1000);
        assert(!pf.aStar(0, 0, 9, 9).empty());
    }

    printf("pathfinder: all checks passed\n");
    return 0;
}
