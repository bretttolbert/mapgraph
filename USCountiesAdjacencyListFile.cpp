#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "USCountiesAdjacencyListFile.h"
#include "StringUtils.h"

namespace GraphGame
{
    extern bool showWarnings;

    USCountiesAdjacencyListFile::USCountiesAdjacencyListFile()
    {
        std::cout << "Loading adjacency list file...\n";
        std::ifstream file;
        file.open(USCOUNTIES_ADJACENCY_LIST_FILENAME, std::ios::in);
        if (file.is_open())
        {
            int lineNum = 0;
            AdjacencyListEntry entry;
            entry.first = 0;
            while (file.good())
            {
                std::string line;
                getline(file,line);
                ++lineNum;
                if (!line.empty())
                {
                    std::vector<std::string> tokens = StringUtils::split(line, '\t');
                    if (tokens.size() == 4)
                    {
                        std::string neighborName = tokens[2];
                        int neighborFips = atoi(tokens[3].c_str());

                        if (tokens[0] != "")
                        {
                            if (entry.first != 0)
                            {
                                adjacencyList.insert(entry);
                                entry.second.clear();
                            }
                            std::string countyName = tokens[0];
                            int countyFips = atoi(tokens[1].c_str());
                            entry.first = countyFips;
                            if (neighborFips != entry.first)
                            {
                                entry.second.insert(neighborFips);
                            }
                            nodeIdToNodeStringMap[countyFips] = countyName;
                        }
                        else
                        {
                            if (neighborFips != entry.first)
                            {
                                entry.second.insert(neighborFips);
                            }
                        }
                    }
                    else
                    {
                        std::cout << "Parser Error on line " << lineNum << ".\n";
                        std::cout << "Expected 4 columns but found " << tokens.size() << std::endl;
                        exit(1);
                    }
                }
            }
            //insert last entry
            if (entry.first != 0)
            {
                adjacencyList.insert(entry);
            }
        }
    }

    std::string USCountiesAdjacencyListFile::fipsToString(int fips)
    {
        //fips code is 5 digits left padded with '0'
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(5) << fips;
        return oss.str();
    }

    void USCountiesAdjacencyListFile::getCountiesInState(const std::string& state, 
                                                         std::set<int>& nodeIdToNodeStringMapInState) const
    {
        NodeIdToNodeStringMap::const_iterator it;
        for (it=nodeIdToNodeStringMap.begin(); it!=nodeIdToNodeStringMap.end(); ++it)
        {
            int countyFips = it->first;
            std::string countyName = it->second;
            //we expect county name to contain state abbreviation, e.g. "Madison County, AL"
            size_t commaPos = countyName.find(',');
            assert(commaPos != std::string::npos);
            size_t statePos = countyName.find(state, commaPos+1);
            if (statePos != std::string::npos)
            {
                nodeIdToNodeStringMapInState.insert(countyFips);
            }
        }
    }

    int USCountiesAdjacencyListFile::getRandomCountyFromState(const std::string& state) const
    {
        std::set<int> nodeIdToNodeStringMapInState;
        getCountiesInState(state, nodeIdToNodeStringMapInState);
        std::set<int>::const_iterator it = nodeIdToNodeStringMapInState.begin();
        std::advance(it, rand() % nodeIdToNodeStringMapInState.size());
        return *it;
    }

    int USCountiesAdjacencyListFile::getRandomCountyFromStates(const std::set<std::string>& states) const
    {
        std::set<int> candidates;
        std::set<std::string>::const_iterator it;
        for (it=states.begin(); it!=states.end(); ++it)
        {
            getCountiesInState(*it, candidates);
        }
        std::set<int>::const_iterator jt;
        std::advance(jt, rand() % candidates.size());
        return *jt;
    }
}
