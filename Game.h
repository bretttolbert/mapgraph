#ifndef GAME_H
#define GAME_H

#include <string>

#include "Graph.h"
#include "USCountiesAdjacencyListFile.h"
#include "USCountiesSvgFile.h"

namespace GraphGame
{
#define DEBUG_GAME 1
    class Game
    {
    public:
        Game(const std::string& start, 
             const std::string& goal);
        void mainLoop();
        void chooseStartNode();
        void chooseGoalNode();
        void printStats(bool final);
    private:
        USCountiesAdjacencyListFile adjacencyFile;
        USCountiesSvgFile svg;
        Graph<int> graph;
        //int maxFuel;
        //int fuel;
        int startNodeId;
        int currentNodeId;
        int goalNodeId;
        std::vector<int> optimalPath;
        int moves;
        bool printNeighbors;
    };
}

#endif