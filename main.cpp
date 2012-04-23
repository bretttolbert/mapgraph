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

//#include "Game.h"
#include "Graph.h"
#include "UsCountiesSvgFile.h"
#include "USCountiesAdjacencyListFile.h"

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
std::ofstream logfile;

//performs BFS, prints info to console and updates SVG but does not save it
void performBfsAndUpdateSvg(USCountiesAdjacencyListFile& adjacencyListFile,
                            USCountiesSvgFile& svg,
                            Graph<int>& graph,
                            int startFips, 
                            int goalFips);

//returns hex code for a random color
std::string randomColor();

int main(int argc, char *argv[])
{
    logfile.open("output.txt", std::ios::out);

    srand((unsigned int)time(NULL));

    Mode mode = MODE_UNDEFINED;
    const char* adjacencyFile = NULL;
    std::string start, goal;
    const char* svgFile = NULL;
    //AdjacencyFileFormat fmt = UNDEFINED;

    if (argc == 1)
    {
        logfile << "Usage: ?\n";
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
                    logfile << "Invalid mode \"" << argv[i] << "\"\n";
                    return 1;
                }
            }
            else
            {
                logfile << "Error: Expected mode after -m flag\n";
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
                logfile << "Error: Expected filename\n";
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
                    logfile << "Invalid format \"" << argv[i] << "\"\n";
                    return 1;
                }
            }
            else
            {
                logfile << "Error: Expected file format specifier\n";
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
                logfile << "Error: Expected value after -s flag\n";
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
                logfile << "Error: Expected value after -g flag\n";
                return 1;
            }
        }
        else if (strcmp(argv[i], "-w") == 0)
        {
            showWarnings = true;
        }
        else
        {
            logfile << "Unrecognized argument \"" << argv[i] << "\"\n";
            return 1;
        }
    }

    if (mode == MODE_UNDEFINED)
    {
        logfile << "Error: Mode not specified\n";
        return 1;
    }

    /*
    if (mode == MODE_GAME || mode == MODE_BFS)
    {
        if (adjacencyFile == NULL)
        {
            logfile << "Error: No adjacency file specified\n";
            return 1;
        }
        else if (fmt == UNDEFINED)
        {
            logfile << "Error: Adjacency file format not specified\n";
            return 1;
        }
    }
    */

    if (mode == MODE_GAME)
    {
        //Game(adjacencyFile, fmt, start, goal);
        logfile << "Temporarily disabled\n";
    }
    else if (mode == MODE_BFS)
    {
        USCountiesAdjacencyListFile adjacencyList;
        USCountiesSvgFile svg;
        Graph<int> graph(adjacencyList.getAdjacencyList());
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
        USCountiesAdjacencyListFile adjacencyListFile;
        USCountiesSvgFile svg;
        Graph<int> graph(adjacencyListFile.getAdjacencyList());
        const USCountiesAdjacencyListFile::CountyFipsMap& counties = adjacencyListFile.getCounties();
        std::vector<std::pair<int,std::string> > washingtons;
        USCountiesAdjacencyListFile::CountyFipsMap::const_iterator it;
        for (it=counties.begin(); it!=counties.end(); ++it)
        {
            if (it->second.find("Washington") != std::string::npos)
            {
                washingtons.push_back(*it);
            }
        }
        logfile << "found " << washingtons.size() << " Washingtons\n";
        std::vector<std::pair<int,std::string> >::const_iterator jt;
        for (jt=washingtons.begin(); jt!=washingtons.end(); ++jt)
        {
            if (jt!=washingtons.begin())
            {
                std::vector<std::pair<int,std::string> >::const_iterator prev = jt; --prev; 
                performBfsAndUpdateSvg(adjacencyListFile, svg, graph, prev->first, jt->first);
            }
        }
        svg.saveFile("output.svg");
    }
    else if (mode == MODE_TEST)
    {
        logfile << "Test stub\n";
    }
    return 0;
}

void performBfsAndUpdateSvg(USCountiesAdjacencyListFile& adjacencyListFile,
                            USCountiesSvgFile& svg,
                            Graph<int>& graph,
                            int startFips, 
                            int goalFips)
{
    std::string start = adjacencyListFile.getCountyNameByFipsCode(startFips);
    std::string goal = adjacencyListFile.getCountyNameByFipsCode(goalFips);
    logfile << "Performing BFS from " << start << " to " << goal << "...\n";
    logfile << "Start: " << start << " (" << USCountiesAdjacencyListFile::fipsToString(startFips) << ")\n";
    logfile << "Goal: " << goal << " (" << USCountiesAdjacencyListFile::fipsToString(goalFips) << ")\n";
    std::vector<int> path = graph.breadthFirstSearch(startFips,goalFips);
    logfile << "Optimal Path:\n";
    std::vector<int>::const_iterator it;
    for (it=path.begin(); it!=path.end(); ++it)
    {
        std::string countyName = adjacencyListFile.getCountyNameByFipsCode(*it);
        logfile << countyName << " (" << *it << "), ";
        if (countyName.find("Washington") != std::string::npos)
        {
            svg.markCountyByFips(*it, "red");
        }
        else
        {
            svg.markCountyByFips(*it, "gray");
        }
    }
    logfile << "\n(" << path.size() << " moves)\n"; 
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