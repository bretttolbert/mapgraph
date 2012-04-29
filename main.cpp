#include <cstdlib>
#include <cmath>
#include <ctime>

#include "GraphGame.h"

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));
    GraphGame::GraphGame::parseCommandLineArgs(argc,argv);
    return 0;
}
