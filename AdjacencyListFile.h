#ifndef ADAJACENCY_LIST_FILE_H
#define ADAJACENCY_LIST_FILE_H

#include <set>

namespace GraphGame
{
    template <typename T>
    class AdjacencyListFile
    {
    public:
        typedef std::pair<T, std::set<T> > AdjacencyListEntry;
        struct AdjacencyListEntryComparator
        {
            virtual bool operator() (const AdjacencyListEntry a, const AdjacencyListEntry b);
        };
        typedef std::set<AdjacencyListEntry, AdjacencyListEntryComparator > AdjacencyList;
        virtual const AdjacencyList& getAdjacencyList() const = 0;
        virtual const std::set<T>& getNeighbors(const T nodeId) const = 0;

        /** 
         * Returns a string representation of the given node.
         * For example, if this is an adjacency list of US County FIPS codes,
         * this method would return the county name for a given FIPS code.
         * If the adjacency list actually contains strings, this method might
         * simply return whatever is passed into it.
         */
        virtual std::string nodeIdToString(const T nodeId) const = 0;

        /**
         * Returns the node ID for given the string representation of the node.
         * For example, if this is an ajdacency list of US County FIPS codes,
         * this method would return the FIPS code for a given county name.
         * If the adjacency list actually contains strings, this method might
         * simply return whatever is passed into it.
         */
        virtual T stringToNodeId(const std::string& str) const = 0;

        virtual T getRandomNodeId() const = 0;
    };

    template <typename T>
    inline
    bool AdjacencyListFile<T>::AdjacencyListEntryComparator::operator() (const AdjacencyListEntry a, const AdjacencyListEntry b)
    {
        return a.first < b.first;
    }
}

#endif
