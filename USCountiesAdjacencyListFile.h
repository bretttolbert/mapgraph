#ifndef USCOUNTIES_ADAJACENCY_LIST_FILE_H
#define USCOUNTIES_ADAJACENCY_LIST_FILE_H

#include <vector>
#include <map>
#include <string>

#include "AdjacencyListFile.h"

namespace GraphGame
{
    #define USCOUNTIES_ADJACENCY_LIST_FILENAME "US_county_adjacency_2010.txt"

    struct USCountiesNodeValue
    {
        std::string name;
        int fips;
    };

    class USCountiesAdjacencyListFile : public AdjacencyListFile<int>
    {
    public:
        typedef std::map<int,std::string> CountyFipsMap;
        USCountiesAdjacencyListFile();
        const AdjacencyListFile<int>::AdjacencyList& getAdjacencyList();
        const CountyFipsMap& getCounties();
        std::string getCountyNameByFipsCode(int fipsCode);
        std::string getCountyNameByFipsCode(const std::string& fipsCode);
        int getFipsCodeByCountyName(const std::string& countyName);
        int getRandomFipsCode();
        static std::string fipsToString(int fips);
    private:
        struct AdjacencyListEntryComparator
        {
            bool operator() (const AdjacencyListEntry a, const AdjacencyListEntry b);
        };
        CountyFipsMap counties; //key: county fips code, value: county name
        //to save memory, the adjacency list contains only fips codes
        AdjacencyList adjacencyList;
    };
}
#endif