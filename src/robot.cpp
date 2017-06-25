#include "robot.hpp"

todo_list robot::todo;

robot::robot()
{
    work = nullptr;
    level = 0;
    dispo_ptr = nullptr;
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
		resout();
		delete work;
		work = nullptr;
	    }
	else
	{
	    resout();
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

    vector<candidate>::iterator it = dispo_ptr->begin();
    travail *tmp = nullptr;

    while(it != dispo_ptr->end())
    {
	tmp = new travail(*work);
	if(tmp == nullptr)
	    throw E_MEM;
	tmp->push_candidate(*it);
	todo.add_work(tmp, level+1);
	tmp = nullptr; // object now owned by todo
	++it;
    }
    dispo_ptr->clear(); // avoid doing twice the work
}

void robot::resout()
{
    vector<candidate> dispo;

    assert(work != nullptr);

	// verification qu'il reste encore des pieces a mettre

    if(work->all_placed())
    {
	time_t now = time(nullptr);

	solutions.push_back(work->current);
	cout << "Solution " << solutions.size() << " trouvee le " << ctime(&now);
	cout.flush();
	return;
    }

    work->find_candidates(dispo);
    dispo_ptr = &dispo; // to have delegate_work() able to push work to the todo_list
    check_delegate(level+1);
    dispo_ptr = nullptr; // just by precaution

	// pour chaque calque, insertion du calque, mise a jour des used/no_used, recursion, suppression du calque

    ++level;
    for(register unsigned int ca = 0 ; ca < dispo.size(); ca++)
    {
	if(work->push_candidate(dispo[ca]))
	{
	    resout();
	    work->pop_candidate(dispo[ca]);
	}
	    // else // ajout non effectue car en conflit avec une autre piece deja en place
    }
    --level;
}
