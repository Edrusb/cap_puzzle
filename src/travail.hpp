#ifndef TRAVAIL_HPP
#define TRAVAIL_HPP

#include "candidate.hpp"
#include "board.hpp"
#include "calque_set.hpp"
#include <vector>

using namespace std;

class travail
{
public:
    travail(unsigned int dimx,
	    unsigned int dimy,
	    const vector<calque_set> & config);

    bool all_placed() { return avail.size() == 0; };
    void find_candidates(vector<ensemble> & dispo);
    bool push_candidate(const ensemble & candid);
    void pop_candidate(const ensemble & candid);

    board current;
private:
    const vector<calque_set> & configuration;
    vector<unsigned int> avail;

};

#endif
