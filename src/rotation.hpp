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

#ifndef ROTATION_HPP
#define ROTATION_HPP

enum rotation
{
    nord = 'N',    //< pas de rotation
    est = 'E',     //< rotation 90° dans le sens des aiguilles d'une montre
    sud = 'S',     //< demi tour
    ouest ='O'     //< rotation 90% dans le sens inverse des aiguilles d'une montre
};

enum retournement
{
    pile = 'p',    //< pas d'inversion de face
    face = 'f'     //< retournement de face
};

extern bool suivant(rotation & arg);

#endif
