/*********************************************************************/
// cap_puzzle - a puzzle resolution program
// Copyright (C) 2002-2018 Denis Corbin
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// to contact the author : http://dar.linux.free.fr/email.html
/*********************************************************************/

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
