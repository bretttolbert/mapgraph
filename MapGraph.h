#ifndef MAP_GRAPH_H
#define MAP_GRAPH_H

#include <string>

namespace MapGraph
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

    #define MAP_GRAPH_ROOT "c:/wamp/www/projects/mapgraph"

    class MapGraph
    {
    public:
        static bool showWarnings;
        static std::string absolutePath(const char* relativePath);
        static int parseCommandLineArgs(int argc, char *argv[]);
    };
}

#endif // MAP_GRAPH_H
