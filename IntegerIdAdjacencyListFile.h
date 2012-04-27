#ifndef INTEGER_ID_ADJACENCY_LIST_FILE_H
#define INTEGER_ID_ADJACENCY_LIST_FILE_H

#include <map>
#include <string>

#include "AdjacencyListFile.h"

namespace GraphGame
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
         */
        virtual int stringToNodeId(const std::string& str) const;   

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
