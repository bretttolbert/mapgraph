#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <set>
#include <queue>
#include <iterator>

#include "AdjacencyListFile.h"

#define DEBUG_GRAPH 0

template <typename T>
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
        T value;
        std::vector<Node*> adjacentNodes;
        Color color; //used in various ways by different searching and traversal algorithms
        Node* parent; //used by pathfinding algorithms
    };

    Graph(typename const AdjacencyListFile<T>::AdjacencyList& adjacencyList);
    ~Graph();

    Node* Graph::findNodeByValue(const T& value);

    /** 
     * Uses breadth first search to find path from start to goal 
     * Returns vector of values representing path taken
     */
    std::vector<T> breadthFirstSearch(T startNodeValue, T goalNodeValue);
private:
    struct NodeComparator
    {
        bool operator() (const Node* a, const Node* b);
    };
    typedef std::set<Node*, NodeComparator> NodeSet;
    NodeSet nodes;
};

template <typename T>
inline
Graph<T>::Graph(typename const AdjacencyListFile<T>::AdjacencyList& adjacencyList)
{
    //create a node for each adjacency list entry
    AdjacencyListFile<T>::AdjacencyList::const_iterator it;
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
        std::vector<T>::const_iterator jt;
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

template <typename T>
inline
Graph<T>::~Graph()
{
    NodeSet::iterator it;
    for (it=nodes.begin(); it!=nodes.end(); ++it)
    {
        Node* node = *it;
        delete node;
    }
}

template <typename T>
inline
typename Graph<T>::Node* Graph<T>::findNodeByValue(const T& value)
{
    Graph<T>::Node temp;
    temp.value = value;
    Graph<T>::NodeSet::const_iterator it = nodes.find(&temp);
    if (it != nodes.end())
    {
        return *it;
    }
    else
    {
        return NULL;
    }
}

template <typename T>
inline
Graph<T>::Node::Node() : 
    color(BLACK), 
    parent(NULL) 
{ }

template <typename T>
inline
bool Graph<T>::NodeComparator::operator() (const Node* a,  const Node* b)
{
    return (a->value < b->value);
}

template <typename T>
inline
typename std::vector<T> Graph<T>::breadthFirstSearch(T startNodeValue, T goalNodeValue)
{
    std::vector<T> path;
    Graph<T>::Node* startNode = findNodeByValue(startNodeValue);
    std::queue<Graph<T>::Node*> q;
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
            Graph<T>::Node *node = currentNode;
            while (node != NULL)
            {
                path.push_back(node->value);
                node = node->parent;
            }
            std::reverse(path.begin(),path.end());
            if (DEBUG_GRAPH)
            {
                std::cout << "found it\npath taken: ";
                std::ostream_iterator<int> output(std::cout, " ");
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
                Graph<T>::Node* adjacentNode = *it;
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
    //reset node color and parent
    Graph<T>::NodeSet::const_iterator it;
    for (it=nodes.begin(); it!=nodes.end(); ++it)
    {
        Graph<T>::Node* node = *it;
        node->color = Graph<int>::Node::BLACK;
        node->parent = NULL;
    }
    return path;
}

#endif