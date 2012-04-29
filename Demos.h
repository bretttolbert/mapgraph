#ifndef GRAPH_GAME_DEMOS_H
#define GRAPH_GAME_DEMOS_H

#include "IntegerIdAdjacencyListFile.h"
#include "SvgFile.h"

namespace GraphGame
{
    void breadthFirstSearch_Demo(IntegerIdAdjacencyListFile* af, SvgFile* svg, 
                                 int start, int goal);
    void isBipartite_Demo(IntegerIdAdjacencyListFile* af, SvgFile* svg);
    void markEachNodeWithRandomColor_Demo(IntegerIdAdjacencyListFile* af, SvgFile* svg);
    std::string randomColor();
}

#endif
