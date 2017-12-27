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

#include "calque.hpp"
#include <algorithm>

void calque::update_busy_cellules()
{
    busy.clear();

    for(register unsigned int x = 0; x < get_size_x(); x++)
	for(register unsigned int y = 0; y < get_size_y(); y++)
	    if(get_etat(x, y) == plein)
		busy.push_back(cellule(x, y));

    sort();
}

bool calque::operator == (const calque & ref)
{
    unsigned int i = 0;

    if(busy.size() != ref.busy.size())
	return false;

    while(i < busy.size() && busy[i] == ref.busy[i])
	i++;

    return i >= busy.size();
}

void calque::sort()
{
    std::sort(busy.begin(), busy.end());
}

void calque::set_etat(unsigned int x, unsigned int y, etat val)
{
    vector<cellule>::iterator it = find(busy.begin(), busy.end(), cellule(x, y));

    if(it == busy.end())
    {
	if(val == plein)
	    busy.push_back(cellule(x, y));
    }
    else
    {
	if(val == vide)
	    busy.erase(it);
    }

    sort();
    espace<etat>::set_etat(x, y, val);
}

void calque::clear(etat val)
{
    espace<etat>::clear(val);
    if(val == vide)
	busy.clear();
    else
	update_busy_cellules();
}

void calque::change_val(const etat val, etat new_val)
{
    espace<etat>::change_val(val, new_val);
    update_busy_cellules();
}
