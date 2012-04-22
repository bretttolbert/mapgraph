#ifndef GAME_H
#define GAME_H

#include <string>

#include "Graph.h"

class Game
{
public:
	Game(const char* adjacencyFile, AdjacencyFileFormat fmt);
	void mainLoop();
	void chooseStartNode();
	void chooseTargetNode();
	void printStats(bool final);
private:
	Graph graph;
	//int maxFuel;
	//int fuel;
	std::string startNode;
	std::string currentNode;
	std::string targetNode;
	std::vector<std::string> optimalPath;
	int moves;
	bool printNeighbors;
};

#endif