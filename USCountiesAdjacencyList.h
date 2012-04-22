#ifndef USCOUNTIES_ADAJACENCY_LIST_H
#define USCOUNTIES_ADAJACENCY_LIST_H

#include <vector>
#include <map>
#include <string>

#include "AdjacencyList.h"

#define USCOUNTIES_ADJACENCY_LIST_FILENAME "US_county_adjacency_2010.txt"

struct USCountiesNodeValue
{
    std::string name;
    int fips;
};

class USCountiesAdjacencyList : public AdjacencyList
{
public:
    USCountiesAdjacencyList();
    std::string getCountyNameByFipsCode(int fipsCode);
    std::string getCountyNameByFipsCode(const std::string& fipsCode);
    int getRandomCountyFips();
    static std::string fipsToString(int fips);
private:
    typedef std::map<int,std::string> CountyFipsMap;
    CountyFipsMap counties; //key: county fips code, value: county name
    //to save memory, the adjacency list contains only fips codes
    typedef std::pair<int, std::vector<int> > AdjacencyListEntry;
    typedef std::vector< AdjacencyListEntry > AdjacencyList;
    AdjacencyList adjacencyList;
};

#endif