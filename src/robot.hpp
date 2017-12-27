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

#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "travail.hpp"
#include "team.hpp"
#include "resultat.hpp"
#include "todo_list.hpp"
#include <deque>

    /// implements the work process of a worker in the team

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
    deque<candidate> *dispo_ptr;
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
