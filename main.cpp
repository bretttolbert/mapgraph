#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstring>

#include "Game.h"
#include "Graph.h"

enum Mode
{
	MODE_UNDEFINED,
	MODE_GAME,
	MODE_BFS
};

int main(int argc, char *argv[])
{
	Mode mode = MODE_UNDEFINED;
	const char* adjacencyFile;
	AdjacencyFileFormat fmt = UNDEFINED;

	if (argc == 1)
	{
		std::cerr << "Usage: ?\n";
		return 1;
	}

	for (int i=1; i<argc; ++i)
	{
		if (strcmp(argv[i], "-m") == 0)
		{
			if (++i < argc)
			{
				if (strcmp(argv[i], "game") == 0)
				{
					mode = MODE_GAME;
				}
				else if (strcmp(argv[i], "bfs") == 0)
				{
					mode = MODE_BFS;
				}
				else
				{
					std::cerr << "Invalid mode \"" << argv[i] << "\"\n";
					return 1;
				}
			}
			else
			{
				std::cerr << "Error: Expected mode after -m flag\n";
				return 1;
			}
		}
		else if (strcmp(argv[i], "-af") == 0)
		{
			if (++i < argc)
			{
				adjacencyFile = argv[i];
			}
			else
			{
				std::cerr << "Error: Expected filename\n";
				return 1;
			}
			if (++i < argc)
			{
				if (strcmp(argv[i], "slcsv") == 0)
				{
					fmt = SINGLE_LINE_CSV;
				}
				else if (strcmp(argv[i], "mltd") == 0)
				{
					fmt = MULTI_LINE_TAB_DELIMITED_US_CENSUS;
				}
				else
				{
					std::cerr << "Invalid format \"" << argv[i] << "\"\n";
					return 1;
				}
			}
			else
			{
				std::cerr << "Error: Expected file format specifier\n";
				return 1;
			}
		}
		else
		{
			std::cerr << "Unrecognized argument \"" << argv[i] << "\"\n";
			return 1;
		}
	}

	if (adjacencyFile == NULL)
	{
		std::cerr << "Error: No adjacency file specified\n";
		return 1;
	}
	else if (mode == MODE_UNDEFINED)
	{
		std::cerr << "Error: Mode not specified\n";
		return 1;
	}
	else if (fmt == UNDEFINED)
	{
		std::cerr << "Error: Adjacency file format not specified\n";
		return 1;
	}

	if (mode == MODE_GAME)
	{
		Game(adjacencyFile, fmt);
	}
	else if (mode == MODE_BFS)
	{
		std::cout << "BFS\n";
	}
	return 0;
}