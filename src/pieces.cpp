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

#include "pieces.hpp"
#include <list>
#include <iostream>

using namespace std;

piece::piece(int fd) : espace<etat>(fd)
{
    unsigned int val;

    read_next('[', fd);
    symbol = read_char_from(fd);
    read_next(']', fd);

    for(unsigned int y = 0; y < get_size_y(); y++)
	for(unsigned int x = 0; x < get_size_x(); x++)
	{
	    val = read_int_from(fd);
	    if(val == 1)
		set_etat(x, y, plein);
	    else
		set_etat(x, y, vide);
	}
}
