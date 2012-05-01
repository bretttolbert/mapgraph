#include <fstream>
#include "TravelingSalesmanProblem.h"
#include "GraphGame.h"

namespace GraphGame
{
    TravelingSalesmanProblem::TravelingSalesmanProblem(
                                 IntegerIdAdjacencyListFile* af_, 
                                 SvgFile* svg_,
                                 const std::vector<int>& targetNodeIds)
        : af(af_),
          svg(svg_),
          graph(af->getAdjacencyList()),
          nodeIdToNodeStringMap(af->getNodeIdToNodeStringMap())
    {
        std::vector<int>::const_iterator idIter;
        for (idIter=targetNodeIds.begin(); idIter!=targetNodeIds.end(); ++idIter)
        {
                TargetNode* targetNode = new TargetNode();
                targetNode->nodeId = *idIter;
                targetNode->nodeString = af->nodeIdToString(*idIter);
                targetNode->visited = false;
                targetNodes.push_back(targetNode);
        }

        std::vector<int> fullPath;        
        std::vector<TargetNode*>::const_iterator jt = targetNodes.begin();
        TargetNode* startNode = *jt;
        TargetNode* currentNode = startNode;
        while (1)
        {
            std::vector<int> path;
            TargetNode* nearestTargetNode = NULL;
            std::vector<int> shortestPath;
            std::cout << currentNode->nodeString << "\n";
            currentNode->visited = true;
            bool foundUnvisitedNode = false;
            for (jt=targetNodes.begin(); jt!=targetNodes.end(); ++jt)
            {
                //find nearest unvisited node
                TargetNode* targetNode = *jt;
                if (!targetNode->visited)
                {
                    foundUnvisitedNode = true;
                    std::vector<int> path = graph.breadthFirstSearch(currentNode->nodeId, targetNode->nodeId);
                    if (nearestTargetNode == NULL || path.size() < shortestPath.size())
                    {
                        nearestTargetNode = targetNode;
                        shortestPath = path;
                    }
                }
            }
            if (!foundUnvisitedNode)
            {
                //no more nodes to visit, return to start node
                std::vector<int> path = graph.breadthFirstSearch(currentNode->nodeId, startNode->nodeId);
                markPath(path);
                svg->markNode(startNode->nodeId, "purple");
                std::cout << startNode->nodeString << "\n";
                break;
            }
            else
            {
                currentNode = nearestTargetNode;
                markPath(shortestPath);
            }
        }
        svg->saveFile("output/output.svg");
    }

    TravelingSalesmanProblem::~TravelingSalesmanProblem()
    {
        std::vector<TargetNode*>::iterator it;
        for (it=targetNodes.begin(); it!=targetNodes.end(); ++it)
        {
            TargetNode* targetNode = *it;
            delete targetNode;
        }
    }

    void TravelingSalesmanProblem::markPath(const std::vector<int>& path)
    {
        std::vector<int>::const_iterator it;
        for (it=path.begin(); it!=path.end(); ++it)
        {
            if (getTargetNodeById(*it) != NULL)
            {
                svg->markNode(*it, "red");
            }
            else
            {
                svg->markNode(*it, "gray");
            }
        }
    }

    TravelingSalesmanProblem::TargetNode* TravelingSalesmanProblem::getTargetNodeById(int id)
    {
        TargetNode* result = NULL;
        std::vector<TargetNode*>::iterator it;
        for (it=targetNodes.begin(); it!=targetNodes.end(); ++it)
        {
            TargetNode* node = *it;
            if (node->nodeId == id)
            {
                result = node;
                break;
            }
        }
        return result;
    }
}

