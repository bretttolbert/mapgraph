#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cassert>

#include "Demos.h"
#include "GraphGame.h"
#include "Game.h"
#include "USCountiesGame.h"
#include "TravelingSalesmanProblem.h"
#include "Graph.h"
#include "USCountiesSvgFile.h"
#include "USStatesSvgFile.h"
#include "USCountiesAdjacencyListFile.h"
#include "CsvAdjacencyListFile.h"
#include "StringUtils.h"
#include "Utils.h"

namespace GraphGame
{
    void breadthFirstSearch_Demo(IntegerIdAdjacencyListFile* af, SvgFile* svg,
                                 int startId, int goalId)
    {
        Graph<int> graph(af->getAdjacencyList());
        std::vector<int> path = graph.breadthFirstSearch(startId,goalId);
        std::cout << "Path:\n";
        std::vector<int>::const_iterator it;
        for (it=path.begin(); it!=path.end(); ++it)
        {
            std::string nodeStr = af->nodeIdToString(*it);
            std::cout << nodeStr << " (" << 
                USCountiesAdjacencyListFile::fipsToString(*it) << ")\n";
            if (svg)
            {
                if (it == path.begin())
                {
                    svg->markNode(*it, "#009900");
                }
                else if (next(it) == path.end())
                {
                    svg->markNode(*it, "#CC0000");
                }
                else
                {
                    svg->markNode(*it, "gray");
                }
            }
        }
        std::cout << "(" << (path.size()-1) << " moves)\n"; 
        if (svg) svg->saveFile("output/output.svg");
    }

    void greedyColoring(IntegerIdAdjacencyListFile* af, SvgFile* svg, 
                        const std::set<std::string>& availableColors)
    {
        std::cout << "Greedy coloring...\n";

        Graph<int> g(af->getAdjacencyList());
        Graph<int>::NodeSet& nodes = g.nodes;
        Graph<int>::NodeSet::iterator it;
        for (it=nodes.begin(); it!=nodes.end(); ++it)
        {
            Graph<int>::Node* currentNode = *it;
            //determine colors of neighbors (can't use these)
            std::set<std::string> neighborColors;
            std::vector<Graph<int>::Node*>& neighbors = currentNode->neighbors;
            std::vector<Graph<int>::Node*>::iterator jt;
            for (jt=neighbors.begin(); jt!=neighbors.end(); ++jt)
            {
                Graph<int>::Node* neighborNode = *jt;
                if (neighborNode->color != "black")
                {
                    neighborColors.insert(neighborNode->color);
                }
            }
            std::set<std::string> candidateColors;
            std::set<std::string>::const_iterator kt;
            for (kt=availableColors.begin(); kt!=availableColors.end(); ++kt)
            {
                if (neighborColors.count(*kt) == 0)
                {
                    candidateColors.insert(*kt);
                }
            }
            if (candidateColors.size() == 0)
            {
                std::cerr << "Coloring not possible with given number of colors (" 
                          << availableColors.size() << "). Greedy coloring failed.\n";
                break;
            }
            else
            {
                currentNode->color = *(candidateColors.begin());
                if (svg)
                {
                    svg->markNode(currentNode->value, currentNode->color);
                }
            }
        }
        if (svg) svg->saveFile("output/output.svg");
        g.resetNodes();
    }

    void randomColoring(IntegerIdAdjacencyListFile* af, SvgFile* svg)
    {
        std::cout << "Generating random coloring...\n";
        //mark each node with a random color
        const IntegerIdAdjacencyListFile::NodeIdToNodeStringMap& nodeMap = 
            af->getNodeIdToNodeStringMap();
        IntegerIdAdjacencyListFile::NodeIdToNodeStringMap::const_iterator it;
        for (it=nodeMap.begin(); it!=nodeMap.end(); ++it)
        {
            svg->markNode(it->first, StringUtils::randomColor());
            svg->saveFile("output/output.svg");
        }
        std::cout << "Done.\n";
    }

    void showNeighbors(IntegerIdAdjacencyListFile* af, SvgFile* svg, int nodeId)
    {
        if (GraphGame::showWarnings)
        {
            //create graph just for validation purposes
            Graph<int> g(af->getAdjacencyList());
        }
        svg->markNode(nodeId, "red");
        std::string nodeStr = af->nodeIdToString(nodeId);
        std::cout << "Neighbors of " << nodeStr << " (" << nodeId << "):\n";
        const std::set<int> neighbors = af->getNeighbors(nodeId);
        const std::set<int>::const_iterator it;
        for (it=neighbors.begin(); it!=neighbors.end(); ++it)
        {
            int neighborId = *it;
            svg->markNode(neighborId, "blue");
            std::cout << af->nodeIdToString(neighborId) << " (" << neighborId << ")\n";
        }
        svg->saveFile("output/output.svg");
    }
}
