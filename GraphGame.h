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
        MODE_BIPARTITE_DEMO,
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

    #define GRAPH_GAME_ROOT "/var/www/graphgame"

    class GraphGame
    {
    public:
        static bool showWarnings;
        static std::string absolutePath(const char* relativePath);
        static int parseCommandLineArgs(int argc, char *argv[]);
    };
}

#endif
