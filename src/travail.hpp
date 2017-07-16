#ifndef TRAVAIL_HPP
#define TRAVAIL_HPP

#include "candidate.hpp"
#include "board.hpp"
#include "calque_set.hpp"
#include <deque>

using namespace std;

    // defines work in progress

class travail
{
public:
    travail(unsigned int dimx,
	    unsigned int dimy,
	    const vector<calque_set> & config);

    bool all_placed() { return avail.size() == 0; };//< vrai si toutes les pieces ont ete placees
    void find_candidates(deque<candidate> & dispo); //< fournit la liste des candidats possible pour poursuivre la resulution
    bool push_candidate(const candidate & candid); //< applique un candidat au travail courant
    void pop_candidate(const candidate & candid);  //< retire un candidat du travail courant

public:
    board current;

private:
    const vector<calque_set> & configuration; //< pointe sur les calques des pieces
    vector<unsigned int> avail; //< liste des pieces restant a placer sur le plateau
};

#endif
