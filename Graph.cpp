#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>

#include "Graph.h"
#include "StringUtils.h"

Graph::Graph()
{

}

Graph::~Graph()
{
	std::vector<Node*>::iterator it;
	for (it=nodes.begin(); it!=nodes.end(); ++it)
	{
		Node* node = *it;
		delete node;
	}
}

void Graph::readAdjacencyListFromFile(const char* filename)
{
	adjacencyList.clear();
	std::ifstream file;
	file.open("48US.txt", std::ios::in);
	if (file.is_open())
	{
		while (file.good())
		{
			std::string line; 
			getline(file,line);
			std::vector<std::string> tokens = StringUtils::split(line, ',');
			//create adjacency list entry
			std::pair< std::string, std::vector<std::string> > pair;
			pair.first = tokens[0];
			tokens.erase(tokens.begin());
			pair.second = tokens;
			adjacencyList.push_back(pair);
		}
		file.close();

		//create a node for each adjacency list entry
		AdjacencyList::const_iterator it;
		for (it=adjacencyList.begin(); it!=adjacencyList.end(); ++it)
		{
			//create node
			Node* node = new Node();
			node->value = it->first;
			nodes.push_back(node);
		}
		//populate adjacentNodes of each node
		for (it=adjacencyList.begin(); it!=adjacencyList.end(); ++it)
		{
			Node* node = findNodeByValue(it->first);
			std::vector<std::string>::const_iterator jt;
			for (jt=it->second.begin(); jt!=it->second.end(); ++jt)
			{
				Node* adjacentNode = findNodeByValue(*jt);
				node->adjacentNodes.push_back(adjacentNode);
			}
		}
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
	std::vector<Node*>::const_iterator it;
	for (it=nodes.begin(); it!=nodes.end(); ++it)
	{
		Node *node = *it;
		if (node->value == value)
		{
			return node;
		}
	}
	return NULL;
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
		if (DEBUG)
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
			if (DEBUG)
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
				if (adjacentNode->color != Node::WHITE)
				{
					if (DEBUG)
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