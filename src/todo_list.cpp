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

#include "todo_list.hpp"

void todo_list::add_work(travail *work, unsigned int level)
{
    todo_mutex.lock();
    try
    {
	indexed_work tmp;
	tmp.work = work;
	tmp.index = level;
	todo.push_back(tmp);
#ifndef NDEBUG
	show();
#endif
    }
    catch(...)
    {
	todo_mutex.unlock();
	throw;
    }
    todo_mutex.unlock();
}

bool todo_list::take_work(travail * & work, unsigned int & level)
{
    bool ret = false;

    todo_mutex.lock();
    try
    {
	if(todo.size() > 1)
	{
	    work = todo.back().work;
	    level = todo.back().index;
	    todo.pop_back();
	    ret = true;
#ifndef NDEBUG
	    show();
#endif
	}
    }
    catch(...)
    {
	todo_mutex.unlock();
	throw;
    }
    todo_mutex.unlock();

    return ret;
}

void todo_list::clear()
{
    todo_mutex.lock();
    try
    {
	deque<indexed_work>::iterator it = todo.begin();

	while(it != todo.end())
	{
	    if(it->work != nullptr)
		delete it->work;
	    ++it;
	}
	todo.clear();
    }
    catch(...)
    {
	todo_mutex.unlock();
	throw;
    }
    todo_mutex.unlock();
}

void todo_list::show()
{
    cout << "Todo list size = " << todo.size() << endl;
}
