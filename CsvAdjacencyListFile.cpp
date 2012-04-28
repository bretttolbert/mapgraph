#include <iostream>
#include <fstream>
#include <vector>

#include "CsvAdjacencyListFile.h"
#include "StringUtils.h"

namespace GraphGame
{
    CsvAdjacencyListFile::CsvAdjacencyListFile(const std::string& filename)
    {
        std::vector<std::vector<std::string> > lines;
        std::cout << "Loading adjacency list file...\n";
        std::ifstream file;
        //iterate over the file once to populate node id to node string map
        file.open(filename.c_str(), std::ios::in);
        if (file.is_open())
        {
            int lineNum = 0;
            while (file.good())
            {
                std::string line;
                getline(file,line);
                if (!line.empty())
                {
                    ++lineNum;
                    std::vector<std::string> tokens = StringUtils::split(line, ',');
                    nodeIdToNodeStringMap[lineNum] = tokens[0];
                    lines.push_back(tokens);
                }
            }
        }
        else
        {
            std::cerr << "Error: Failed to open file \"" << filename << "\"\n";
        }
        IntegerIdAdjacencyListFile::NodeIdToNodeStringMap::const_iterator it;
        for (it=nodeIdToNodeStringMap.begin(); it!=nodeIdToNodeStringMap.end(); ++it)
        {
            std::cout << it->first << ": " << it->second << std::endl;
        }
        file.close();

        //now iterate over the lines to populate adjacency list with node IDs
        std::vector<std::vector<std::string> >::const_iterator line_it;
        for (line_it=lines.begin(); line_it!=lines.end(); ++line_it)
        {
            const std::vector<std::string>& tokens = *line_it;
            std::string nodeStr = tokens[0];
            int nodeId = stringToNodeId(nodeStr);
            AdjacencyListEntry entry;
            entry.first = nodeId;
            std::vector<std::string>::const_iterator token_it = tokens.begin();
            std::advance(token_it, 1);
            for (; token_it!=tokens.end(); ++token_it)
            {
                std::string neighborNodeStr = *token_it;
                int neighborNodeId = stringToNodeId(neighborNodeStr);
                entry.second.insert(neighborNodeId);
            }
            adjacencyList.insert(entry);
        }
    }
}
