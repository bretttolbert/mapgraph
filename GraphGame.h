#ifndef GRAPH_GAME_H
#define GRAPH_GAME_H

#include <string>

namespace GraphGame
{
    enum Mode
    {
        MODE_UNDEFINED,
        MODE_GAME,
        MODE_BFS_DEMO,
        MODE_TSP_DEMO,
        MODE_GREEDY_COLORING,
        MODE_NEIGHBORS,
        MODE_RANDOM_COLORING,
        MODE_TEST
    };

    enum AdjacencyFilePreset
    {
        ADJACENCY_FILE_PRESET_NONE,
        ADJACENCY_FILE_PRESET_US_STATES,
        ADJACENCY_FILE_PRESET_US_COUNTIES
    };

    enum NodesPreset
    {
        NODES_PRESET_NONE,
        NODES_PRESET_WASHINGTONS
    };

    #define GRAPH_GAME_ROOT "/home/brett/Projects/graphgame"

    class GraphGame
    {
    public:
        static bool showWarnings;
        static std::string absolutePath(const char* relativePath);
        static int parseCommandLineArgs(int argc, char *argv[]);
    };
}

#endif
