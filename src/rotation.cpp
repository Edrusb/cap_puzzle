#include "rotation.hpp"

bool suivant(rotation & arg)
{
    switch(arg)
    {
    case nord:
	arg = est;
	return true;
    case est:
	arg = sud;
	return true;
    case sud:
	arg = ouest;
	return true;
    default:
	return false;
    }
}

