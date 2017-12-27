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

#ifndef CALQUE_HPP
#define CALQUE_HPP

#include <vector>
#include "espace.hpp"
#include "etat.hpp"
#include "cellule.hpp"

using namespace std;

    /// calque is a binary espace (empty/busy) which also provides an optimized way to get the list of busy cells

class calque : public espace<etat>
{
public:
    calque(unsigned int x, unsigned int y) : espace<etat>(x, y) { busy.clear(); };

    bool operator == (const calque & ref);
    const vector<cellule> & get_busy_cellules() const { return busy; };
    void clear() { espace<etat>::clear(vide); busy.clear(); };

	// surcharge des methode de espace

    void set_etat(unsigned int x, unsigned int y, etat val);
    void clear(etat val);
    void change_val(const etat val, etat new_val);

private:
    vector<cellule> busy; // list is sorted

    void update_busy_cellules();
    void sort();
};

#endif
