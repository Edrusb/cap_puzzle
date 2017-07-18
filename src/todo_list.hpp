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
