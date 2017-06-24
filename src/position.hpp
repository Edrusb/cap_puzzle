#ifndef POSITION_HPP
#define POSITION_HPP

#include "rotation.hpp"

struct position
{
    position() { loc_x = loc_y = 0; orientation = nord; cote = pile; };

    unsigned int loc_x;   //< position de la premiere case du canneva de la pice
    unsigned int loc_y;   //< position de la premiere case du canneva de la pice
    rotation orientation;
    retournement cote;
};

extern bool next_position(position & next, unsigned int size_x, unsigned int size_y);

#endif
