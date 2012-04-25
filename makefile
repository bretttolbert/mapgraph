SRC	= main.cpp Game.cpp StringUtils.cpp SvgFile.cpp USCountiesAdjacencyListFile.cpp USCountiesSvgFile.cpp  TravelingSalesmanProblem.cpp tinyxml2/tinyxml2.cpp

all: $(SRC)
	g++ -Wall -fpermissive $(SRC) -o graphgame
