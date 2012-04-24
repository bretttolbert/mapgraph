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
        virtual const std::set<int>& getNeighbors(int nodeId) const = 0;
    };

    template <typename T>
    inline
    bool AdjacencyListFile<T>::AdjacencyListEntryComparator::operator() (const AdjacencyListEntry a, const AdjacencyListEntry b)
    {
        return a.first < b.first;
    }
}

#endif