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

#include "position.hpp"

bool next_position(position & next, unsigned int size_x, unsigned int size_y)
{
    if(next.cote != face)
	next.cote = face;
    else
	if(suivant(next.orientation))
	    next.cote = pile;
	else
	    if(next.loc_x < size_x - 1)
	    {
		next.cote = pile;
		next.orientation = nord;
		next.loc_x++;
	    }
	    else
		if(next.loc_y < size_y - 1)
		{
		    next.cote = pile;
		    next.orientation = nord;
		    next.loc_x = 0;
		    next.loc_y++;
		}
		else
		    return false;

    return true;
}
