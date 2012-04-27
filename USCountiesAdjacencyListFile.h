#ifndef USCOUNTIES_ADAJACENCY_LIST_FILE_H
#define USCOUNTIES_ADAJACENCY_LIST_FILE_H

#include <vector>
#include <map>
#include <string>

#include "IntegerIdAdjacencyListFile.h"

namespace GraphGame
{
    #define USCOUNTIES_ADJACENCY_LIST_FILENAME "US_county_adjacency_2010.txt"

    class USCountiesAdjacencyListFile : public IntegerIdAdjacencyListFile
    {
    public:
        USCountiesAdjacencyListFile();

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

    };
}
#endif
