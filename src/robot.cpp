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

#include "robot.hpp"

todo_list robot::todo;
libthreadar::mutex robot::flag_control;
bool robot::init_flag;
unsigned int robot::parked;
libthreadar::freezer robot::parking;

robot::robot()
{
    work = nullptr;
    level = 0;
    dispo_ptr = nullptr;
}

void robot::reset_init_flag()
{
    flag_control.lock();
    try
    {
	init_flag = true;
	parked = 0;
	parking.reset();
    }
    catch(...)
    {
	flag_control.unlock();
	throw;
    }
    flag_control.unlock();
}

void robot::receive_work(travail *w, unsigned int index)
{
    if(work != nullptr)
	E_BUG;
    work = w;
    level = index;
}

void robot::inherited_inherited_run()
{
    bool loop = true;

    if(check_init_flag())
    {
	    // fill the todo list with some data
	resout(true);
	delete work;
	work = nullptr;
	set_init_flag_down();
    }

    while(loop)
    {
	if(work == nullptr)
	    if(!todo.take_work(work, level)) // fetching from todo list
	    {
		if(!ask_delegation()) // request work from other team members
		    loop = false;
	    }
	    else
	    {
		resout(false);
		delete work;
		work = nullptr;
	    }
	else
	{
	    resout(false);
	    delete work;
	    work = nullptr;
	}
    }
    check_delegate(0);
}

	// inherited from parent team
void robot::delegate_work()
{
    assert(dispo_ptr != nullptr);
    assert(work != nullptr);

    deque<candidate>::iterator it = dispo_ptr->begin();
    travail *tmp = nullptr;

    while(it != dispo_ptr->end())
    {
	tmp = new travail(*work);
	if(tmp == nullptr)
	    throw E_MEM;
	try
	{
	    tmp->push_candidate(*it);
	    todo.add_work(tmp, level+1);
	}
	catch(...)
	{
	    delete tmp;
	    throw;
	}
	tmp = nullptr; // object now owned by todo
	++it;
    }
    dispo_ptr->clear(); // avoid doing twice the work
}

void robot::resout(bool init)
{
    deque<candidate> dispo;

    assert(work != nullptr);

	// verification qu'il reste encore des pieces a mettre

    if(work->all_placed())
    {
	time_t now = time(nullptr);

	solutions.push_back(work->current);
	cout.flush();
	return;
    }

    work->find_candidates(dispo);
    dispo_ptr = &dispo; // to have delegate_work() able to push work to the todo_list
    if(init)
	delegate_work();
    else
	check_delegate(level+1);
    dispo_ptr = nullptr; // just by precaution

	// pour chaque calque, insertion du calque, mise a jour des used/no_used, recursion, suppression du calque

    ++level;
    for(deque<candidate>::iterator ca = dispo.begin(); ca != dispo.end(); ca++)
    {
	if(work->push_candidate(*ca))
	{
	    resout(false);
	    work->pop_candidate(*ca);
	}
	    // else // ajout non effectue car en conflit avec une autre piece deja en place
    }
    --level;
}

bool robot::check_init_flag()
{
    bool ret = false;  // assuming we are not the first object awaken with a work available
    bool stop = false; // assuming we will not suspend

    flag_control.lock();
    try
    {
	if(init_flag)
	{
	    if(work == nullptr)
	    {
		++parked;
		stop = true; // we will suspend
	    }
	    else // we have work and we are the first
		ret = true; // we are the first object awaken with a work available
	}
    }
    catch(...)
    {
	flag_control.unlock();
	throw;
    }
    flag_control.unlock();

    if(stop)
	parking.lock();

    return ret;
}

void robot::set_init_flag_down()
{
    flag_control.lock();
    try
    {
	if(!init_flag)
	    E_BUG;
	init_flag = false;

	while(parked > 0)
	{
	    parking.unlock();
	    --parked;
	}
    }
    catch(...)
    {
	flag_control.unlock();
	throw;
    }
    flag_control.unlock();
}
