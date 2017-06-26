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

    static void reset_init_flag();

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

    void resout(bool init);    // look for solution based on current "work"
    bool check_init_flag();    // return true if we are the first robot with work
    void set_init_flag_down(); //

    static todo_list todo;     // thread safe object

    static libthreadar::mutex flag_control;  // controls the access to init_flag
    static bool init_flag;                   // set to false by the first robot
    static unsigned int parked;              // number of robot waiting or expected to wait on parking
    static libthreadar::freezer parking;     // robots starting without work go suspending on this if init_flag is true

};

#endif
