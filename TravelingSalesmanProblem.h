#ifndef TRAVELING_SALESMAN_PROBLEM_H
#define TRAVELING_SALESMAN_PROBLEM_H

#include "USCountiesAdjacencyListFile.h"
#include "USCountiesSvgFile.h"
#include "Graph.h"

namespace GraphGame
{
    class TravelingSalesmanProblem
    {
    public:
        TravelingSalesmanProblem();
        void performBfsAndUpdateSvg(USCountiesAdjacencyListFile& adjacencyListFile,
                                    USCountiesSvgFile& svg,
                                    Graph<int>& graph,
                                    int startFips, 
                                    int goalFips);
    private:
    };
}

#endif