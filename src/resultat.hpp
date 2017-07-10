#ifndef RESULTAT_HPP
#define RESULTAT_HPP

#include "board.hpp"
#include <list>

using namespace std;

typedef list<board> resultat;

extern resultat operator += (resultat & x, const resultat & y);

#endif
