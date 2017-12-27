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

#ifndef BOARD_HPP
#define BOARD_HPP

#include "espace.hpp"
#include "calque.hpp"
#include "erreurs.hpp"
#include "rotation.hpp"

    /// manages a board where pieces can be added to and removed from

class board : public espace<unsigned char>
{
public:
    board(unsigned x, unsigned y, unsigned char init) : espace<unsigned char>(x, y), empty(init) { clear(init); };

    bool operator == (const board & ref) const { return espace<unsigned char>::operator ==(ref); };
    bool add(const vector<cellule> & ou, unsigned char quoi);   //< add a piece to the board do nothing and return false if failed
    void remove(const vector<cellule> & ou, unsigned char quoi); //< remove a piece from the board
    bool find_free_space(signed int & x, signed int & y) const;

    bool similaire(const board & ref) const; // retourne true si même board par rotation et/ou retournement (pile/face)

private:
    unsigned char empty;

    board transform(rotation rot, retournement ret) const;
};

#endif
