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

#include "travail.hpp"
#include <algorithm>

travail::travail(unsigned int dimx,
		 unsigned int dimy,
		 const vector<calque_set> & config): current(dimx, dimy, '.'),
						     configuration(config)
{
    for(unsigned int i = 0; i < configuration.size() ; ++i)
	avail.insert(i);
}


void travail::find_candidates(deque<candidate> & dispo)
{
    signed int free_x, free_y;
    candidate tmp;

    dispo.clear();

	// recherche d'une place libre (une seule suffit) à remplir dans le tableau en cours
	// (si plus de place libre -> pas de solution au problème)

    if(!current.find_free_space(free_x, free_y))
	throw E_IMPOSSIBLE; // tout l'espace est utilise pourtant il reste des pieces a ajouter !


	// recherche des calques disponibles pour les pièces non utilisées

    for(unordered_set<unsigned int>::iterator piid = avail.begin(); piid != avail.end(); ++piid)
    {
	tmp.calque_set_index = *piid;

	assert(tmp.calque_set_index < configuration.size());
	const calque_set & current_calque_set = configuration[tmp.calque_set_index];
	unsigned int num_calque = current_calque_set.get_total_num();
	tmp.symbol = current_calque_set.get_symbol();

	for(unsigned int caid = 0; caid < num_calque; caid++)
	{
	    const calque & tmp_calque = current_calque_set.read_calque(caid);

	    if(tmp_calque.get_etat(free_x, free_y) == plein)
	    {
		tmp.busy_cellules = & tmp_calque.get_busy_cellules();
		dispo.push_back(tmp);
	    }
	}
    }
}


bool travail::push_candidate(const candidate & candid)
{
    assert(candid.busy_cellules != nullptr);
    if(current.add(*(candid.busy_cellules), candid.symbol))
    {
	unsigned int val = avail.erase(candid.calque_set_index);
	assert(val == 1);
	return true;
    }
    else
	return false;
}

void travail::pop_candidate(const candidate & candid)
{
    assert(candid.busy_cellules != nullptr);
    avail.insert(candid.calque_set_index);
    current.remove(*(candid.busy_cellules), candid.symbol);
}
