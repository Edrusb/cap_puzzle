/*********************************************************************/
// cap_puzzle - a puzzle resolution program
// Copyright (C) 2002-2018 Denis Corbin
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// to contact the author : http://dar.linux.free.fr/email.html
/*********************************************************************/

#ifndef TRAVAIL_HPP
#define TRAVAIL_HPP

#include "candidate.hpp"
#include "board.hpp"
#include "calque_set.hpp"
#include <deque>
#include <unordered_set>

using namespace std;

    // defines work in progress

class travail
{
public:
    travail(unsigned int dimx,
	    unsigned int dimy,
	    const vector<calque_set> & config);

    bool all_placed() { return avail.size() == 0; };//< vrai si toutes les pieces ont ete placees
    void find_candidates(deque<candidate> & dispo); //< fournit la liste des candidats possible pour poursuivre la resolution
    bool push_candidate(const candidate & candid); //< applique un candidat au travail courant
    void pop_candidate(const candidate & candid);  //< retire un candidat du travail courant

public:
    board current;

private:
    const vector<calque_set> & configuration; //< pointe sur les calques des pieces
    unordered_set<unsigned int> avail; //< liste des pieces restant a placer sur le plateau
};

#endif
