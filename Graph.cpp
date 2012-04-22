#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <cstdlib>

#include "Graph.h"
#include "StringUtils.h"

extern bool showWarnings;

Graph::Graph()
{
}

Graph::~Graph()
{
    NodeSet::iterator it;
    for (it=nodes.begin(); it!=nodes.end(); ++it)
    {
        Node* node = *it;
        delete node;
    }
}

void Graph::readAdjacencyListFromFile(const char* filename, AdjacencyFileFormat fmt)
{
    std::cout << "Loading file...\n";
    adjacencyList.clear();
    std::ifstream file;
    file.open(filename, std::ios::in);
    if (file.is_open())
    {
        if (fmt == SINGLE_LINE_CSV)
        {
            while (file.good())
            {
                std::string line; 
                getline(file,line);
                std::vector<std::string> tokens = StringUtils::split(line, ',');
                //create adjacency list entry
                std::pair< std::string, std::vector<std::string> > entry;
                entry.first = tokens[0];
                tokens.erase(tokens.begin());
                entry.second = tokens;
                adjacencyList.push_back(entry);
            }
        }
        else if (fmt == MULTI_LINE_TAB_DELIMITED_US_CENSUS)
        {
            int lineNum = 0;
            std::pair< std::string, std::vector<std::string> > entry;
            while (file.good())
            {
                std::string line;
                getline(file,line);
                ++lineNum;
                if (!line.empty())
                {
                    std::vector<std::string> tokens = StringUtils::split(line, '\t');
                    if (tokens.size() == 4)
                    {
                        if (tokens[0] != "")
                        {
                            if (!entry.first.empty())
                            {
                                adjacencyList.push_back(entry);
                                entry.second.clear();
                            }
                            entry.first = tokens[0];
                            if (tokens[2] != entry.first)
                            {
                                entry.second.push_back(tokens[2]);
                            }
                        }
                        else
                        {
                            if (tokens[2] != entry.first)
                            {
                                entry.second.push_back(tokens[2]);
                            }
                        }
                    }
                    else
                    {
                        std::cerr << "Parser Error on line " << lineNum << ".\n";
                        std::cerr << "Expected 4 columns but found " << tokens.size() << std::endl;
                        exit(1);
                    }
                }
            }
        }
        file.close();
        std::cout << "Done. " << adjacencyList.size() << " entries loaded from file.\n";
        std::cout << "Generating nodes...\n";

        //create a node for each adjacency list entry
        AdjacencyList::const_iterator it;
        for (it=adjacencyList.begin(); it!=adjacencyList.end(); ++it)
        {
            //create node
            Node* node = new Node();
            node->value = it->first;
            nodes.insert(node);
        }
        //populate adjacentNodes of each node
        for (it=adjacencyList.begin(); it!=adjacencyList.end(); ++it)
        {
            Node* node = findNodeByValue(it->first);
            if (node == NULL)
            {
                std::cerr << "Error: node is NULL - findNodeByValue returned NULL for " << it->first << "\n";
                exit(1);
            }
            std::vector<std::string>::const_iterator jt;
            for (jt=it->second.begin(); jt!=it->second.end(); ++jt)
            {
                Node* adjacentNode = findNodeByValue(*jt);
                if (adjacentNode == NULL && showWarnings)
                {
                    std::cerr << "Warning: No entry was found for \"" << *jt << "\" although it was specified as a neighbor of \"" << it->first << "\".\n";
                }
                else
                {
                    node->adjacentNodes.push_back(adjacentNode);
                }
            }
        }
        std::cout << "Done.\n";
    }
    else
    {
        std::cout << "Error opening file.\n";
    }
}

void Graph::printAdjacencyList()
{
    AdjacencyList::const_iterator it;
    for (it=adjacencyList.begin(); it!=adjacencyList.end(); ++it)
    {
        std::string node = it->first;
        std::vector<std::string> neighbors = it->second;
        std::cout << node << " : ";
        std::ostream_iterator<std::string> output(std::cout, " ");
        std::copy(neighbors.begin(), neighbors.end(), output);
        std::cout << std::endl;
    }
}

