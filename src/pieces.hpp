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

#ifndef PIECES_HPP
#define PIECES_HPP

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

#include "erreurs.hpp"
#include "etat.hpp"
#include "espace.hpp"

using namespace std;

class piece : public espace<etat>
{
public:
    piece(unsigned char symbole, unsigned int x, unsigned y) : espace<etat>(x, y) { symbol = symbole; };
    piece(int fd);

    unsigned char get_symbol() const { return symbol; };

    bool operator < (const piece & ref) const { return get_size_x() < ref.get_size_x(); };

private:
    unsigned char symbol;
};

#endif
