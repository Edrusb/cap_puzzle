#include "board.hpp"
#include <iostream>
#include <vector>

using namespace std;

board & board::operator = (const board & ref)
{
    const espace<unsigned char> *x_ref = &ref;
    espace<unsigned char> *x_this = this;

    *x_this = *x_ref;
    empty = ref.empty;

    return *this;
}

bool board::add(const vector<cellule> & ou, unsigned char quoi)
{
    for(signed int num = 0; num < ou.size(); num++)
    {
	assert(ou[num].x < get_size_x());
	assert(ou[num].y < get_size_y());
	if(get_etat(ou[num].x, ou[num].y) == empty)
	    set_etat(ou[num].x, ou[num].y, quoi);
	else
	{
	    for(signed int denum = num-1; denum >= 0; denum--)
	    {
		assert(get_etat(ou[denum].x, ou[denum].y) == quoi);
		set_etat(ou[denum].x, ou[denum].y, empty);
	    }
	    return false;
	}
    }
    return true;
}

void board::remove(const vector<cellule> & ou, unsigned char quoi)
{
    for(unsigned int num = 0; num < ou.size(); num++)
    {
	assert(ou[num].x < get_size_x());
	assert(ou[num].y < get_size_y());
	assert(get_etat(ou[num].x, ou[num].y) == quoi);
	set_etat(ou[num].x, ou[num].y, empty);
    }
}

bool board::find_free_space(signed int & x, signed int & y) const
{
    register signed int min_x = 0;
    register signed int max_x = get_size_x();
    register signed int min_y = 0;
    register signed int max_y = get_size_y();

	// on cherche le long du bord et on tourne vers le centre.

    try
    {
	while(max_x >= min_x && max_y >= min_y)
	{
		// en haut
	    x = min_x + 1;
	    y = min_y;
	    while(x < max_x)
		if(get_etat(x, y) == empty)
		    throw true;
		else
		    x++;

		//  a droite
	    x = max_x - 1;
	    y++;
	    while(y < max_y)
		if(get_etat(x, y) == empty)
		    throw true;
		else
		    y++;

		// en bas
	    x--;
	    y = max_y - 1;
	    while(x >= min_x)
		if(get_etat(x, y) == empty)
		    throw true;
		else
		    x--;

		// a gauche
	    x = min_x;
	    y--;
	    while(y >= min_y)
		if(get_etat(x, y) == empty)
		    throw true;
		else
		    y--;

		// on reduit le cadre si pas trouve
	    min_x++;
	    min_y++;
	    max_x--;
	    max_y--;
	}
    }
    catch(bool)
    {
	return true;
    }
    return false;
}

board board::transform(rotation rot, retournement ret) const
{
    board xformed = board(0,0,empty);
    register unsigned int dx, dy;

    if(rot == est || rot == ouest)
	xformed = board(get_size_y(), get_size_x(), empty);
    else
	xformed = board(get_size_x(), get_size_y(), empty);

    for(register unsigned int x = 0; x < get_size_x(); x++)
	for(register unsigned int y = 0; y < get_size_y(); y++)
	{
	    switch(rot)
	    {
	    case nord:
		dx = x;
		dy = y;
		break;
	    case est:
		dx = get_size_y() - y - 1;
		dy = x;
		break;
	    case sud:
		dx = get_size_x() - x - 1;
		dy = get_size_y() - y - 1;
		break;
	    case ouest:
		dx = y;
		dy = get_size_x() - x - 1;
		break;
	    default:
		E_BUG
	    }

	    if(ret == face)
		dx = xformed.get_size_x() - dx - 1;

	    xformed.set_etat(dx, dy, get_etat(x, y));
	}
    return xformed;
}

bool board::similaire(const board & ref) const
{
    rotation rot = nord;

    do
    {
	if(transform(rot, pile) == ref)
	    return true;
	if(transform(rot, face) == ref)
	    return true;
    }
    while(suivant(rot));

    return false;
}
