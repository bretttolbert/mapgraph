#include <fstream>
#include <iostream>
#include <algorithm>

#include "USCountiesAdjacencyList.h"
#include "StringUtils.h"

USCountiesAdjacencyList::USCountiesAdjacencyList()
{
    std::cout << "Loading adjacency list file...\n";
    std::ifstream file;
    file.open(USCOUNTIES_ADJACENCY_LIST_FILENAME, std::ios::in);
    if (file.is_open())
    {
        int lineNum = 0;
        std::pair< int, std::vector<int> > entry;
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
                            adjacencyList.push_back(entry);
                            entry.second.clear();
                        }
                        std::string countyName = tokens[0];
                        int countyFips = atoi(tokens[1].c_str());
                        entry.first = countyFips;
                        if (neighborFips != entry.first)
                        {
                            entry.second.push_back(neighborFips);
                        }
                        counties[countyFips] = countyName;
                    }
                    else
                    {
                        if (neighborFips != entry.first)
                        {
                            entry.second.push_back(neighborFips);
                        }
                    }
                }
                else
                {
                    std::cerr << "Parser Error on line " << lineNum << ".\n";
                    std::cerr << "Expected 4 columns but found " << tokens.size() << std::endl;
                    exit(1);
                }
            }
        }
    }
}

std::string USCountiesAdjacencyList::getCountyNameByFipsCode(int fipsCode)
{
    return counties[fipsCode];
}

std::string USCountiesAdjacencyList::getCountyNameByFipsCode(const std::string& fipsCode)
{
    return counties[atoi(fipsCode.c_str())];
}

int USCountiesAdjacencyList::getRandomCountyFips()
{
    CountyFipsMap::const_iterator it = counties.begin();
    std::advance(it, rand() % counties.size());
    return it->first;
}

std::string USCountiesAdjacencyList::fipsToString(int fips)
{
    //fips code is 5 digits left padded with '0'
    char buf[33];
    _itoa_s(fips, buf, sizeof(buf), 10);
    if (buf == NULL)
    {
        std::cerr << "fipsToString: buf is NULL\n";
        exit(1);
    }
    std::string str(buf);
    while (str.size() < 5)
    {
        str.insert('0', 0);
    }
    return str;
}