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

#ifndef TODO_LIST_HPP
#define TODO_LIST_HPP

#include "travail.hpp"
#include <libthreadar/libthreadar.hpp>
#include <deque>

using namespace std;

    // stores works that no worker is currently trying to solve

class todo_list
{
public:
    ~todo_list() { clear(); };
    void add_work(travail *work, unsigned int level);
    bool take_work(travail * & work, unsigned int & level);

    void clear();
private:
    struct indexed_work
    {
	travail *work;
	unsigned int index;
    };

    libthreadar::mutex todo_mutex;
    deque<indexed_work> todo;
    void show();
};

#endif
