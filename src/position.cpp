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
