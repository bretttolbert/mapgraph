#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <queue>
#include <set>

#include "Game.h"
#include "StringUtils.h"

Game::Game(const char* adjacencyFile, AdjacencyFileFormat fmt) :
	//maxFuel(10),
	//fuel(10),
	moves(0),
	printNeighbors(true)
{
	srand((unsigned int)time(NULL));
	graph.readAdjacencyListFromFile(adjacencyFile, fmt);
	chooseStartNode();
	currentNode = startNode;
	chooseTargetNode();
	optimalPath = graph.breadthFirstSearch(startNode, targetNode);
	mainLoop();
}

void Game::chooseStartNode()
{
	std::vector<std::string> candidates = graph.getNodeValues();
	startNode = candidates[rand() % candidates.size()];
}

void Game::chooseTargetNode()
{
	std::vector<std::string> candidates = graph.getNodeValues();
	std::vector<std::string> nonCandidates = graph.getNeighbors(currentNode);
	nonCandidates.push_back(currentNode);
	std::vector<std::string>::const_iterator it;
	for (it=nonCandidates.begin(); it!=nonCandidates.end(); ++it)
	{
		candidates.erase(std::remove(candidates.begin(), candidates.end(), *it), candidates.end());
	}
	targetNode = candidates[rand() % candidates.size()];
}

void Game::printStats(bool final)
{
	if (printNeighbors)
	{
		std::cout << "Neighbors:\n";
		graph.printNeighbors(currentNode, "\n");
		std::cout << std::endl;
	}
	std::cout << "Target: " << targetNode << std::endl;
	std::cout << "Moves: " << moves << std::endl;
	if (final)
	{
		std::cout << "Optimal Path: ";
		std::ostream_iterator<std::string> output(std::cout, " ");
		std::copy(optimalPath.begin(), optimalPath.end(), output);
		std::cout << "(" << optimalPath.size() << " moves)\n";
	}
	//std::cout << "Fuel: " << fuel << "/" << maxFuel << std::endl;
}

void Game::mainLoop()
{
	while (1)
	{
		std::cout << "\nWelcome to " << currentNode << std::endl;
		std::vector<std::string> neighbors = graph.getNeighbors(currentNode);
		printStats(false);
		std::string dest;
		std::cout << "Enter Destination: ";
		getline(std::cin, dest);
		std::vector<std::string>::const_iterator it;
		StringUtils::CaseInsensitiveComparator comp(dest);
		while ((it = std::find_if(neighbors.begin(), neighbors.end(), comp)) == neighbors.end())
		{
			std::cout << "Invalid Destination. Destination must be a neighbor.\n";
			std::cout << "Enter Destination: ";
			std::cin >> dest;
			comp.setMatchTarget(dest);
		}
		currentNode = *it;
		++moves;
		if (currentNode == targetNode)
		{
			std::cout << "Target Reached!!!\nFinal Stats:\n";
			printStats(true);
			break;
		}
	}
}