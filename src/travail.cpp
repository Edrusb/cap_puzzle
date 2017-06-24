#include "travail.hpp"
#include <algorithm>

travail::travail(unsigned int dimx,
		 unsigned int dimy,
		 const vector<calque_set> & config): current(dimx, dimy, '.'),
						     configuration(config)
{
    for(unsigned int i = 0; i < configuration.size() ; i++)
	avail.push_back(i);
}


void travail::find_candidates(vector<candidate> & dispo)
{
    signed int free_x, free_y;
    candidate tmp;

    dispo.clear();

	// recherche d'une place libre (une seule suffit) à remplir dans le tableau en cours
	// (si plus de place libre -> pas de solution au problème)

    if(!current.find_free_space(free_x, free_y))
	throw E_IMPOSSIBLE; // tout l'espace est utilise pourtant il reste des pieces a ajouter !


	// recherche des calques disponibles pour les pièces non utilisées

    for(register unsigned int piid = 0; piid < avail.size(); piid++)
    {
	assert(avail[piid] < configuration.size());
	const calque_set & current_calque_set = configuration[avail[piid]];
	unsigned int num_calque = current_calque_set.get_total_num();

	for(unsigned int caid = 0; caid < num_calque; caid++)
	    if(current_calque_set.read_calque(caid).get_etat(free_x, free_y) == plein)
	    {
		tmp.calque_set_index = avail[piid];
		tmp.busy_cellules = & current_calque_set.read_calque(caid).get_busy_cellules();
		tmp.symbol = current_calque_set.get_symbol();
		dispo.push_back(tmp);
	    }
    }
}


bool travail::push_candidate(const candidate & candid)
{
    assert(candid.busy_cellules != nullptr);
    if(current.add(*(candid.busy_cellules), candid.symbol))
    {
	vector<unsigned int>::iterator it = find(avail.begin(), avail.end(), candid.calque_set_index);
	if(it == avail.end())
	    E_BUG;
	avail.erase(it);
	return true;
    }
    else
	return false;
}

void travail::pop_candidate(const candidate & candid)
{
    assert(candid.busy_cellules != nullptr);
    avail.push_back(candid.calque_set_index);
    current.remove(*(candid.busy_cellules), candid.symbol);
}
