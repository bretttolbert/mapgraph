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

        /**
         * Converts fips to numeric string, left padded with zeros
         */
        static std::string fipsToString(int fips); 

        /**
         * Inserts all counties in the specified state into the supplied set
         */
        void getCountiesInState(const std::string& state, std::set<int>& countiesInState) const;

        /**
         * Returns a random county from the specified state
         */
        int getRandomCountyFromState(const std::string& state) const;

        /**
         * Returns a random county from the specified states
         */ 
        int getRandomCountyFromStates(const std::set<std::string>& states) const;
    private:
        CountyFipsMap counties; //key: county fips code, value: county name
        //to save memory, the adjacency list contains only fips codes
        AdjacencyList adjacencyList;
    };
}
#endif
