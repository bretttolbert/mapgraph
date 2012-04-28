#ifndef US_COUNTIES_GAME_H
#define US_COUNTIES_GAME_H

#include "Game.h"

namespace GraphGame
{
    class USCountiesGame : public Game
    {
    public:
        USCountiesGame(const std::string& start,
                       const std::string& goal);
        virtual void chooseStartNode();
        virtual void chooseGoalNode();
        virtual void mainLoop();
        virtual void displayChoicesAndGetInput();
        virtual bool processInput();
        virtual void printStats(bool final);
    protected:
        CsvAdjacencyListFile statesAdjacencyFile;
        USCountiesSvgFile svg;
    };
}

#endif
