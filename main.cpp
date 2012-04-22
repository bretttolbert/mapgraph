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

//#include "Game.h"
#include "Graph.h"
#include "UsCountiesSvgFile.h"
#include "USCountiesAdjacencyList.h"

enum Mode
{
    MODE_UNDEFINED,
    MODE_GAME,
    MODE_BFS,
    MODE_WASHINGTONS,
    MODE_TEST
};

/*
 * Application Globals
 */
bool showWarnings = false;

//performs BFS, prints info to console and updates SVG but does not save it
void performBfsAndUpdateSvg(USCountiesAdjacencyList& adjacencyList,
                            USCountiesSvgFile& svg,
                            Graph<int>& graph,
                            int startFips, 
                            int goalFips);

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    Mode mode = MODE_UNDEFINED;
    const char* adjacencyFile = NULL;
    std::string start, goal;
    const char* svgFile = NULL;
    //AdjacencyFileFormat fmt = UNDEFINED;

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
                else if (strcmp(argv[i], "washingtons") == 0)
                {
                    mode = MODE_WASHINGTONS;
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
        /*
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
        */
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

    /*
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
    */

    if (mode == MODE_GAME)
    {
        //Game(adjacencyFile, fmt, start, goal);
        std::cout << "Temporarily disabled\n";
    }
    else if (mode == MODE_BFS)
    {
        USCountiesAdjacencyList adjacencyList;
        USCountiesSvgFile svg;
        Graph<int> graph((const Graph<int>::AdjacencyList&)adjacencyList.getAdjacencyList());
        int startFips, goalFips;
        if (start.empty())
        {
            startFips = adjacencyList.getRandomFipsCode();
            start = adjacencyList.getCountyNameByFipsCode(startFips);
        }
        else
        {
            startFips = adjacencyList.getFipsCodeByCountyName(start);
        }
        if (goal.empty())
        {
            goalFips = adjacencyList.getRandomFipsCode();
            goal = adjacencyList.getCountyNameByFipsCode(goalFips);
        }
        else
        {
            goalFips = adjacencyList.getFipsCodeByCountyName(goal);
        }
        performBfsAndUpdateSvg(adjacencyList, svg, graph, startFips, goalFips);
        svg.saveFile("output.svg");
    }
    else if (mode == MODE_WASHINGTONS)
    {
        USCountiesAdjacencyList adjacencyList;
        USCountiesSvgFile svg;
        Graph<int> graph((const Graph<int>::AdjacencyList&)adjacencyList.getAdjacencyList());
        const USCountiesAdjacencyList::CountyFipsMap& counties = adjacencyList.getCounties();
        std::vector<std::pair<int,std::string> > washingtons;
        USCountiesAdjacencyList::CountyFipsMap::const_iterator it;
        for (it=counties.begin(); it!=counties.end(); ++it)
        {
            if (it->second.find("Washington") != std::string::npos)
            {
                washingtons.push_back(*it);
            }
        }
        std::cout << "found " << washingtons.size() << " Washingtons\n";
        std::vector<std::pair<int,std::string> >::const_iterator jt;
        for (jt=washingtons.begin(); jt!=washingtons.end(); ++jt)
        {
            performBfsAndUpdateSvg(adjacencyList, svg, graph, 
                adjacencyList.getFipsCodeByCountyName("Madison County, AL"), jt->first);
        }
        svg.saveFile("output.svg");
    }
    else if (mode == MODE_TEST)
    {
        std::cout << "Test stub\n";
    }
    return 0;
}

void performBfsAndUpdateSvg(USCountiesAdjacencyList& adjacencyList,
                            USCountiesSvgFile& svg,
                            Graph<int>& graph,
                            int startFips, 
                            int goalFips)
{
    std::string start = adjacencyList.getCountyNameByFipsCode(startFips);
    std::string goal = adjacencyList.getCountyNameByFipsCode(goalFips);
    std::cout << "Performing BFS from " << start << " to " << goal << "...\n";
    std::cout << "Start: " << start << " (fips: " << USCountiesAdjacencyList::fipsToString(startFips) << ")\n";
    std::cout << "Goal: " << goal << " (fips: " << USCountiesAdjacencyList::fipsToString(goalFips) << ")\n";
    std::vector<int> path = graph.breadthFirstSearch(startFips,goalFips);
    std::cout << "Optimal Path:\n";
    std::vector<int>::const_iterator it;
    for (it=path.begin(); it!=path.end(); ++it)
    {
        std::cout << adjacencyList.getCountyNameByFipsCode(*it) << std::endl;\
        svg.markCountyByFips(*it, "purple");
    }
    std::cout << "(" << path.size() << " moves)\n"; 
}