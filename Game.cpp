#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <queue>
#include <set>

#include "MapGraph.h"
#include "Game.h"
#include "StringUtils.h"
#include "Utils.h"

namespace MapGraph
{
    Game::Game(IntegerIdAdjacencyListFile* adjacencyFile_,
               SvgFile* svg_,
               const std::string& start, 
               const std::string& goal) :
        adjacencyFile(adjacencyFile_),
        svg(svg_),
        graph(adjacencyFile->getAdjacencyList()),
        moves(0),
        printNeighbors(true)
    {
        if (start.empty())
        {
            chooseStartNode();
        }
        else
        {
            startNodeId = adjacencyFile->stringToNodeId(start);
        }
        currentNodeId = startNodeId;
        if (goal.empty())
        {
            chooseGoalNode();
        }
        else
        {
            goalNodeId = adjacencyFile->stringToNodeId(goal);
        }
        optimalPath = graph.breadthFirstSearch(startNodeId, goalNodeId);
        mainLoop();
    }

    Game::~Game()
    {
        delete adjacencyFile;
        if (svg)
        {
            delete svg;
        }
    }

    void Game::chooseStartNode()
    {
        startNodeId = adjacencyFile->getRandomNodeId();
    }

    void Game::chooseGoalNode()
    {
        goalNodeId = adjacencyFile->getRandomNodeId();
    }

    void Game::printStats(bool final)
    {
        if (printNeighbors)
        {
            std::cout << "Neighbors:\n";
            const std::set<int>& neighbors = adjacencyFile->getNeighbors(currentNodeId);
            std::set<int>::const_iterator it;
            for (it=neighbors.begin(); it!=neighbors.end(); ++it)
            {
                std::cout << adjacencyFile->nodeIdToString(*it) << "\n";
            }
            std::cout << std::endl;
        }
        std::cout << "Goal: " << adjacencyFile->nodeIdToString(goalNodeId) << std::endl;
        std::cout << "Moves: " << moves << std::endl;
        if (final || DEBUG_GAME)
        {
            std::cout << "Optimal Path:\n";
            std::vector<int>::const_iterator it;
            for (it=optimalPath.begin(); it!=optimalPath.end(); ++it)
            {
                std::cout << adjacencyFile->nodeIdToString(*it) << "\n";
            }
            std::cout << "(" << optimalPath.size() << " moves)\n";
            if (svg) svg->saveFile("output/game-output.svg");
        }
        std::cout << std::endl;
    }

    void Game::mainLoop()
    {
		if (svg)
		{
            //mark optimalPath on svg
            std::vector<int>::const_iterator it;
            for (it=optimalPath.begin(); it!=optimalPath.end(); ++it)
            {
                if (it == optimalPath.begin())
                {
                    svg->markNode(*it, "#009900");
                }
                else if (next(it) == optimalPath.end())
                {
                    svg->markNode(*it, "#CC0000");
                }
                else
                {
                    svg->markNode(*it, "gray");
                }
            }
        }
        while (1)
        {
            displayChoicesAndGetInput();
            if (processInput())
            {
                break;
            }
        }
    }

    void Game::displayChoicesAndGetInput()
    {
        std::cout << "\nWelcome to " << adjacencyFile->nodeIdToString(currentNodeId) << std::endl;
        const std::set<int>& neighbors = adjacencyFile->getNeighbors(currentNodeId);
        std::set<std::string> neighborNames;
        std::set<int>::const_iterator it;
        for (it=neighbors.begin(); it!=neighbors.end(); ++it)
        {
            std::string name = adjacencyFile->nodeIdToString(*it);
            neighborNames.insert(name);
        }
        printStats(false);
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
        currentNodeId = adjacencyFile->stringToNodeId(match);
    }

    bool Game::processInput()
    {
        ++moves;
        if (currentNodeId == goalNodeId)
        {
            std::cout << "Goal Reached!!!\nFinal Stats:\n";
            printStats(true);
            return true;
        }
        else
        {
            if (svg) svg->markNode(currentNodeId, "blue");
            return false;
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
