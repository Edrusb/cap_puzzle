#ifndef TEAM_HPP
#define TEAM_HPP

#include <libthreadar/libthreadar.hpp>

#include <list>

using namespace std;

class team: public libthreadar::thread
{
public:
    team();
    ~team();

	/// all thread will start runing when all are awaken

	/// \note by default a thread runs ASAP which is equivalent to giving 1 for the team size, here
    static void set_team_size(unsigned int size_cohorte);

protected:
	/// a running team member has to regularly check that another worker need work

	/// \param[in] index is priority level to select the team member that will delegate work
	/// \note lower index are more elligible to be asked to delegate a work
	/// \note index must be strictly positive, zero is not a valid value
    void check_delegate(unsigned int index);

	/// defines how the current object provides work to a another team member

	/// \note when check_delegate() is run and if the current object is elected to deletate a work
	/// this method is called
    virtual void delegate_work() = 0;

	/// a team member that has no more work can call this to request work from another team member

	/// \return true if a work could be let available for delegation
    bool ask_delegation();

	/// the code to be run in a separated thread
    virtual void inherited_inherited_run() = 0;

private:
    struct member
    {
	member()
	{
	    is_pending = false;
	    index = 0;
	    winner = false;
	    pending_delegate.lock();
	    dying_or_dead = false;
	};

	bool is_pending;       // is or is about to be suspended on pending_delegate
	libthreadar::mutex pending_delegate;
	unsigned int index;    // zero if member has not work to provide
	bool winner;           // the winner has to provide work
	bool dying_or_dead;    // whether the thread is dead or about to die
    };

    member *me;

	// moving as private the method inherited from libthreadar::thread
    void inherited_run();


    static libthreadar::mutex instances_control;
    static list<member *> instances;
    static bool election_started;
    static bool work_provided;
    static libthreadar::barrier *synchro;
};

#endif
