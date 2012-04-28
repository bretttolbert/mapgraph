#include "USCountiesGame.h"

namespace GraphGame
{
    USCountiesGame::USCountiesGame(const std::string& start, 
                                   const std::string& goal) :
        Game::Game(new USCountiesAdjacencyListFile(), start, goal),
        statesAdjacencyFile("48US.txt")
    {
        
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
        if (final || DEBUG_GAME)
        {
            svg.saveFile("output.svg");
        }
        Game::printStats(final);
    }

    void USCountiesGame::mainLoop()
    {
		if (moves == 0)
		{
            //mark optimalPath on svg
            std::vector<int>::const_iterator it;
            for (it=optimalPath.begin(); it!=optimalPath.end(); ++it)
            {
                svg.markCountyByFips(*it, "red");
            }
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
        svg.markCountyByFips(currentNodeId, "green");
        return Game::processInput();
    }
}
