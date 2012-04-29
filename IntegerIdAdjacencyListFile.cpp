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
        //first split node1 into tokens (words) using space as the delimeter
        std::string lquery(query);
        std::transform(lquery.begin(), lquery.end(), lquery.begin(), ::tolower);
        std::vector<std::string> queryTokens = StringUtils::split(lquery, ' ');
        //filter out tokens containing 'county'
        std::vector<std::string> goodTokens;
        std::vector<std::string>::const_iterator it;
        for (it=queryTokens.begin(); it!=queryTokens.end(); ++it)
        {
            std::string token = *it;
            if (token.find("county") == std::string::npos)
            {
                while (token[token.size()-1] == ',')
                {
                    token.erase(token.size()-1);
                }
                goodTokens.push_back(token);
            }
        }
        queryTokens = goodTokens;
        int highestScoreSoFar = 0;
        int highestScoreNodeId = 0;
        
        NodeIdToNodeStringMap::const_iterator node_it;
        for (node_it=nodeIdToNodeStringMap.begin(); 
             node_it!=nodeIdToNodeStringMap.end(); ++node_it)
        {
            std::string lnode = node_it->second;
            std::transform(lnode.begin(), lnode.end(), lnode.begin(), ::tolower);
            //check for exact match (overrides scoring)
            if (lnode == lquery)
            {
                result = node_it->second;
                return node_it->first;
            }
            int nodeScore = 0;
            //split current node into tokens
            std::vector<std::string> nodeTokens = StringUtils::split(lnode, ' ');
            std::vector<std::string>::const_iterator node_token_it;
            for (node_token_it=nodeTokens.begin();
                 node_token_it!=nodeTokens.end();
                 ++node_token_it)
            {
                std::string nodeToken = *node_token_it;
                std::vector<std::string>::const_iterator query_it;
                for (query_it=queryTokens.begin(); 
                     query_it!=queryTokens.end(); ++query_it)
                {
                    std::string queryToken = *query_it;
                    //check for exact token match
                    if (nodeToken == queryToken)
                    {
                        nodeScore += queryToken.size() * 50;
                    }
                    else if (nodeToken.find(queryToken) != std::string::npos)
                    {
                        int percentMatch = (int)floor(((queryToken.size() / (float)nodeToken.size()) * 100) + 0.5);
                        //std::cout << "queryToken=\"" << queryToken << "\" "
                        //          << "nodeToken=\"" << nodeToken << "\" "
                        //          << "percentMatch=" << percentMatch << std::endl;
                        nodeScore += percentMatch * queryToken.size();
                    }
                }
            }
            if (nodeScore > highestScoreSoFar)
            {
                //std::cout << "New highest score: " << node_it->second << " (" << nodeScore << ")\n"; 
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
