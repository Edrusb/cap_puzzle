#include "resultat.hpp"

resultat operator += (resultat & x, const resultat & y)
{
    vector<board>::const_iterator it = y.begin();

    while(it != y.end())
    {
	x.push_back(*it);
	++it;
    }

    return x;
}
