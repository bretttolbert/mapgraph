#include <fstream>
#include "TravelingSalesmanProblem.h"

namespace GraphGame
{
    extern bool showWarnings;

    TravelingSalesmanProblem::TravelingSalesmanProblem()
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
        std::cout << "found " << washingtons.size() << " Washingtons\n";
        std::vector<std::pair<int,std::string> >::const_iterator jt;
        for (jt=washingtons.begin(); jt!=washingtons.end(); ++jt)
        {
            std::vector<std::pair<int,std::string> >::const_iterator kt;
            for (kt = washingtons.begin(); kt!=washingtons.end(); ++kt)
            {
                if (jt != kt)
                {
                    performBfsAndUpdateSvg(adjacencyListFile, svg, graph, jt->first, kt->first);
                }
            }
        }
        svg.saveFile("output.svg");
    }

    void TravelingSalesmanProblem::performBfsAndUpdateSvg(USCountiesAdjacencyListFile& adjacencyListFile,
                                USCountiesSvgFile& svg,
                                Graph<int>& graph,
                                int startFips, 
                                int goalFips)
    {
        std::string start = adjacencyListFile.nodeIdToString(startFips);
        std::string goal = adjacencyListFile.nodeIdToString(goalFips);
        std::cout << "Performing BFS from " << start << " to " << goal << "...\n";
        std::cout << "Start: " << start << " (" << USCountiesAdjacencyListFile::fipsToString(startFips) << ")\n";
        std::cout << "Goal: " << goal << " (" << USCountiesAdjacencyListFile::fipsToString(goalFips) << ")\n";
        std::vector<int> path = graph.breadthFirstSearch(startFips,goalFips);
        std::cout << "Optimal Path:\n";
        std::vector<int>::const_iterator it;
        for (it=path.begin(); it!=path.end(); ++it)
        {
            std::string countyName = adjacencyListFile.nodeIdToString(*it);
            std::cout << countyName << " (" << *it << "), ";
            if (countyName.find("Washington") != std::string::npos)
            {
                svg.markCountyByFips(*it, "red");
            }
            else
            {
                svg.markCountyByFips(*it, "gray");
            }
        }
        std::cout << "\n(" << path.size() << " moves)\n"; 
    }
}
