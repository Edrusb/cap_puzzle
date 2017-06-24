#CXXFLAGS = -O3 -DNDEBUG -std=c++11
#CXXFLAGS = -O2 -DNDEBUG -std=c++11 -pg
CXXFLAGS = -g -pedantic -std=c++11 -pg
#LDFLAGS = -O3 -std=c++11
#LDFLAGS = -O2 -std=c++11 -pg
LDFLAGS = -g

OBJ=puzzle.o calque_set.o position.o calque.o pieces.o rotation.o board.o fd_tools.o

all: puzzle

clean:
	rm -f *.o puzzle

puzzle: $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $@

puzzle.cpp : calque_set.hpp pieces.hpp board.hpp
calque_set.cpp : calque_set.hpp
position.cpp: position.hpp
calque.cpp: calque.hpp
pieces.cpp: pieces.hpp
rotation.cpp: rotation.hpp
board.cpp: board.hpp
fd_tools.cpp: fd_tools.hpp

calque_set.hpp: pieces.hpp calque.hpp position.hpp
calque.hpp: espace.hpp etat.hpp
pieces.hpp: erreurs.hpp etat.hpp espace.hpp
board.hpp: espace.hpp calque.hpp erreurs.hpp rotation.hpp
position.hpp: rotation.hpp
calque.hpp: espace.hpp etat.hpp cellule.hpp
espace.hpp: erreurs.hpp fd_tools.hpp
