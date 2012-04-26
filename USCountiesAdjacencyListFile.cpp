#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>

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
                            counties[countyFips] = countyName;
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
        }
    }

    const AdjacencyListFile<int>::AdjacencyList& USCountiesAdjacencyListFile::getAdjacencyList() const
    {
        return adjacencyList;
    }

    const std::set<int>& USCountiesAdjacencyListFile::getNeighbors(const int nodeId) const
    {
        AdjacencyListEntry temp;
        temp.first = nodeId;
        AdjacencyList::const_iterator it = adjacencyList.find(temp);
        if (it == adjacencyList.end())
        {
            std::cout << "Error: invalid nodeId\n";
            exit(1);
        }
        return it->second;
    }

    std::string USCountiesAdjacencyListFile::nodeIdToString(const int fipsCode) const
    {
        CountyFipsMap::const_iterator it = counties.find(fipsCode);
        if (it != counties.end())
        {
            return it->second;
        }
        else
        {
            std::cout << "Error: Invalid fipsCode \"" << fipsCode << "\"\n";
            return "";
        }
    }

    int USCountiesAdjacencyListFile::stringToNodeId(const std::string& countyName) const
    {
        int result = 0;
        CountyFipsMap::const_iterator it;
        for (it=counties.begin(); it!=counties.end(); ++it)
        {
            if (it->second == countyName)
            {
                result = it->first;
                break;
            }
        }
        return result;
    }

    int USCountiesAdjacencyListFile::getRandomNodeId() const
    {
        CountyFipsMap::const_iterator it = counties.begin();
        std::advance(it, rand() % counties.size());
        return it->first;
    }

    const USCountiesAdjacencyListFile::CountyFipsMap& USCountiesAdjacencyListFile::getCounties()
    {
        return counties;
    }

    std::string USCountiesAdjacencyListFile::fipsToString(int fips)
    {
        //fips code is 5 digits left padded with '0'
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(5) << fips;
        return oss.str();
    }
}
