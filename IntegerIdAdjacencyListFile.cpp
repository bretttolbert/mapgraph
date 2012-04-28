#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

#include "IntegerIdAdjacencyListFile.h"
#include "StringUtils.h"

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
            std::cerr << "Error: invalid nodeId\n";
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
            std::cerr << "Error: Invalid nodeId \"" << nodeId << "\"\n";
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

    int IntegerIdAdjacencyListFile::nodeSearch(const std::string& query, std::string& result)
    {
        //search nodes for anything resembling node1
        //first split node1 into tokens using space as the delimeter
        std::vector<std::string> tokens = StringUtils::split(query, ' ');
        //refine the list of tokens by converting each to lowercase and skipping
        //tokens containing 'county'
        std::vector<std::string> goodTokens;
        std::vector<std::string>::const_iterator it;
        for (it=tokens.begin(); it!=tokens.end(); ++it)
        {
            std::string token = *it;
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            if (token.find("county") == std::string::npos)
            {
                goodTokens.push_back(token);
            }
        }
        tokens = goodTokens;
        int highestScoreSoFar = 0;
        int highestScoreNodeId = 0;
        
        NodeIdToNodeStringMap::const_iterator node_it;
        for (node_it=nodeIdToNodeStringMap.begin(); 
             node_it!=nodeIdToNodeStringMap.end(); ++node_it)
        {
            std::string node = node_it->second;
            int nodeScore = 0;
            std::string lnode(node);
            std::transform(lnode.begin(), lnode.end(), lnode.begin(), ::tolower);
            //check if nodeStr contains any of the tokens
            std::vector<std::string>::const_iterator token_it;
            for (token_it=tokens.begin(); token_it!=tokens.end(); ++token_it)
            {
                std::string token = *token_it;
                if (lnode.find(token) != std::string::npos)
                {
                    nodeScore++;
                }
            }
            if (nodeScore > highestScoreSoFar)
            {
                //std::cout << "New highest score: " << node << " (" << nodeScore << ")\n"; 
                highestScoreSoFar = nodeScore;
                highestScoreNodeId = node_it->first;
            }
        }
        if (highestScoreSoFar > 0)
        {
            result = nodeIdToNodeStringMap[highestScoreNodeId];
            return highestScoreNodeId;
        }
        else
        {
            return 0;
        }
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
