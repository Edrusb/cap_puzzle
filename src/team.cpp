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

#include "team.hpp"
#include "erreurs.hpp"

#include <assert.h>

libthreadar::mutex team::instances_control;
list<team::member *> team::instances;
bool team::election_started = false;
libthreadar::barrier* team::scrutin = nullptr;
libthreadar::barrier* team::depouille = nullptr;
unsigned int team::starters = 0;
libthreadar::freezer team::pending_starters;

team::team()
{
    instances_control.lock();
    try
    {
	me = new member();
	if(me == nullptr)
	    throw E_MEM;
	instances.push_back(me);
	me->status = stopped;
    }
    catch(...)
    {
	instances_control.unlock();
	throw;
    }
    instances_control.unlock();
}

team::~team()
{
    instances_control.lock();
    try
    {
	list<member *>::iterator it = instances.begin();
	while(it != instances.end() && *it != me)
	    ++it;

	if(it == instances.end())
	    E_BUG;
	delete me;
	me = nullptr;
	instances.erase(it);
    }
    catch(...)
    {
	instances_control.unlock();
	throw;
    }
    instances_control.unlock();
}


void team::check_delegate(unsigned int index)
{
    if(election_started) // reading a value out of mutex control, yes, for efficiency
    {
	election_registering_lock_unlock(index);

	assert(scrutin != nullptr);
	scrutin->wait();  /// BARRIER

	depouillement_lock();

	assert(depouille != nullptr);
	depouille->wait(); /// BARRIER for the work to be provided

	ending_unlock();
    }
}

bool team::ask_delegation()
{
    instances_control.lock();
    election_started = true;
    instances_control.unlock();

    check_delegate(0); // we have no work to provide, using 0 as index

    return me->index > 0; // if there is a winner it will set index to 1 after work has been provided
}

void team::election_registering_lock_unlock(unsigned int index)
{
    instances_control.lock();
    try
    {
	list<member *>::iterator it = instances.begin();
	member *winner_candidate = nullptr;
	unsigned int lower_index = 0;

	    // updating our fields

	assert(me != nullptr);
	assert(me->status == running || me->status == stopping);
	switch(me->status)
	{
	case newcomer:
	    E_BUG;
	case running:
	    me->status = pending;
	    break;
	case pending:
	    E_BUG;
	case stopping:
	    me->status = stopending;
	    break;
	case stopending:
	    E_BUG;
	case stopped:
	    E_BUG;
	default:
	    E_BUG;
	}
	me->index = index;

	    // looking for a winner

	while(it != instances.end()
	      && *it != nullptr
	      && (*it)->status != running
	      && (*it)->status != stopping)
	{
	    switch((*it)->status)
	    {
	    case newcomer:
	    case stopped:
		break;
	    case running:
	    case stopping:
		E_BUG;
	    case pending:
	    case stopending:
		if((*it)->index < lower_index
		   || lower_index == 0)
		{
		    lower_index = (*it)->index;
		    winner_candidate = *it;
		}
		break;
	    default:
		E_BUG;
	    }
	    ++it;
	}

	if(it == instances.end()) // the candidate is a real winner
	{

		// looking at election result

	    assert(winner_candidate != nullptr);
	    winner_candidate->winner = true;
	}
    }
    catch(...)
    {
	instances_control.unlock();
	throw;
    }
    instances_control.unlock();
}

void team::depouillement_lock()
{
    if(me->winner)
    {
	instances_control.lock();
	try
	{
	    election_started = false;

	    if(me->index > 0) // we can provide a work
	    {
		list<member *>::iterator it = instances.begin();

		    // inherited
		delegate_work();

		    // and informing requesting thread a work was provided
		while(it != instances.end())
		{
		    if(*it != nullptr &&
		       (*it)->index == 0) // this one was a work requestor
			(*it)->index = 1; // signaling that a work has been provided
		    ++it;
		}
	    }

		// set stopping -> stopped
		// newcomers -> running
		// pending -> running
	    cleanup_instances();

		// awaking the newcomers
	    assert(pending_starters.get_value() == -starters);
	    starters = 0;
	    while(pending_starters.get_value() < 0)
		pending_starters.unlock();

		// resizing scrutin
	    resize_barrier(scrutin);
	}
	catch(...)
	{
	    instances_control.unlock();
	    throw;
	}

	    // we do not release instances_control yet!
    }
}

void team::ending_unlock()
{
    if(me->winner)
    {
	try
	{
	    resize_barrier(depouille);
	    me->winner = false;
	}
	catch(...)
	{
	    instances_control.unlock();
	    throw;
	}
	instances_control.unlock();
    }
}

void team::first_breath()
{
    bool barriers_ok = false;

    instances_control.lock();
    try
    {
	if(!election_started)
	{
	    me->status = running;
	    resize_barrier(scrutin);
	    resize_barrier(depouille);
	    barriers_ok = true;
	}
	else // must wait for the election to end for the barrier to be resized
	{
	    ++starters;
	    me->status = newcomer;
	}
    }
    catch(...)
    {
	instances_control.unlock();
	throw;
    }
    instances_control.unlock();

    if(!barriers_ok)
	pending_starters.lock();
}

void team::last_breath()
{
    bool go_check_delegate = false;

    instances_control.lock();
    try
    {
	if(election_started)
	{
	    me->status = stopping;
	    go_check_delegate = true;
	}
	else
	{
	    me->status = stopped;
	    resize_barrier(scrutin);
	    resize_barrier(depouille);
	}
    }
    catch(...)
    {
	instances_control.unlock();
	throw;
    }
    instances_control.unlock();

    if(go_check_delegate)
	check_delegate(0);
}

void team::inherited_run()
{
    assert(me != nullptr);
    first_breath();
    inherited_inherited_run();
    last_breath();
}

void team::cleanup_instances()
{
    list<member *>::iterator it = instances.begin();

    while(it != instances.end())
    {
	assert(*it != nullptr);
	switch((*it)->status)
	{
	case newcomer:
	case pending:
	    (*it)->status = running;
	    break;
	case running:
	    E_BUG;
	case stopping:
	    E_BUG;
	case stopending:
	    (*it)->status = stopped;
	    break;
	case stopped:
	    break;
	default:
	    E_BUG;
	}
	++it;
    }
}

void team::resize_barrier(libthreadar::barrier* & val)
{
    unsigned int size = 0;
    list<member *>::iterator it = instances.begin();

	// calculating the new size of the barrier

    while(it != instances.end())
    {
	assert(*it != nullptr);
	switch((*it)->status)
	{
	case newcomer:
	case running:
	case pending:
	    ++size;
	    break;
	case stopping:
	case stopending:
	case stopped:
	    break;
	default:
	    E_BUG;
	}
	++it;
    }

    if(val != nullptr && val->get_count() != size)
    {
	delete val;
	val = nullptr;
    }

    if(val == nullptr && size > 0)
    {
	val = new libthreadar::barrier(size);
	if(val == nullptr)
	    throw E_MEM;
    }
}
