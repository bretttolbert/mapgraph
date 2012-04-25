#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cmath>
#include <ctime>

#include "Game.h"
#include "TravelingSalesmanProblem.h"
#include "Graph.h"
#include "USCountiesSvgFile.h"
#include "USCountiesAdjacencyListFile.h"

namespace GraphGame
{

    enum Mode
    {
        MODE_UNDEFINED,
        MODE_GAME,
        MODE_BFS,
        MODE_TSP,
        MODE_TEST
    };

    /*
     * Application Globals
     */
    bool showWarnings = false;
    std::ofstream log;

    //returns hex code for a random color
    std::string randomColor();

    int parseCommandLineArgs(int argc, char *argv[])
    {
        Mode mode = MODE_UNDEFINED;
        const char* adjacencyFile = NULL;
        std::string start, goal;
        const char* svgFile = NULL;
        //AdjacencyFileFormat fmt = UNDEFINED;

        if (argc == 1)
        {
            log << "Usage: ?\n";
            return 1;
        }

        for (int i=1; i<argc; ++i)
        {
            if (strcmp(argv[i], "-m") == 0)
            {
                if (++i < argc)
                {
                    if (strcmp(argv[i], "game") == 0)
                    {
                        mode = MODE_GAME;
                    }
                    else if (strcmp(argv[i], "bfs") == 0)
                    {
                        mode = MODE_BFS;
                    }
                    else if (strcmp(argv[i], "test") == 0)
                    {
                        mode = MODE_TEST;
                    }
                    else if (strcmp(argv[i], "tsp") == 0)
                    {
                        mode = MODE_TSP;
                    }
                    else
                    {
                        log << "Invalid mode \"" << argv[i] << "\"\n";
                        return 1;
                    }
                }
                else
                {
                    log << "Error: Expected mode after -m flag\n";
                    return 1;
                }
            }
            /*
            else if (strcmp(argv[i], "-af") == 0)
            {
                if (++i < argc)
                {
                    adjacencyFile = argv[i];
                }
                else
                {
                    log << "Error: Expected filename\n";
                    return 1;
                }
                if (++i < argc)
                {
                    if (strcmp(argv[i], "slcsv") == 0)
                    {
                        fmt = SINGLE_LINE_CSV;
                    }
                    else if (strcmp(argv[i], "mltdusc") == 0)
                    {
                        fmt = MULTI_LINE_TAB_DELIMITED_US_CENSUS;
                    }
                    else
                    {
                        log << "Invalid format \"" << argv[i] << "\"\n";
                        return 1;
                    }
                }
                else
                {
                    log << "Error: Expected file format specifier\n";
                    return 1;
                }
            }
            */
            else if (strcmp(argv[i], "-s") == 0)
            {
                if (++i < argc)
                {
                    start = argv[i];
                }
                else
                {
                    log << "Error: Expected value after -s flag\n";
                    return 1;
                }
            }
            else if (strcmp(argv[i], "-g") == 0)
            {
                if (++i < argc)
                {
                    goal = argv[i];
                }
                else
                {
                    log << "Error: Expected value after -g flag\n";
                    return 1;
                }
            }
            else if (strcmp(argv[i], "-w") == 0)
            {
                showWarnings = true;
            }
            else
            {
                log << "Unrecognized argument \"" << argv[i] << "\"\n";
                return 1;
            }
        }

        if (mode == MODE_UNDEFINED)
        {
            log << "Error: Mode not specified\n";
            return 1;
        }

        /*
        if (mode == MODE_GAME || mode == MODE_BFS)
        {
            if (adjacencyFile == NULL)
            {
                log << "Error: No adjacency file specified\n";
                return 1;
            }
            else if (fmt == UNDEFINED)
            {
                log << "Error: Adjacency file format not specified\n";
                return 1;
            }
        }
        */

        if (mode == MODE_GAME)
        {
            std::cout << "Starting game mode...\n";
            Game(start, goal);
        }
        else if (mode == MODE_BFS)
        {
            USCountiesAdjacencyListFile adjacencyListFile;
            USCountiesSvgFile svg;
            Graph<int> graph(adjacencyListFile.getAdjacencyList());
            int startFips, goalFips;
            if (start.empty())
            {
                startFips = adjacencyListFile.getRandomFipsCode();
                start = adjacencyListFile.getCountyNameByFipsCode(startFips);
            }
            else
            {
                startFips = adjacencyListFile.getFipsCodeByCountyName(start);
            }
            if (goal.empty())
            {
                goalFips = adjacencyListFile.getRandomFipsCode();
                goal = adjacencyListFile.getCountyNameByFipsCode(goalFips);
            }
            else
            {
                goalFips = adjacencyListFile.getFipsCodeByCountyName(goal);
            }
            log << "Performing BFS from " << start << " to " << goal << "...\n";
            log << "Start: " << start << " (" << USCountiesAdjacencyListFile::fipsToString(startFips) << ")\n";
            log << "Goal: " << goal << " (" << USCountiesAdjacencyListFile::fipsToString(goalFips) << ")\n";
            std::vector<int> path = graph.breadthFirstSearch(startFips,goalFips);
            log << "Optimal Path:\n";
            std::vector<int>::const_iterator it;
            for (it=path.begin(); it!=path.end(); ++it)
            {
                std::vector<int>::const_iterator jt;
                for (jt=path.begin(); jt!=path.end(); ++it)
                {
                    std::string countyName = adjacencyListFile.getCountyNameByFipsCode(*it);
                    log << countyName << " (" << *it << "), ";
                    if (it == path.begin())
                    {
                        svg.markCountyByFips(*it, "red");
                    }
                    else
                    {
                        svg.markCountyByFips(*it, "gray");
                    }
                }
            }
            log << "\n(" << path.size() << " moves)\n"; 
            svg.saveFile("output.svg");
        }
        else if (mode == MODE_TSP)
        {
            log << "Traveling Salesman Problem\n";
            TravelingSalesmanProblem tsp;
        }
        else if (mode == MODE_TEST)
        {
            log << "Test stub\n";
        }
        return 0;
    }

    std::string randomColor()
    {
        std::ostringstream oss;
        oss << '#' << std::hex << std::setfill('0')
            << std::setw(2) << (rand() % 256)
            << std::setw(2) << (rand() % 256)
            << std::setw(2) << (rand() % 256);
        return oss.str();
    }

}

int main(int argc, char *argv[])
{
    GraphGame::log.open("output.txt", std::ios::out);
    srand((unsigned int)time(NULL));
    GraphGame::parseCommandLineArgs(argc,argv);
    GraphGame::log.close();
    return 0;
}
