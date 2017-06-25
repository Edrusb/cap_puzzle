#include "team.hpp"
#include "erreurs.hpp"

#include <assert.h>

libthreadar::mutex team::instances_control;
list<team::member *> team::instances;
bool team::election_started = false;
bool team::work_provided = false;
libthreadar::barrier *team::synchro = nullptr;

team::team()
{
    instances_control.lock();
    try
    {
	member *me = new member();
	if(me == nullptr)
	    throw E_MEM;
	instances.push_back(me);
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

void team::set_team_size(unsigned int size_cohorte)
{
    instances_control.lock();
    try
    {
	list<member *>::iterator it = instances.begin();

	while(it != instances.end() && (*it != nullptr) && !(*it)->dying_or_dead)
	    ++it;

	if(it != instances.end())
	    E_BUG;

	if(synchro != nullptr)
	{
	    delete synchro;
	    synchro = nullptr;
	}

	if(size_cohorte > 1)
	{
	    synchro = new libthreadar::barrier(size_cohorte);
	    if(synchro == nullptr)
		throw E_MEM;
	}
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
	instances_control.lock();
	try
	{
	    list<member *>::iterator it = instances.begin();
	    member *winner_candidate = nullptr;
	    unsigned int lower_index = 0;

		// updating our fields

	    assert(me != nullptr);
	    if(!me->dying_or_dead)
		me->is_pending = true;
	    me->index = index;

		// looking for a winner

	    while(it != instances.end() && *it != nullptr && ((*it)->is_pending || (*it)->dying_or_dead))
	    {
		if((*it)->index < lower_index
		   || lower_index == 0)
		{
		    lower_index = (*it)->index;
		    winner_candidate = *it;
		}
		++it;
	    }

	    if(it == instances.end()) // the candidate is a real winner
	    {

		    // looking at election result

		election_started = false;
		if(lower_index == 0) // no work remain for the team
		    work_provided = false;
		else
		{
			// informing the winner

		    work_provided = true;
		    assert(winner_candidate != nullptr);
		    winner_candidate->winner = true;
		}

		    // awaking all team members except self unless I am a work requestor

		it = instances.begin();
		while(it != instances.end() && (*it) != nullptr)
		{
		    if((*it) != me             // awake all except me (I am already awaken)
		       && ((*it)->index > 0    // and do not awake work requestors if...
			   || !work_provided)) // ...there is a winner to awake them later on
			(*it)->pending_delegate.unlock();
		    ++it;
		}
		if(it != instances.end())
		    E_BUG;

			// go to sleep only if I am a requestor and a work will be provided
		if(me->index > 0 || !work_provided)
		    me->is_pending = false;
	    }
	}
	catch(...)
	{
	    instances_control.unlock();
	    throw;
	}
	instances_control.unlock();

	if(me->is_pending)
	    me->pending_delegate.lock();

	if(me->winner)
	{
	    delegate_work();

		// awaiking work requestors now that the work delegation is completed
	    instances_control.lock();
	    try
	    {
		list<member *>::iterator it = instances.begin();
		me->winner = false;
		work_provided = false;

		while(it != instances.end() && *it != nullptr)
		{
		    if((*it)->index == 0 && (*it)->is_pending)
		    {
			(*it)->index = 1; // to inform requestors that a work has been provided
			(*it)->pending_delegate.unlock();
		    }
		    ++it;
		}
	    }
	    catch(...)
	    {
		instances_control.unlock();
		throw;
	    }
	    instances_control.unlock();
	}

	me->is_pending = false;
    }
}

bool team::ask_delegation()
{
    instances_control.lock();
    try
    {
	if(!election_started)
	{
	    if(work_provided) // election has ended but winner has not yet provided work
	    {
		    // we add ourselve to the list of team member the winner will awake once
		    // work delegation will be done
		me->is_pending = true;
		me->index = 0;
		assert(me->winner == false);
	    }
	    else // no election started, or previous election fully completed
		election_started = true;
	}
    }
    catch(...)
    {
	instances_control.unlock();
	throw;
    }
    instances_control.unlock();

    if(me->is_pending)
    {
	me->pending_delegate.lock();
	me->is_pending = false;
    }
    else
	check_delegate(0); // we have no work to provide;

    return me->index > 0; // if there is a winner it will set it to 1 after work is provided
}

void team::inherited_run()
{
    me->dying_or_dead = false;
    if(synchro != nullptr)
	synchro->wait();
    inherited_inherited_run();
    me->dying_or_dead = true;
    check_delegate(0);
}
