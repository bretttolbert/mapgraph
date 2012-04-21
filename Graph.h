#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <set>

#ifndef DEBUG
#define DEBUG 0
#endif

/**
 * Adjacency File Formats:
 *   SINGLE_LINE_CSV
 *     Example file: 48US.txt
 *     One line per node with tokens seperated by commas.
 *     First token is the node. The rest of the tokens are its neighbors.
 *   MULTI_LINE_TAB_DELIMITED_US_CENSUS
 *     Example file: US_county_adjacency_2010.txt (from http://www.census.gov/geo/www/geoareas/county_adjacency.html)
 *     Each node entry spans multiple lines. The first line is the node.
 *     The followings lines are its neighbors. Each line consists of the
 *     following columns: 
 *	     1 County Name	    2010 State and County Name
 *	     2 County GEOID     2010 State and County FIPS Codes
 *	     3 Neighbor Name	2010 State and County name of each neighboring county or county equivalent
 *	     4 Neighbor GEOID	2010 State and County FIPS Codes of each neighboring county or county equivalent
 *     For the first line of a block, columns 3 and 4 are identical to columns 1 and 2
 *     For the rest of the lines in a block, columns 1 and 2 are empty.
 **/
enum AdjacencyFileFormat
{
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