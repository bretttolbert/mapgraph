#ifndef GAME_H
#define GAME_H

#include <string>

#include "Graph.h"
#include "IntegerIdAdjacencyListFile.h"
#include "SvgFile.h"

namespace GraphGame
{
#define DEBUG_GAME 0
    class Game
    {
    public:
        Game(IntegerIdAdjacencyListFile* adjacencyFile,
             SvgFile* svg,
             const std::string& start, 
             const std::string& goal);
        ~Game();
        virtual void mainLoop();
        virtual void displayChoicesAndGetInput();
        virtual bool processInput();
        virtual void chooseStartNode();
        virtual void chooseGoalNode();
        virtual void printStats(bool final);
    protected:
		bool validateInput(const std::set<std::string>& choices, 
						   const std::string& input,
						   std::string& match);
        IntegerIdAdjacencyListFile* adjacencyFile;
        SvgFile* svg;
        Graph<int> graph;
        int startNodeId;
        int currentNodeId;
        int goalNodeId;
        std::vector<int> optimalPath;
        int moves;
        bool printNeighbors;
    };
}

#endif
