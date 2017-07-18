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
