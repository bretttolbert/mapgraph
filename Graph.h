#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

#ifndef DEBUG
#define DEBUG 0
#endif

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
	void readAdjacencyListFromFile(const char* filename);

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
	typedef std::vector< std::pair< std::string, std::vector<std::string> > > AdjacencyList;
	AdjacencyList adjacencyList;
	std::vector<Node*> nodes;
};

#endif