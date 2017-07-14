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
