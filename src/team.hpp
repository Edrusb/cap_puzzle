#ifndef TEAM_HPP
#define TEAM_HPP

#include <libthreadar/libthreadar.hpp>

#include <list>

using namespace std;

    /// handle communication between a cohort of workers
    ///
    /// there is no chef in the team. worker regularly check
    /// that no other worker of the team is pending for work
    /// if so an election is done to select the worker that has
    /// the highest load, the elected worker push all its work into
    /// the todo_list and becomes a new pending worker.
    /// A pending worker fetches a work from the todo_list and solved
    /// it, upon termination it fetches another one from the todo_list
    /// and so on. When the todo_list is empty it triggers for an election
    /// for a working memeber to put its work into the todo_list.
    /// up to the time the election fails finding any active memeber

class team: public libthreadar::thread
{
public:
    team();
    ~team();

    void run_in_calling_thread() { inherited_run(); };

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

    enum etat
    {
	stopped,    //< stopped thread or not yet running
	newcomer,   //< thread activated during an election process
	running,    //< normal thread not involved in an election
	pending,    //< normal thread involved into an election process
	stopping,   //< thread dying during an election
	stopending, //< thread dyning involved into an election process
    };

    struct member
    {
	member()
	{
	    status = stopped;
	    index = 0;
	    winner = false;
	};

	etat status;           // thread status
	unsigned int index;    // zero if member has not work to provide
	bool winner;           // the winner has to provide work
    };

    member *me;

    void election_registering_lock_unlock(unsigned int index);
    void depouillement_lock();
    void ending_unlock();
    void first_breath();
    void last_breath();
	// moving as private the method inherited from libthreadar::thread
    void inherited_run();
    void cleanup_instances();

	// static fields and methods

    static libthreadar::mutex instances_control;
    static list<member *> instances;
    static bool election_started;
    static libthreadar::barrier* scrutin;
    static libthreadar::barrier* depouille;
    static unsigned int starters;
    static libthreadar::freezer pending_starters;

    static void resize_barrier(libthreadar::barrier* & val);

};

#endif
