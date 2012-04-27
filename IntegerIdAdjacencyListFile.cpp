#include <iostream>
#include <algorithm>
#include <cmath>

#include "IntegerIdAdjacencyListFile.h"

namespace GraphGame
{
    const AdjacencyListFile<int>::AdjacencyList& IntegerIdAdjacencyListFile::getAdjacencyList() const
    {
        return adjacencyList;
    }

    const std::set<int>& IntegerIdAdjacencyListFile::getNeighbors(const int nodeId) const
    {
        AdjacencyListEntry temp;
        temp.first = nodeId;
        AdjacencyList::const_iterator it = adjacencyList.find(temp);
        if (it == adjacencyList.end())
        {
            std::cout << "Error: invalid nodeId\n";
            exit(1);
        }
        return it->second;
    }

    std::string IntegerIdAdjacencyListFile::nodeIdToString(const int nodeId) const
    {
        NodeIdToNodeStringMap::const_iterator it = nodeIdToNodeStringMap.find(nodeId);
        if (it != nodeIdToNodeStringMap.end())
        {
            return it->second;
        }
        else
        {
            std::cout << "Error: Invalid nodeId \"" << nodeId << "\"\n";
            return "";
        }
    }

    int IntegerIdAdjacencyListFile::stringToNodeId(const std::string& countyName) const
    {
        int result = 0;
        NodeIdToNodeStringMap::const_iterator it;
        for (it=nodeIdToNodeStringMap.begin(); it!=nodeIdToNodeStringMap.end(); ++it)
        {
            if (it->second == countyName)
            {
                result = it->first;
                break;
            }
        }
        return result;
    }

    const IntegerIdAdjacencyListFile::NodeIdToNodeStringMap& IntegerIdAdjacencyListFile::getNodeIdToNodeStringMap() const
    {
        return nodeIdToNodeStringMap;
    }

    int IntegerIdAdjacencyListFile::getRandomNodeId() const
    {
        NodeIdToNodeStringMap::const_iterator it = nodeIdToNodeStringMap.begin();
        std::advance(it, rand() % nodeIdToNodeStringMap.size());
        return it->first;
    }
}
