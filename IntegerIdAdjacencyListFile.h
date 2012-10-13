#ifndef INTEGER_ID_ADJACENCY_LIST_FILE_H
#define INTEGER_ID_ADJACENCY_LIST_FILE_H

#include <map>
#include <string>

#include "AdjacencyListFile.h"

namespace MapGraph
{
    /**
     * Type of adjacency list file which uses integer IDs to respresent each node
     * and has methods for converting between string representation of a node
     * and it's integer ID. For example, if the adjacency list file contains
     * counties along with county FIPS codes, the string representation of a node
     * might by "Madison County, AL" and the node ID might be 1089 (the FIPS code
     * for Madison County, AL).
     */
    class IntegerIdAdjacencyListFile : public AdjacencyListFile<int>
    {
    public:
        typedef std::map<int,std::string> NodeIdToNodeStringMap;
        /** 
         * Returns a string representation of the given node.
         * For example, if this is an adjacency list of US County FIPS codes,
         * this method would return the county name for a given FIPS code.
         */
        virtual std::string nodeIdToString(const int nodeId) const;

        /**
         * Returns the node ID for given the string representation of the node.
         * For example, if this is an ajdacency list of US County FIPS codes,
         * this method would return the FIPS code for a given county name.
         * Returns 0 if no exact match is found.
         */
        virtual int stringToNodeId(const std::string& str) const;   

        /**
         * Searches for a node that best matches the given query string.
         * This is done by splitting the query string into tokens and scoring
         * each candidate based on the number of tokens matched.
         * Returns 0 if no matching results are found.
         * If multiple nodes are tied for the highest score, it returns the 
         * first one found (the node with the lowest numerical ID).
         */
        virtual int nodeSearch(const std::string& query, std::string& result);

        virtual const NodeIdToNodeStringMap& getNodeIdToNodeStringMap() const;

        /**
         * implementation of AdjacencyListFile<int> interface
         */
        virtual const AdjacencyListFile<int>::AdjacencyList& getAdjacencyList() const;
        virtual const std::set<int>& getNeighbors(const int nodeId) const;
        virtual int getRandomNodeId() const;
    protected:
        NodeIdToNodeStringMap nodeIdToNodeStringMap;
        AdjacencyList adjacencyList;
    };
}

#endif
