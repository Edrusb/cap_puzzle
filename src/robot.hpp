#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "travail.hpp"
#include "team.hpp"
#include "resultat.hpp"
#include "todo_list.hpp"
#include <list>

class robot: public team
{
public:
    robot();
    ~robot() { if(work != nullptr) delete work; };

    void receive_work(travail *w, unsigned int index);
    const resultat & get_solutions() const { return solutions; };

protected:

	// inherited from parent team
    void delegate_work();
    void inherited_inherited_run();


private:
    travail *work;
    unsigned int level;
    vector<candidate> *dispo_ptr;
    resultat solutions;

    void resout(); // look for solution based on current "work"

    static todo_list todo; // thread safe object
};

#endif
