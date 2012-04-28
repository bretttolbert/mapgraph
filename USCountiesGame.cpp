#include "USCountiesGame.h"
#include "USCountiesAdjacencyListFile.h"
#include "USCountiesSvgFile.h"

namespace GraphGame
{
    USCountiesGame::USCountiesGame(const std::string& start, 
                                   const std::string& goal) :
        Game::Game(new USCountiesAdjacencyListFile(), 
                   new USCountiesSvgFile(), start, goal),
        statesAdjacencyFile("48US.txt")
    {
        
    }

    USCountiesGame::~USCountiesGame()
    {
        delete adjacencyFile;
    }

    void USCountiesGame::chooseStartNode()
    {
        startNodeId = ((USCountiesAdjacencyListFile*)adjacencyFile)->getRandomCountyFromState("AL");
    }

    void USCountiesGame::chooseGoalNode()
    {
        goalNodeId = ((USCountiesAdjacencyListFile*)adjacencyFile)->getRandomCountyFromState("AL");
    }

    void USCountiesGame::printStats(bool final)
    {
        Game::printStats(final);
    }

    void USCountiesGame::mainLoop()
    {
		if (moves == 0)
		{
			std::cout << "Tip: You do not have to enter the destination exactly. "
					  << "For example, to choose \"Montgomery County, AL\", simply enter \"mont\".\n";
		}
        Game::mainLoop();
    }

    void USCountiesGame::displayChoicesAndGetInput()
    {
        Game::displayChoicesAndGetInput();
    }

    bool USCountiesGame::processInput()
    {
        return Game::processInput();
    }
}
