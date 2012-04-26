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
        virtual const AdjacencyListFile<int>::AdjacencyList& getAdjacencyList() const;
        virtual const std::set<int>& getNeighbors(const int nodeId) const;
        virtual std::string nodeIdToString(const int fipsCode) const;
        virtual int stringToNodeId(const std::string& countyName) const;
        virtual int getRandomNodeId() const;
        const CountyFipsMap& getCounties();
        static std::string fipsToString(int fips); //converts fips to numeric string, left padded with zeros
    private:
        CountyFipsMap counties; //key: county fips code, value: county name
        //to save memory, the adjacency list contains only fips codes
        AdjacencyList adjacencyList;
    };
}
#endif
