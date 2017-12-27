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

#include "calque_set.hpp"
#include <assert.h>
#include <algorithm>

calque_set::calque_set(const piece & p, unsigned int x, unsigned int y)
{
    position pos;
    calque cal = calque(x, y);

    symbol = p.get_symbol();
    dimx = x;
    dimy = y;

    do
    {
	cal.clear();
	if(update_calque_with(p, pos, cal))
	    possible.push_back(cal);
    }
    while(next_position(pos, x, y));

    uniques();
}

const calque & calque_set::read_calque(unsigned int x) const
{
    assert(x < possible.size());
    return possible[x];
}

const vector<cellule> & calque_set::get_busy_cellules_for_calque(unsigned int index) const
{
    assert(index < possible.size());
    return possible[index].get_busy_cellules();
}

bool calque_set::update_calque_with(const piece & p, const position & pos, calque & result)
{
    register signed int a1, b1, a2, b2; //< coefficients matrice 2x2 (ligne a et b, colonnes 1 et 2)
    bool conflict = false;

    switch(pos.orientation)
    {
    case nord:
	a1 = b2 = 1;
	a2 = b1 = 0;
	break;
    case est:
	a2 = 1;
	a1 = b2 = 0;
	b1 = -1;
	break;
    case sud:
	a1 = b2 = -1;
	a2 = b1 = 0;
	break;
    case ouest:
	b1 = 1;
	a1 = b2 = 0;
	a2 = -1;
	break;
    default:
	E_BUG
    }

    if(pos.cote == face)
    {
	b1 = -b1;
	b2 = -b2;
    }


    for(register unsigned int x = 0; !conflict && x < p.get_size_x(); x++)
	for(register unsigned int y = 0; !conflict && y < p.get_size_y(); y++)
	{
	    if(p.get_etat(x,y) != vide)
	    {
		register signed int b_x = pos.loc_x + a1*x + b1*y;
		register signed int b_y = pos.loc_y + a2*x + b2*y;

		if(b_x < 0 || b_y < 0 || b_x >= result.get_size_x() || b_y >= result.get_size_y())
		    conflict = true; // la piece sort du cadre apres la transformation "pos"
		else
		    result.set_etat(b_x, b_y, plein);
 	    }
	}

    return !conflict;
}

void calque_set::uniques()
{
    vector<calque> resultat;
    vector<calque>::iterator it, chch;

    for(it = possible.begin(); it != possible.end(); it++)
    {
	chch = find(resultat.begin(), resultat.end(), *it);
	if(chch == resultat.end())
	    resultat.push_back(*it);
    }

    possible = resultat;
}
