#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cassert>

#include "Game.h"
#include "USCountiesGame.h"
#include "TravelingSalesmanProblem.h"
#include "Graph.h"
#include "USCountiesSvgFile.h"
#include "USStatesSvgFile.h"
#include "USCountiesAdjacencyListFile.h"
#include "CsvAdjacencyListFile.h"
#include "Demos.h"

namespace GraphGame
{

    enum Mode
    {
        MODE_UNDEFINED,
        MODE_GAME,
        MODE_BFS_DEMO,
        MODE_TSP_DEMO,
        MODE_BIPARTITE_DEMO,
        MODE_TEST
    };

    enum AdjacencyFilePreset
    {
        ADJACENCY_FILE_PRESET_NONE,
        ADJACENCY_FILE_PRESET_US_STATES,
        ADJACENCY_FILE_PRESET_US_COUNTIES
    };

    /*
     * Application Globals
     */
    bool showWarnings = false;

    int parseCommandLineArgs(int argc, char *argv[])
    {
        Mode mode = MODE_UNDEFINED;
        bool usingAdjacencyFilePreset = false;
        AdjacencyFilePreset adjacencyFilePreset = ADJACENCY_FILE_PRESET_NONE;
        const char* adjacencyFile = NULL; //filename or preset name
        std::string start, goal;
        const char* svgFile = NULL;
        //AdjacencyFileFormat fmt = UNDEFINED;

        if (argc == 1)
        {
            std::cout << "Usage: ?\n";
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
                    else if (strcmp(argv[i], "bfs-demo") == 0)
                    {
                        mode = MODE_BFS_DEMO;
                    }
                    else if (strcmp(argv[i], "tsp-demo") == 0)
                    {
                        mode = MODE_TSP_DEMO;
                    }
                    else if (strcmp(argv[i], "bipartite-demo") == 0)
                    {
                        mode = MODE_BIPARTITE_DEMO;
                    }
                    else if (strcmp(argv[i], "test") == 0)
                    {
                        mode = MODE_TEST;
                    }
                    else
                    {
                        std::cout << "Invalid mode \"" << argv[i] << "\"\n";
                        return 1;
                    }
                }
                else
                {
                    std::cerr << "Expected mode after -m flag\n";
                    return 1;
                }
            }
            else if (strcmp(argv[i], "-af") == 0)
            {
                if (++i < argc)
                {
                    if (strcmp(argv[i], "preset") == 0)
                    {
                        usingAdjacencyFilePreset = true;
                    }
                    else
                    {
                        adjacencyFile = argv[i];
                    }
                }
                else
                {
                    std::cerr << "Expected filename or preset\n";
                    return 1;
                }
                if (++i < argc)
                {
                    if (usingAdjacencyFilePreset)
                    {
                        if (strcmp(argv[i], "us-states") == 0)
                        {
                            adjacencyFilePreset = ADJACENCY_FILE_PRESET_US_STATES;
                        }
                        else if (strcmp(argv[i], "us-counties") == 0)
                        {
                            adjacencyFilePreset = ADJACENCY_FILE_PRESET_US_COUNTIES;
                        }
                        else
                        {
                            std::cerr << "Unrecognized preset \"" << argv[i] << "\"\n";
                            return 1;
                        }
                    }
                    else
                    {
                        std::cerr << "Not implemented\n";
                        return 1;
                        /*
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
                            std::cout << "Invalid format \"" << argv[i] << "\"\n";
                            return 1;
                        }
                        */
                    }
                }
                else
                {
                    if (usingAdjacencyFilePreset)
                    {
                        std::cerr << "No preset specified\n";
                    }
                    else
                    {
                        std::cerr << "No file format specified\n";
                    }
                    return 1;
                }
            }
            else if (strcmp(argv[i], "-s") == 0)
            {
                if (++i < argc)
                {
                    start = argv[i];
                }
                else
                {
                    std::cout << "Expected value after -s flag\n";
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
                    std::cout << "Expected value after -g flag\n";
                    return 1;
                }
            }
            else if (strcmp(argv[i], "-w") == 0)
            {
                showWarnings = true;
            }
            else
            {
                std::cout << "Unrecognized argument \"" << argv[i] << "\"\n";
                return 1;
            }
        }

        if (mode == MODE_UNDEFINED)
        {
            std::cout << "Mode not specified\n";
            return 1;
        }

        if (mode == MODE_GAME)
        {
            if (!usingAdjacencyFilePreset)
            {
                std::cerr << "No adjacency file specified\n";
                return 1;
            }
            std::cout << "Starting game mode...\n";
            if (usingAdjacencyFilePreset)
            {
                switch (adjacencyFilePreset)
                {
                case ADJACENCY_FILE_PRESET_US_STATES:
                    Game(new CsvAdjacencyListFile("48US.txt"), start, goal);
                    break;
                case ADJACENCY_FILE_PRESET_US_COUNTIES:
                    USCountiesGame(start, goal);
                    break;
                default:
                    assert(false);
                }
            }
        }
        else if (mode == MODE_BFS_DEMO)
        {
            breadthFirstSearch_Demo(start, goal);
        }
        else if (mode == MODE_TSP_DEMO)
        {
            std::cout << "Traveling Salesman Problem\n";
            TravelingSalesmanProblem tsp;
        }
        else if (mode == MODE_BIPARTITE_DEMO)
        {
            isBipartite_Demo();
        }
        else if (mode == MODE_TEST)
        {
            std::cout << "Test stub\n";
        }
        return 0;
    }
}

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));
    GraphGame::parseCommandLineArgs(argc,argv);
    return 0;
}
