#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cmath>
#include <ctime>

#include "Game.h"
#include "Graph.h"
#include "UsCountiesSvgFile.h"

enum Mode
{
    MODE_UNDEFINED,
    MODE_GAME,
    MODE_BFS,
    MODE_TEST
};

/*
 * Application Globals
 */
bool showWarnings = false;

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    Mode mode = MODE_UNDEFINED;
    const char* adjacencyFile = NULL;
    const char* start = NULL;
    const char* goal = NULL;
    const char* svgFile = NULL;
    AdjacencyFileFormat fmt = UNDEFINED;

    if (argc == 1)
    {
        std::cerr << "Usage: ?\n";
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
                else
                {
                    std::cerr << "Invalid mode \"" << argv[i] << "\"\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Error: Expected mode after -m flag\n";
                return 1;
            }
        }
        else if (strcmp(argv[i], "-af") == 0)
        {
            if (++i < argc)
            {
                adjacencyFile = argv[i];
            }
            else
            {
                std::cerr << "Error: Expected filename\n";
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
                    std::cerr << "Invalid format \"" << argv[i] << "\"\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Error: Expected file format specifier\n";
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
                std::cerr << "Error: Expected value after -s flag\n";
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
                std::cerr << "Error: Expected value after -g flag\n";
                return 1;
            }
        }
        else if (strcmp(argv[i], "-w") == 0)
        {
            showWarnings = true;
        }
        else
        {
            std::cerr << "Unrecognized argument \"" << argv[i] << "\"\n";
            return 1;
        }
    }

    if (mode == MODE_UNDEFINED)
    {
        std::cerr << "Error: Mode not specified\n";
        return 1;
    }

    if (mode == MODE_GAME || mode == MODE_BFS)
    {
        if (adjacencyFile == NULL)
        {
            std::cerr << "Error: No adjacency file specified\n";
            return 1;
        }
        else if (fmt == UNDEFINED)
        {
            std::cerr << "Error: Adjacency file format not specified\n";
            return 1;
        }
    }

    if (mode == MODE_GAME)
    {
        Game(adjacencyFile, fmt, start, goal);
    }
    else if (mode == MODE_BFS)
    {
        Graph graph;
        graph.readAdjacencyListFromFile(adjacencyFile, fmt);
        std::vector<std::string> candidates = graph.getNodeValues();
        if (!start || start == "")
        {
            start = candidates[rand() % candidates.size()].c_str();
        }
        if (!goal || goal == "")
        {
            goal = candidates[rand() % candidates.size()].c_str();
        }
        std::cout << "Performing BFS from " << start << " to " << goal << "...\n";
        std::vector<std::string> path = graph.breadthFirstSearch(start, goal);
        std::cout << "Optimal Path:\n";
        std::ostream_iterator<std::string> output(std::cout, "\n");
        std::copy(path.begin(), path.end(), output);
        std::cout << "(" << path.size() << " moves)\n";
        //create svg
        UsCountiesSvgFile svg;
        std::vector<std::string>::const_iterator it;
        for (it=path.begin(); it!=path.end(); ++it)
        {
            const char * countyName = it->c_str();
            const char * fill = "red";
            if (it == path.begin())
            {
                fill = "blue";
            }
            if (!svg.markCountyByName(countyName, fill) && showWarnings)
            {
                std::cerr << "Warning: County \"" << countyName << "\" not found in SVG file\n";
            }
        }
        svg.saveFile("output.svg");
    }
    else if (mode == MODE_TEST)
    {
        UsCountiesSvgFile svg;
        svg.markCountyByName("Richmond Co., VA", "red");
        svg.markCountyByName("Richmond, VA", "blue");
        svg.saveFile("output.svg");
    }
    return 0;
}