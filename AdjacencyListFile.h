#ifndef ADAJACENCY_LIST_H
#define ADAJACENCY_LIST_H

/**
 * Interface
 */

template <typename T>
class AdjacencyListFile
{
public:
    typedef std::pair<T, std::vector<T> > AdjacencyListEntry;
    typedef std::vector<AdjacencyListEntry > AdjacencyList;
    virtual const AdjacencyList& getAdjacencyList() = 0;
};

#endif