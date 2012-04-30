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
#include "Game.h"
#include "USCountiesGame.h"
#include "TravelingSalesmanProblem.h"
#include "Graph.h"
#include "USCountiesSvgFile.h"
#include "USStatesSvgFile.h"
#include "USCountiesAdjacencyListFile.h"
#include "CsvAdjacencyListFile.h"
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
            std::cout << nodeStr << " (" << *it << "), ";
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
        std::cout << "\n(" << (path.size()-1) << " moves)\n"; 
        if (svg) svg->saveFile("output/output.svg");
    }

    void isBipartite_Demo(IntegerIdAdjacencyListFile* af, SvgFile* svg)
    {
        Graph<int> g(af->getAdjacencyList());
        bool isBipartite = true; 
        Graph<int>::Node* startNode = g.getRandomNode();
        std::queue<Graph<int>::Node*> q;
        startNode->color = Graph<int>::Node::COLOR_RED;
        if (svg) svg->markNode(startNode->value, "red");
        q.push(startNode);
        int step = 0;
        while (q.size() > 0)
        {
            Graph<int>::Node* currentNode = q.front();
            q.pop();
            std::vector<Graph<int>::Node*>::iterator it;
            for (it=currentNode->neighbors.begin(); it!=currentNode->neighbors.end(); ++it)
            {
                Graph<int>::Node* adjacentNode = *it;
                if (adjacentNode == NULL)
                {
                    std::cout << "Error: adjacentNode is NULL\n";
                    std::cout << "currentNode = " << currentNode->value << std::endl;
                    exit(1);
                }
                if (adjacentNode->color == Graph<int>::Node::COLOR_BLACK)
                {
                    //mark it as opposite color
                    if (currentNode->color == Graph<int>::Node::COLOR_RED)
                    {
                        adjacentNode->color = Graph<int>::Node::COLOR_BLUE;
                        if (svg) svg->markNode(adjacentNode->value, "blue");
                    }
                    else
                    {
                        adjacentNode->color = Graph<int>::Node::COLOR_RED;
                        if (svg) svg->markNode(adjacentNode->value, "red");
                    }
                    //enqueue neighbor node
                    q.push(adjacentNode);
                    //save svg
                    if (svg)
                    {
                        std::ostringstream oss;
                        oss << "output/bipartite-demo/bipartite_" << ++step << ".svg";
                        svg->saveFile(oss.str().c_str());
                    }
                }
                else
                {
                    //adjacent node has already been marked
                    //verify that it's a different color than current node.
                    //if it isn't, then the graph is not bipartite
                    if (adjacentNode->color == currentNode->color)
                    {
                        isBipartite = false;
                        break;
                    }
                }
            }
        }
        if (isBipartite)
        {
            std::cout << "Graph is bipartite.\n";
        }
        else
        {
            std::cout << "Graph is not bipartite.\n";
        }
        if (svg) svg->saveFile("output/output.svg");
        g.resetNodes();
    }

    void markEachNodeWithRandomColor_Demo(IntegerIdAdjacencyListFile* af, SvgFile* svg)
    {
        //mark each node with a random color
        const IntegerIdAdjacencyListFile::NodeIdToNodeStringMap& nodeMap = 
            af->getNodeIdToNodeStringMap();
        IntegerIdAdjacencyListFile::NodeIdToNodeStringMap::const_iterator it;
        for (it=nodeMap.begin(); it!=nodeMap.end(); ++it)
        {
            svg->markNode(it->first, randomColor());
            svg->saveFile("output/output.svg");
        }
    }

    std::string randomColor()
    {
        std::ostringstream oss;
        oss << '#' << std::hex << std::setfill('0')
            << std::setw(2) << (rand() % 256)
            << std::setw(2) << (rand() % 256)
            << std::setw(2) << (rand() % 256);
        return oss.str();
    }
}
