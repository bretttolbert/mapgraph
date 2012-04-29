CC_FLAGS = -c -Wall -fpermissive
OBJ = main.o StringUtils.o 

all: graphgame

graphgame: $(OBJ)
	g++ $(OBJ) -o graphgame.cgi

main.o: main.cpp
	g++ $(CC_FLAGS) main.cpp

StringUtils.o: ../StringUtils.cpp
	g++ $(CC_FLAGS) ../StringUtils.cpp

clean:
	rm -rf *.o graphgame.cgi
