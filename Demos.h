#ifndef GRAPH_GAME_DEMOS_H
#define GRAPH_GAME_DEMOS_H

#include "IntegerIdAdjacencyListFile.h"
#include "SvgFile.h"

namespace GraphGame
{
    void breadthFirstSearch_Demo(const std::string& start, const std::string& goal);
    void isBipartite_Demo(IntegerIdAdjacencyListFile* af, SvgFile* svg);
    void markEachStateWithRandomColor_Demo();
    void markEachCountyWithRandomColor_Demo();
    std::string randomColor();
}

#endif
