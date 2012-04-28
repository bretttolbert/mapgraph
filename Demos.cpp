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

namespace GraphGame
{
    void breadthFirstSearch_Demo(const std::string& start, const std::string& goal)
    {
        std::cout << "Starting breadth first search mode...\n";
        USCountiesAdjacencyListFile adjacencyListFile;
        USCountiesSvgFile svg;
        Graph<int> graph(adjacencyListFile.getAdjacencyList());
        int startFips, goalFips;
        if (start.empty())
        {
            startFips = adjacencyListFile.getRandomNodeId();
            start = adjacencyListFile.nodeIdToString(startFips);
        }
        else
        {
            startFips = adjacencyListFile.stringToNodeId(start);
        }
        if (goal.empty())
        {
            goalFips = adjacencyListFile.getRandomNodeId();
            goal = adjacencyListFile.nodeIdToString(goalFips);
        }
        else
        {
            goalFips = adjacencyListFile.stringToNodeId(goal);
        }
        std::cout << "Performing BFS from " << start << " to " << goal << "...\n";
        std::cout << "Start: " << start << " (" << USCountiesAdjacencyListFile::fipsToString(startFips) << ")\n";
        std::cout << "Goal: " << goal << " (" << USCountiesAdjacencyListFile::fipsToString(goalFips) << ")\n";
        std::vector<int> path = graph.breadthFirstSearch(startFips,goalFips);
        std::cout << "Optimal Path:\n";
        std::vector<int>::const_iterator it;
        for (it=path.begin(); it!=path.end(); ++it)
        {
            std::string countyName = adjacencyListFile.nodeIdToString(*it);
            std::cout << countyName << " (" << *it << "), ";
            if (it == path.begin())
            {
                svg.markCountyByFips(*it, "red");
            }
            else
            {
                svg.markCountyByFips(*it, "gray");
            }
        }
        std::cout << "\n(" << path.size() << " moves)\n"; 
        svg.saveFile("output/bfs.svg");
    }

    void isBipartite_Demo()
    {
        CsvAdjacencyListFile adjacencyFile("48US.txt");
        USStatesSvgFile svg;
        Graph<int> g(adjacencyFile.getAdjacencyList());
        bool isBipartite = true; 
        Graph<int>::Node* startNode = g.getRandomNode();
        std::queue<Graph<int>::Node*> q;
        startNode->color = Graph<int>::Node::COLOR_RED;
        svg.markStateByAbbreviation(adjacencyFile.nodeIdToString(startNode->value), "red");
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
                        svg.markStateByAbbreviation(adjacencyFile.nodeIdToString(adjacentNode->value), "blue");
                    }
                    else
                    {
                        adjacentNode->color = Graph<int>::Node::COLOR_RED;
                        svg.markStateByAbbreviation(adjacencyFile.nodeIdToString(adjacentNode->value), "red");
                    }
                    //enqueue neighbor node
                    q.push(adjacentNode);
                    //save svg
                    std::ostringstream oss;
                    oss << "output/bipartite_" << ++step << ".svg";
                    svg.saveFile(oss.str().c_str());
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
        //reset node color
        Graph<int>::NodeSet::const_iterator it;
        for (it=g.nodes.begin(); it!=g.nodes.end(); ++it)
        {
            Graph<int>::Node* node = *it;
            node->color = Graph<int>::Node::COLOR_BLACK;
        }
        if (isBipartite)
        {
            std::cout << "Graph is bipartite.\n";
        }
        else
        {
            std::cout << "Graph is not bipartite.\n";
        }
        svg.saveFile("output/bipartite_final.svg");
    }

    void markEachStateWithRandomColor_Demo()
    {
        //mark each state with a random color
        CsvAdjacencyListFile statesAdjacencyFile("48US.txt");
        USStatesSvgFile statesSvg;
        const IntegerIdAdjacencyListFile::NodeIdToNodeStringMap& statesMap = 
            statesAdjacencyFile.getNodeIdToNodeStringMap();
        IntegerIdAdjacencyListFile::NodeIdToNodeStringMap::const_iterator state_it;
        for (state_it=statesMap.begin(); state_it!=statesMap.end(); ++state_it)
        {
            statesSvg.markStateByAbbreviation(state_it->second, randomColor());
            statesSvg.saveFile("output/states_marked_with_random_colors.svg");
        }
    }

    void markEachCountyWithRandomColor_Demo()
    {
        //mark each county with a random color
        USCountiesAdjacencyListFile countiesAdjacencyFile;
        USCountiesSvgFile countiesSvg;
        const IntegerIdAdjacencyListFile::NodeIdToNodeStringMap& countiesMap = 
            countiesAdjacencyFile.getNodeIdToNodeStringMap();
        IntegerIdAdjacencyListFile::NodeIdToNodeStringMap::const_iterator county_it;
        for (county_it=countiesMap.begin(); county_it!=countiesMap.end(); ++county_it)
        {
            countiesSvg.markCountyByFips(county_it->first, randomColor());
            countiesSvg.saveFile("output/counties_marked_with_random_colors.svg");
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
