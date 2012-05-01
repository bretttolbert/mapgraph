#ifndef TRAVELING_SALESMAN_PROBLEM_H
#define TRAVELING_SALESMAN_PROBLEM_H

#include <vector>

#include "Graph.h"
#include "SvgFile.h"
#include "IntegerIdAdjacencyListFile.h"
#include "USCountiesAdjacencyListFile.h"
#include "USCountiesSvgFile.h"

namespace GraphGame
{
    class TravelingSalesmanProblem
    {
    public:
        TravelingSalesmanProblem(IntegerIdAdjacencyListFile* af, 
                                 SvgFile* svg,
                                 const std::vector<int>& targetNodeIds);
        ~TravelingSalesmanProblem();
    private:
        struct TargetNode
        {
            int nodeId;
            std::string nodeString;
            bool visited;
        };
        IntegerIdAdjacencyListFile* af;
        SvgFile* svg;
        std::vector<TargetNode*> targetNodes;
        Graph<int> graph;
        const IntegerIdAdjacencyListFile::NodeIdToNodeStringMap& nodeIdToNodeStringMap;
        void markPath(const std::vector<int>& path);
        TargetNode* getTargetNodeById(int id);
    };
}

#endif
