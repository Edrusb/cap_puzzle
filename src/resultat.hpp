#ifndef RESULTAT_HPP
#define RESULTAT_HPP

#include "board.hpp"
#include <vector>

using namespace std;

typedef vector<board> resultat;

extern resultat operator += (resultat & x, const resultat & y);

#endif
