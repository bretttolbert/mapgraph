#include <cstdlib>
#include <cmath>
#include <ctime>

#include "MapGraph.h"

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));
    MapGraph::MapGraph::parseCommandLineArgs(argc,argv);
    return 0;
}
