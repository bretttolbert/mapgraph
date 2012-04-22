#ifndef GAME_H
#define GAME_H

#include <string>

#include "Graph.h"

#define DEBUG_GAME 1

class Game
{
public:
    Game(const char* adjacencyFile, 
         AdjacencyFileFormat fmt, 
         const char* start, 
         const char* goal);
    void mainLoop();
    void chooseStartNode();
    void chooseGoalNode();
    void printStats(bool final);
private:
    Graph graph;
    //int maxFuel;
    //int fuel;
    std::string startNode;
    std::string currentNode;
    std::string goalNode;
    std::vector<std::string> optimalPath;
    int moves;
    bool printNeighbors;
};

#endif