CC_FLAGS = -c -Wall -fpermissive
OBJ = main.o StringUtils.o Game.o USCountiesGame.o SvgFile.o IntegerIdAdjacencyListFile.o CsvAdjacencyListFile.o USCountiesAdjacencyListFile.o USCountiesSvgFile.o TravelingSalesmanProblem.o tinyxml2.o

all: graphgame

graphgame: $(OBJ)
	g++ $(OBJ) -o graphgame

main.o: main.cpp
	g++ $(CC_FLAGS) main.cpp

StringUtils.o: StringUtils.cpp
	g++ $(CC_FLAGS) StringUtils.cpp

Game.o: Game.cpp
	g++ $(CC_FLAGS) Game.cpp

USCountiesGame.o: USCountiesGame.cpp
	g++ $(CC_FLAGS) USCountiesGame.cpp

SvgFile.o: SvgFile.cpp
	g++ $(CC_FLAGS) SvgFile.cpp

IntegerIdAdjacencyListFile.o: IntegerIdAdjacencyListFile.cpp
	g++ $(CC_FLAGS) IntegerIdAdjacencyListFile.cpp

CsvAdjacencyListFile.o: CsvAdjacencyListFile.cpp
	g++ $(CC_FLAGS) CsvAdjacencyListFile.cpp

USCountiesAdjacencyListFile.o: USCountiesAdjacencyListFile.cpp
	g++ $(CC_FLAGS) USCountiesAdjacencyListFile.cpp

USCountiesSvgFile.o: USCountiesSvgFile.cpp
	g++ $(CC_FLAGS) USCountiesSvgFile.cpp

TravelingSalesmanProblem.o: TravelingSalesmanProblem.cpp
	g++ $(CC_FLAGS) TravelingSalesmanProblem.cpp

tinyxml2.o:	tinyxml2/tinyxml2.cpp
	g++ $(CC_FLAGS) tinyxml2/tinyxml2.cpp

clean:
	rm -rf *o graphgame
