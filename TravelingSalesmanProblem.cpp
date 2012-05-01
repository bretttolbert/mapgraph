#include <fstream>
#include "TravelingSalesmanProblem.h"
#include "GraphGame.h"

namespace GraphGame
{
    TravelingSalesmanProblem::TravelingSalesmanProblem()
        : graph(adjacencyListFile.getAdjacencyList()),
          nodeIdToNodeStringMap(adjacencyListFile.getNodeIdToNodeStringMap())
    {
        IntegerIdAdjacencyListFile::NodeIdToNodeStringMap::const_iterator it;
        for (it=nodeIdToNodeStringMap.begin(); it!=nodeIdToNodeStringMap.end(); ++it)
        {
            if (it->second.find("Washington") != std::string::npos)
            {
                TargetNode* targetNode = new TargetNode();
                targetNode->nodeId = it->first;
                targetNode->nodeString = it->second;
                targetNode->visited = false;
                targetNodes.insert(targetNode);
            }
        }
        std::cout << "Found " << targetNodes.size() << " Washingtons\n";
        std::vector<int> fullPath;        
        std::set<TargetNode*>::const_iterator jt = targetNodes.begin();
        TargetNode* startNode = *jt;
        TargetNode* currentNode = startNode;
        while (1)
        {
            std::vector<int> path;
            TargetNode* nearestTargetNode = NULL;
            std::vector<int> shortestPath;
            std::cout << "marking " << currentNode->nodeString << " as visisted\n";
            currentNode->visited = true;
            bool foundUnvisitedNode = false;
            for (jt=targetNodes.begin(); jt!=targetNodes.end(); ++jt)
            {
                //find nearest unvisited node
                TargetNode* targetNode = *jt;
                if (!targetNode->visited)
                {
                    foundUnvisitedNode = true;
                    std::cout << "found unvisited node " << targetNode->nodeString << std::endl;
                    std::vector<int> path = graph.breadthFirstSearch(currentNode->nodeId, targetNode->nodeId);
                    if (nearestTargetNode == NULL || path.size() < shortestPath.size())
                    {
                        nearestTargetNode = targetNode;
                        shortestPath = path;
                        std::cout << "new shortest path is to " << nearestTargetNode->nodeString << std::endl;
                    }
                }
            }
            if (!foundUnvisitedNode)
            {
                //no more nodes to visit, return to start node
                std::vector<int> path = graph.breadthFirstSearch(currentNode->nodeId, startNode->nodeId);
                markPath(path);
                svg.markNode(startNode->nodeId, "purple");
                break;
            }
            else
            {
                currentNode = nearestTargetNode;
                markPath(shortestPath);
            }
        }
        svg.saveFile("output/output.svg");
    }

    TravelingSalesmanProblem::~TravelingSalesmanProblem()
    {
        std::set<TargetNode*>::iterator it;
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
            std::string nodeString = adjacencyListFile.nodeIdToString(*it);
            if (it != path.begin())
            {
                if (nodeString.find("Washington") != std::string::npos)
                {
                    svg.markNode(*it, "red");
                }
                else
                {
                    svg.markNode(*it, "gray");
                }
            }
        }
    }
}

