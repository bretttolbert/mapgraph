#ifndef TRAVELING_SALESMAN_PROBLEM_H
#define TRAVELING_SALESMAN_PROBLEM_H

#include <set>
#include <vector>

#include "USCountiesAdjacencyListFile.h"
#include "USCountiesSvgFile.h"
#include "Graph.h"
#include "IntegerIdAdjacencyListFile.h"

namespace GraphGame
{
    class TravelingSalesmanProblem
    {
    public:
        TravelingSalesmanProblem();
        ~TravelingSalesmanProblem();
    private:
        struct TargetNode
        {
            int nodeId;
            std::string nodeString;
            bool visited;
        };
        std::set<TargetNode*> targetNodes;
        USCountiesSvgFile svg;
        USCountiesAdjacencyListFile adjacencyListFile;
        Graph<int> graph;
        const IntegerIdAdjacencyListFile::NodeIdToNodeStringMap& nodeIdToNodeStringMap;
        void markPath(const std::vector<int>& path);
    };
}

#endif
