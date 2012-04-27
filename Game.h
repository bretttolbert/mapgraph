#ifndef GAME_H
#define GAME_H

#include <string>

#include "Graph.h"
#include "USCountiesAdjacencyListFile.h"
#include "USCountiesSvgFile.h"
#include "CsvAdjacencyListFile.h"

namespace GraphGame
{
#define DEBUG_GAME 0
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
		bool validateInput(const std::set<std::string>& choices, 
						   const std::string& input,
						   std::string& match);
        USCountiesAdjacencyListFile adjacencyFile;
        CsvAdjacencyListFile statesAdjacencyFile;
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
