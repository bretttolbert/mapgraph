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
    extern std::ofstream log;
    extern bool showWarnings;

    Game::Game(const std::string& start, 
             const std::string& goal) :
        adjacencyFile(),
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
            startNodeId = adjacencyFile.getFipsCodeByCountyName(start);
        }
        currentNodeId = startNodeId;
        if (goal.empty())
        {
            chooseGoalNode();
        }
        else
        {
            goalNodeId = adjacencyFile.getFipsCodeByCountyName(goal);
        }
        optimalPath = graph.breadthFirstSearch(startNodeId, goalNodeId);
        mainLoop();
    }

    void Game::chooseStartNode()
    {
        startNodeId = adjacencyFile.getRandomFipsCode();
        /*
        std::vector<std::string> candidates = graph.getNodeValues();
        startNode = candidates[rand() % candidates.size()];
        */
    }

    void Game::chooseGoalNode()
    {
        goalNodeId = adjacencyFile.getRandomFipsCode();
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
            std::set<std::string> names = adjacencyFile.getCountyNamesByFipsCodes(neighbors);
            std::set<std::string>::const_iterator it;
            for (it=names.begin(); it!=names.end(); ++it)
            {
                std::cout << *it << "\n";
            }
            std::cout << std::endl;
        }
        std::cout << "Goal: " << adjacencyFile.getCountyNameByFipsCode(goalNodeId) << std::endl;
        std::cout << "Moves: " << moves << std::endl;
        if (final || DEBUG_GAME)
        {
            std::cout << "Optimal Path:\n";
            std::vector<std::string> names = adjacencyFile.getCountyNamesByFipsCodes(optimalPath);
            std::vector<std::string>::const_iterator it;
            for (it=names.begin(); it!=names.end(); ++it)
            {
                std::cout << *it << "\n";
            }
            std::cout << "(" << optimalPath.size() << " moves)\n";
        }
        //std::cout << "Fuel: " << fuel << "/" << maxFuel << std::endl;
        std::cout << std::endl;
    }

    void Game::mainLoop()
    {
        while (1)
        {
            std::cout << "\nWelcome to " << adjacencyFile.getCountyNameByFipsCode(currentNodeId) << std::endl;
            const std::set<int>& neighbors = adjacencyFile.getNeighbors(currentNodeId);
            std::set<std::string> neighborNames;
            std::set<int>::const_iterator it;
            for (it=neighbors.begin(); it!=neighbors.end(); ++it)
            {
                std::string name = adjacencyFile.getCountyNameByFipsCode(*it);
                neighborNames.insert(name);
            }
            printStats(false);
            std::string dest;
            std::cout << "Enter Destination: ";
            getline(std::cin, dest);
            std::set<std::string>::const_iterator jt;
            StringUtils::CaseInsensitiveComparator comp(dest);
            while ((jt = std::find_if(neighborNames.begin(), neighborNames.end(), comp)) == neighborNames.end())
            {
                std::cout << "Invalid Destination. Destination must be a neighbor.\n";
                std::cout << "Enter Destination: ";
                std::cin >> dest;
                comp.setMatchTarget(dest);
            }
            currentNodeId = adjacencyFile.getFipsCodeByCountyName(*jt);
            ++moves;
            if (currentNodeId == goalNodeId)
            {
                std::cout << "Goal Reached!!!\nFinal Stats:\n";
                printStats(true);
                break;
            }
        }
    }
}