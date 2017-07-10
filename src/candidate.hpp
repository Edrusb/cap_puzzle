#ifndef CANDIDATE_HPP
#define CANDIDATE_HPP

#include "cellule.hpp"
#include <vector>

using namespace std;

    /// holds information about a calque to try in order to fill a hole

struct candidate
{
    unsigned int calque_set_index;          //< index of the piece the calque is taken from
    const vector<cellule> * busy_cellules;  //< list of cells occupied by the piece in that position
    unsigned char symbol;                   //< symbol of the piece
};

#endif
