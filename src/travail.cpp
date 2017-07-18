#include "travail.hpp"
#include <algorithm>

travail::travail(unsigned int dimx,
		 unsigned int dimy,
		 const vector<calque_set> & config): current(dimx, dimy, '.'),
						     configuration(config)
{
    for(unsigned int i = 0; i < configuration.size() ; ++i)
	avail.push_back(i);
}


void travail::find_candidates(deque<candidate> & dispo)
{
    signed int free_x, free_y;
    candidate tmp;
    unsigned int avail_size = avail.size();

    dispo.clear();

	// recherche d'une place libre (une seule suffit) à remplir dans le tableau en cours
	// (si plus de place libre -> pas de solution au problème)

    if(!current.find_free_space(free_x, free_y))
	throw E_IMPOSSIBLE; // tout l'espace est utilise pourtant il reste des pieces a ajouter !


	// recherche des calques disponibles pour les pièces non utilisées

    for(register unsigned int piid = 0; piid < avail_size; piid++)
    {
	tmp.calque_set_index = avail[piid];

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
