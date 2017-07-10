#include "resultat.hpp"

resultat operator += (resultat & x, const resultat & y)
{
    resultat::const_iterator it = y.begin();

    while(it != y.end())
    {
	x.push_back(*it);
	++it;
    }

    return x;
}
