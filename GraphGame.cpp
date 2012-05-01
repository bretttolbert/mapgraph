#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cassert>

#include "GraphGame.h"
#include "Game.h"
#include "USCountiesGame.h"
#include "TravelingSalesmanProblem.h"
#include "Graph.h"
#include "USCountiesSvgFile.h"
#include "USStatesSvgFile.h"
#include "USCountiesAdjacencyListFile.h"
#include "CsvAdjacencyListFile.h"
#include "Demos.h"
#include "StringUtils.h"

namespace GraphGame
{
    bool GraphGame::showWarnings = false;

    std::string GraphGame::absolutePath(const char* relativePath)
    {
        std::string result = GRAPH_GAME_ROOT;
        if (result[result.size()-1] != '/')
        {
            result += '/';
        }
        result += relativePath;
        return result;
    }

    int GraphGame::parseCommandLineArgs(int argc, char *argv[])
    {
        Mode mode = MODE_UNDEFINED;
        bool usingAdjacencyFilePreset = false;
        AdjacencyFilePreset adjacencyFilePreset = ADJACENCY_FILE_PRESET_NONE;
        const char* adjacencyFile = NULL; //filename or preset name
        std::string node1, node2;
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
                    else if (strcmp(argv[i], "neighbors") == 0)
                    {
                        mode = MODE_NEIGHBORS;
                    }
                    else if (strcmp(argv[i], "random-coloring") == 0)
                    {
                        mode = MODE_RANDOM_COLORING;
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
            else if (strcmp(argv[i], "-n1") == 0)
            {
                if (++i < argc)
                {
                    node1 = argv[i];
                }
                else
                {
                    std::cout << "Expected value after -n1 flag\n";
                    return 1;
                }
            }
            else if (strcmp(argv[i], "-n2") == 0)
            {
                if (++i < argc)
                {
                    node2 = argv[i];
                }
                else
                {
                    std::cout << "Expected value after -n2 flag\n";
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
                    {
                        IntegerIdAdjacencyListFile* af = new CsvAdjacencyListFile(absolutePath("48US.txt"));
                        USStatesSvgFile* svg = new USStatesSvgFile(af);
                        Game(af, svg, node1, node2);
                        break;
                    }
                    case ADJACENCY_FILE_PRESET_US_COUNTIES:
                    {
                        USCountiesGame(node1, node2);
                        break;
                    }
                    default:
                        assert(false);
                }
            }
        }
        else if (mode == MODE_BFS_DEMO 
                || mode == MODE_TSP_DEMO
                || mode == MODE_BIPARTITE_DEMO
                || mode == MODE_NEIGHBORS
                || mode == MODE_RANDOM_COLORING
                || mode == MODE_TEST)
        {
            IntegerIdAdjacencyListFile* af = NULL;
            SvgFile* svg = NULL;
            if (usingAdjacencyFilePreset)
            {
                switch (adjacencyFilePreset)
                {
                case ADJACENCY_FILE_PRESET_US_STATES:
                    std::cout << "Using US States preset\n";
                    af = new CsvAdjacencyListFile(absolutePath("48US.txt"));
                    svg = new USStatesSvgFile(af);
                    break;
                case ADJACENCY_FILE_PRESET_US_COUNTIES:
                    std::cout << "Using US Counties preset\n";
                    af = new USCountiesAdjacencyListFile();
                    svg = new USCountiesSvgFile();
                    break;
                default:
                    std::cerr << "Error: No adjacency file specified\n";
                    exit(1);
                }
            }
            else
            {
                std::cerr << "Error: No adjacency file specified\n";
                exit(1);
            }
            int node1Id, node2Id;
            if (node1.empty())
            {
                node1Id = af->getRandomNodeId();
                node1 = af->nodeIdToString(node1Id);
            }
            else
            {
                std::string match;
                if (!(node1Id = af->nodeSearch(node1, match)))
                {
                    std::cerr << "No matching node found for \"" << node1 << "\"\n";
                    exit(1);
                }
                else
                {
                    std::cout << "Found match for \"" << node1 << "\": " << match << " (" << node1Id << ")\n";
                    node1 = match;
                }
            }
            if (node2.empty())
            {
                node2Id = af->getRandomNodeId();
                node2 = af->nodeIdToString(node2Id);
            }
            else
            {
                std::string match;
                if (!(node2Id = af->nodeSearch(node2, match)))
                {
                    std::cerr << "No matching node found for \"" << node2 << "\"\n";
                    exit(1);
                }
                else
                {
                    std::cout << "Found match for \"" << node2 << "\": " << match << " (" << node2Id << ")\n";
                    node2 = match;
                }
            }
            
            if (mode == MODE_BFS_DEMO)
            {
                std::cout << "Starting breadth first search mode...\n";
                std::cout << "Performing BFS from " << node1 << " (" << node1Id << ") "
                          << "to " << node2 << " (" << node2Id << ")...\n";
                breadthFirstSearch_Demo(af, svg, node1Id, node2Id);
            }
            else if (mode == MODE_TSP_DEMO)
            {
                std::cout << "Traveling Salesman Problem\n";
                TravelingSalesmanProblem tsp;
            }
            else if (mode == MODE_BIPARTITE_DEMO)
            {
                isBipartite_Demo(af, svg);
            }
            else if (mode == MODE_NEIGHBORS)
            {
                showNeighbors(af, svg, node1Id);
            }
            else if (mode == MODE_RANDOM_COLORING)
            {
                randomColoring(af, svg);
            }
            else if (mode == MODE_TEST)
            {
                std::cout << "Test stub\n";
            }
            else
            {
                assert(false);
            }

            delete af;
            delete svg;
        }
        return 0;
    }
}
