#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <set>

#define DEBUG_GRAPH 0

enum AdjacencyFileFormat
{
    UNDEFINED,
    SINGLE_LINE_CSV,
    MULTI_LINE_TAB_DELIMITED_US_CENSUS
};

class Graph
{
public:
    class Node
    {
    public:
        enum Color
        {
            BLACK, WHITE, RED, GREEN, BLUE
        };
        Node();
        bool operator<(const Node &other) const;
        std::string value;
        std::vector<Node*> adjacentNodes;
        Color color; //used in various ways by different searching and traversal algorithms
        Node* parent; //used by pathfinding algorithms
    };

    Graph();
    ~Graph();

    /**
     *
     */
    void readAdjacencyListFromFile(const char* filename, AdjacencyFileFormat fmt);

    /**
     *
     */
    void printAdjacencyList();

    /**
     *
     */
    bool isNodeValid(const std::string& node);

    /**
     * Returns ptr to node with given value
     * Returns NULL if none found
     */
    Node* findNodeByValue(const std::string& value);

    /**
     *
     */
    std::vector<std::string> getNeighbors(const std::string& node);

    /**
     * Prints the neighbors of the specified node using the specified
     * string a delimeter. Does not print a newline.
     */
    void printNeighbors(const std::string& node, const char* delim);

    /**
     * Returns a vector containing the value of every node in the graph
     */
    std::vector<std::string> getNodeValues();

    /** 
     * Uses breadth first search to find path from start to goal 
     * Returns vector of values representing path taken
     */
    std::vector<std::string> breadthFirstSearch(std::string startNodeValue, std::string goalNodeValue);

private:
    typedef std::pair<std::string, std::vector<std::string> > AdjacencyListEntry;
    typedef std::vector< AdjacencyListEntry > AdjacencyList;
    AdjacencyList adjacencyList;
    struct NodeComparator
    {
        bool operator() (const Node* a, const Node* b);
    };
    typedef std::set<Node*, NodeComparator> NodeSet;
    NodeSet nodes;
};

#endif