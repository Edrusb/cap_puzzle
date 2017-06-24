#include "calque.hpp"
#include <algorithm>

calque & calque::operator = (const calque & ref)
{
    const espace<etat> *x_ref = & ref;
    espace<etat> *x_this = this;

    *x_this = *x_ref;
    busy = ref.busy;

    return *this;
}

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

	/*   unsigned int maxi = busy.size() - 1;
    cellule tmp = cellule(0,0);

    for(unsigned int i = 0; i < maxi; i++)
	for(unsigned int j = i+1; j < busy.size(); j++)
	    if(busy[j] < busy[i])
	    {
		tmp = busy[i];
		busy[i] = busy[j];
		busy[j] = tmp;
	    }

	*/
}


void calque::set_etat(unsigned int x, unsigned int y, etat val)
{
    vector<cellule>::iterator it;

    espace<etat>::set_etat(x, y, val);
    it = find(busy.begin(), busy.end(), cellule(x, y));
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
