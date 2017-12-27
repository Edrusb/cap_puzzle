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

#ifndef CELLULE_HPP
#define CELLULE_HPP

    /// defines coordinates in a 2 dimensions table

struct cellule
{
    cellule(unsigned int vx, unsigned int vy) { x = vx; y = vy; };

    unsigned int x;
    unsigned int y;
    bool operator < (const cellule & ref) const { return x < ref.x || (x == ref.x && y < ref.y); };
    bool operator == (const cellule & ref) const { return x == ref.x && y == ref.y; };
};

#endif
