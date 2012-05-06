CC_FLAGS = -c -g -Wall -fpermissive
OBJ = main.o GraphGame.o StringUtils.o Game.o Demos.o USCountiesGame.o SvgFile.o CsvFile.o USCountiesSvgFile.o USStatesSvgFile.o IntegerIdAdjacencyListFile.o CsvAdjacencyListFile.o USCountiesAdjacencyListFile.o TravelingSalesmanProblem.o tinyxml2.o

all: graphgame

graphgame: $(OBJ)
	g++ $(OBJ) -o graphgame

main.o: main.cpp
	g++ $(CC_FLAGS) main.cpp

GraphGame.o: GraphGame.cpp
	g++ $(CC_FLAGS) GraphGame.cpp

StringUtils.o: StringUtils.cpp
	g++ $(CC_FLAGS) StringUtils.cpp

Demos.o: Demos.cpp
	g++ $(CC_FLAGS) Demos.cpp

Game.o: Game.cpp
	g++ $(CC_FLAGS) Game.cpp

USCountiesGame.o: USCountiesGame.cpp
	g++ $(CC_FLAGS) USCountiesGame.cpp

SvgFile.o: SvgFile.cpp
	g++ $(CC_FLAGS) SvgFile.cpp

CsvFile.o: CsvFile.cpp
	g++ $(CC_FLAGS) CsvFile.cpp

USCountiesSvgFile.o: USCountiesSvgFile.cpp
	g++ $(CC_FLAGS) USCountiesSvgFile.cpp

USStatesSvgFile.o: USStatesSvgFile.cpp
	g++ $(CC_FLAGS) USStatesSvgFile.cpp

IntegerIdAdjacencyListFile.o: IntegerIdAdjacencyListFile.cpp
	g++ $(CC_FLAGS) IntegerIdAdjacencyListFile.cpp

CsvAdjacencyListFile.o: CsvAdjacencyListFile.cpp
	g++ $(CC_FLAGS) CsvAdjacencyListFile.cpp

USCountiesAdjacencyListFile.o: USCountiesAdjacencyListFile.cpp
	g++ $(CC_FLAGS) USCountiesAdjacencyListFile.cpp

TravelingSalesmanProblem.o: TravelingSalesmanProblem.cpp
	g++ $(CC_FLAGS) TravelingSalesmanProblem.cpp

tinyxml2.o:	tinyxml2/tinyxml2.cpp
	g++ $(CC_FLAGS) tinyxml2/tinyxml2.cpp

clean:
	rm -rf *o graphgame
