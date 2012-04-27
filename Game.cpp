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

namespace GraphGame
{
    extern bool showWarnings;

    Game::Game(const std::string& start, 
             const std::string& goal) :
        statesAdjacencyFile("48US.txt"),
        graph(adjacencyFile.getAdjacencyList()),
        //maxFuel(10),
        //fuel(10),
        moves(0),
        printNeighbors(true)
    {
        if (start.empty())
        {
            chooseStartNode();
        }
        else
        {
            startNodeId = adjacencyFile.stringToNodeId(start);
        }
        currentNodeId = startNodeId;
        if (goal.empty())
        {
            chooseGoalNode();
        }
        else
        {
            goalNodeId = adjacencyFile.stringToNodeId(goal);
        }
        optimalPath = graph.breadthFirstSearch(startNodeId, goalNodeId);
        //mark optimalPath on svg
        std::vector<int>::const_iterator it;
        for (it=optimalPath.begin(); it!=optimalPath.end(); ++it)
        {
            svg.markCountyByFips(*it, "red");
        }
        mainLoop();
    }

    void Game::chooseStartNode()
    {
        startNodeId = adjacencyFile.getRandomCountyFromState("AL");
        /*
        std::vector<std::string> candidates = graph.getNodeValues();
        startNode = candidates[rand() % candidates.size()];
        */
    }

    void Game::chooseGoalNode()
    {
        goalNodeId = adjacencyFile.getRandomCountyFromState("AL");
        /*
        std::vector<std::string> candidates = graph.getNodeValues();
        std::vector<std::string> nonCandidates = graph.getNeighbors(currentNode);
        nonCandidates.push_back(currentNode);
        std::vector<std::string>::const_iterator it;
        for (it=nonCandidates.begin(); it!=nonCandidates.end(); ++it)
        {
            candidates.erase(std::remove(candidates.begin(), candidates.end(), *it), candidates.end());
        }
        goalNode = candidates[rand() % candidates.size()];
        */
    }

    void Game::printStats(bool final)
    {
        if (printNeighbors)
        {
            std::cout << "Neighbors:\n";
            const std::set<int>& neighbors = adjacencyFile.getNeighbors(currentNodeId);
            std::set<int>::const_iterator it;
            for (it=neighbors.begin(); it!=neighbors.end(); ++it)
            {
                std::cout << adjacencyFile.nodeIdToString(*it) << "\n";
            }
            std::cout << std::endl;
        }
        std::cout << "Goal: " << adjacencyFile.nodeIdToString(goalNodeId) << std::endl;
        std::cout << "Moves: " << moves << std::endl;
        if (final || DEBUG_GAME)
        {
            std::cout << "Optimal Path:\n";
            std::vector<int>::const_iterator it;
            for (it=optimalPath.begin(); it!=optimalPath.end(); ++it)
            {
                std::cout << adjacencyFile.nodeIdToString(*it) << "\n";
            }
            std::cout << "(" << optimalPath.size() << " moves)\n";
            svg.saveFile("output.svg");
        }
        //std::cout << "Fuel: " << fuel << "/" << maxFuel << std::endl;
        std::cout << std::endl;
    }

    void Game::mainLoop()
    {
        while (1)
        {
            std::cout << "\nWelcome to " << adjacencyFile.nodeIdToString(currentNodeId) << std::endl;
            const std::set<int>& neighbors = adjacencyFile.getNeighbors(currentNodeId);
            std::set<std::string> neighborNames;
            std::set<int>::const_iterator it;
            for (it=neighbors.begin(); it!=neighbors.end(); ++it)
            {
                std::string name = adjacencyFile.nodeIdToString(*it);
                neighborNames.insert(name);
            }
            printStats(false);
			if (moves == 0)
			{
				std::cout << "Tip: You do not have to enter the destination exactly. "
						  << "For example, to choose \"Montgomery County, AL\", simply enter \"mont\".\n";
			}
            std::string input;
            std::cout << "Enter Destination: ";
            getline(std::cin, input);
            std::set<std::string>::const_iterator jt;
			std::string match;
			while (!validateInput(neighborNames, input, match))
            {
                std::cout << "Invalid Destination. Destination must match a neighbor.\n";
                std::cout << "Enter Destination: ";
                std::cin >> input;
            }
            currentNodeId = adjacencyFile.stringToNodeId(match);
            svg.markCountyByFips(currentNodeId, "green");
            svg.saveFile("output.svg"); //save it each time in case user aborts
            ++moves;
            if (currentNodeId == goalNodeId)
            {
                std::cout << "Goal Reached!!!\nFinal Stats:\n";
                printStats(true);
                break;
            }
        }
    }

	bool Game::validateInput(const std::set<std::string>& choices, const std::string& input, std::string& match)
	{
		std::string linput(input);
		std::transform(linput.begin(), linput.end(), linput.begin(), ::tolower);
		std::set<std::string>::const_iterator it;
		for (it=choices.begin(); it!=choices.end(); ++it)
		{
			std::string lchoice(*it);
			std::transform(lchoice.begin(), lchoice.end(), lchoice.begin(), ::tolower);
			if (lchoice.find(linput) != std::string::npos)
			{
				match = *it;
				return true;
			}
		}
		return false;
	}
}
