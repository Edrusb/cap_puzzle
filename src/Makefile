CXXFLAGS = -O3 -std=c++11 -DNDEBUG
LDFLAGS = -O3 -std=c++11 -lthreadar

#CXXFLAGS = -O2  -std=c++11 -DNDEBUG
#LDFLAGS = -O2 -std=c++11 -lthreadar

#CXXFLAGS = -g -std=c++11 -pedantic -pg
#LDFLAGS = -g -std=c++11 -lthreadar -pg

OBJ=cap_puzzle.o calque_set.o position.o calque.o pieces.o rotation.o board.o fd_tools.o travail.o team.o todo_list.o robot.o resultat.o

all: cap_puzzle

clean:
	rm -f *.o cap_puzzle

cap_puzzle: $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $@

cap_puzzle.cpp : calque_set.hpp pieces.hpp board.hpp travail.hpp
calque_set.cpp : calque_set.hpp
position.cpp: position.hpp
calque.cpp: calque.hpp
pieces.cpp: pieces.hpp
rotation.cpp: rotation.hpp
board.cpp: board.hpp
fd_tools.cpp: fd_tools.hpp
travail.cpp: travail.hpp
team.cpp: team.hpp erreurs.hpp
todo_list.cpp: todo_list.hpp
robot.cpp: robot.hpp
resultat.cpp: resultat.hpp

calque_set.hpp: pieces.hpp calque.hpp position.hpp
calque.hpp: espace.hpp etat.hpp
pieces.hpp: erreurs.hpp etat.hpp espace.hpp
board.hpp: espace.hpp calque.hpp erreurs.hpp rotation.hpp
position.hpp: rotation.hpp
calque.hpp: espace.hpp etat.hpp cellule.hpp
espace.hpp: erreurs.hpp fd_tools.hpp
travail.hpp: candidate.hpp board.hpp calque_set.hpp
todo_list.hpp: travail.hpp
robot.hpp: travail.hpp team.hpp resultat.hpp todo_list.hpp
