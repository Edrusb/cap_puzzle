#######################################################################
# cap_puzzle - a puzzle resolution program
# Copyright (C) 2002-2018 Denis Corbin
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# to contact the author : http:dar.linux.free.fr/email.html
#######################################################################

# STATIC = -static
STATIC =
CXX_STD=-std=c++11
# OPTIM=-O4 -DNDEBUG
# OPTIM=-O2 -DNDEBUG
OPTIM=-ggdb

CXXFLAGS = $(OPTIM) $(CXX_STD) `pkg-config --cflags libthreadar` $(STATIC)
LDFLAGS = $(OPTIM) $(CXX_STD) $(STATIC)
LIBS=`pkg-config --libs libthreadar`

OBJ=cap_puzzle.o calque_set.o position.o calque.o pieces.o rotation.o board.o fd_tools.o travail.o team.o todo_list.o robot.o resultat.o

all: cap_puzzle

clean:
	rm -f *.o cap_puzzle

cap_puzzle: $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) $(LIBS) -o $@

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
