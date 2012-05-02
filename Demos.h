#ifndef GRAPH_GAME_DEMOS_H
#define GRAPH_GAME_DEMOS_H

#include "IntegerIdAdjacencyListFile.h"
#include "SvgFile.h"

namespace GraphGame
{
    void breadthFirstSearch_Demo(IntegerIdAdjacencyListFile* af, SvgFile* svg, 
                                 int start, int goal);
    void greedyColoring(IntegerIdAdjacencyListFile* af, SvgFile* svg);
    void randomColoring(IntegerIdAdjacencyListFile* af, SvgFile* svg);
    void showNeighbors(IntegerIdAdjacencyListFile* af, SvgFile* svg, int nodeId);
    std::string randomColor();
}

#endif