bool Graph::isNodeValid(const std::string& node)
{
    bool result = false;
    AdjacencyList::const_iterator it;
    for (it=adjacencyList.begin(); it!=adjacencyList.end(); ++it)
    {
        if (node == it->first)
        {
            result = true;
            break;
        }
    }
    return result;
}

Graph::Node* Graph::findNodeByValue(const std::string& value)
{
    Node temp;
    temp.value = value;
    NodeSet::const_iterator it = nodes.find(&temp);
    if (it != nodes.end())
    {
        return *it;
    }
    else
    {
        return NULL;
    }
}

std::vector<std::string> Graph::getNeighbors(const std::string& node)
{
    std::vector<std::string> result;
    AdjacencyList::const_iterator it;
    for (it=adjacencyList.begin(); it!=adjacencyList.end(); ++it)
    {
        if (node == it->first)
        {
            result.resize(it->second.size());
            std::copy(it->second.begin(), it->second.end(), result.begin());
            break;
        }
    }
    return result;
}

void Graph::printNeighbors(const std::string& node, const char* delim)
{
    std::vector<std::string> neighbors = getNeighbors(node);
    //std::cout << "Neighbors of " << node << ": ";
    std::ostream_iterator<std::string> output(std::cout, delim);
    std::copy(neighbors.begin(), neighbors.end(), output);
    //std::cout << std::endl;
}

std::vector<std::string> Graph::getNodeValues()
{
    std::vector<std::string> result;
    result.reserve(adjacencyList.size());
    AdjacencyList::const_iterator it;
    for (it=adjacencyList.begin(); it!=adjacencyList.end(); ++it)
    {
        result.push_back(it->first);
    }
    return result;
}

Graph::Node::Node() : 
    color(BLACK), 
    parent(NULL) 
{ }

bool Graph::NodeComparator::operator() (const Node* a,  const Node* b)
{
    return (a->value < b->value);
}

std::vector<std::string> Graph::breadthFirstSearch(std::string startNodeValue, std::string goalNodeValue)
{
    std::vector<std::string> path;
    Node* startNode = findNodeByValue(startNodeValue);
    std::queue<Node*> q;
    startNode->color = Node::WHITE; //white=marked, black=unmarked (default)
    q.push(startNode);
    while (q.size() > 0)
    {
        Node* currentNode = q.front(); 
        q.pop();
        if (DEBUG_GRAPH)
        {
            std::cout << "examining " << currentNode->value << std::endl;
        }
        if (currentNode->value == goalNodeValue)
        {
            //populate path vector
            Node *node = currentNode;
            while (node != NULL)
            {
                path.push_back(node->value);
                node = node->parent;
            }
            std::reverse(path.begin(),path.end());
            if (DEBUG_GRAPH)
            {
                std::cout << "found it\npath taken: ";
                std::ostream_iterator<std::string> output(std::cout, " ");
                std::copy(path.begin(), path.end(), output);
                std::cout << std::endl;
            }
            break;
        }
        else
        {
            std::vector<Node*>::const_iterator it;
            for (it=currentNode->adjacentNodes.begin(); it!=currentNode->adjacentNodes.end(); ++it)
            {
                Node* adjacentNode = *it;
                if (adjacentNode == NULL)
                {
                    std::cerr << "Error: adjacentNode is NULL\n";
                    std::cerr << "currentNode = " << currentNode->value << std::endl;
                    exit(1);
                }
                if (adjacentNode->color != Node::WHITE)
                {
                    if (DEBUG_GRAPH)
                    {
                        std::cout << "enqueuing neighbor " << adjacentNode->value << std::endl;
                    }
                    adjacentNode->parent = currentNode;
                    q.push(adjacentNode);
                    adjacentNode->color = Node::WHITE;
                }
            }
        }
    }
    return path;
}