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

#include "calque_set.hpp"
#include "pieces.hpp"
#include "board.hpp"
#include "travail.hpp"
#include <list>
#include <unistd.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "resultat.hpp"
#include "robot.hpp"

static travail *init_resout(const list<piece> & pi,              //< list of piece to play with
			    unsigned int dimx,                   //< dimension of the board
			    unsigned int dimy,                   //< dimension of the board
			    vector<calque_set> & configuration); //< set of all valid calques derived from each piece
static bool read_from_file(const char *name, list<piece> & disponibles, unsigned int & x, unsigned int & y);
static void display_time(const string & message);
static void display_solutions(const resultat & filtred_solutions);
static void debug_piece(const list<piece> & disponibles);
static void display_usage(const char *cmd);
static resultat remove_dup(const resultat & solutions);

static travail *init_resout(const list<piece> & pi,
			    unsigned int dimx,
			    unsigned int dimy,
			    vector<calque_set> & configuration)
{
    list<piece>::const_iterator it = pi.begin();

    configuration.clear();
    while(it != pi.end())
    {
	cout << "pre-calculating layers for piece " << it->get_symbol() << endl;
	cout.flush();
	configuration.push_back(calque_set(*it, dimx, dimy));
	it++;
    }

    return new travail(dimx, dimy, configuration);
}


static bool read_from_file(const char *name, list<piece> & disponibles, unsigned int & x, unsigned int & y)
{

	// ouverture du fichier

    int fd = open(name, O_RDONLY);
    if(fd < 0)
    {
	perror("Reading configuration file");
	return false;
    }

	// lecture des dimensions du tableau

    read_next('[', fd);
    x = read_int_from(fd);
    read_next(',', fd);
    y = read_int_from(fd);
    read_next(']', fd);

	// lecture des pieces

    disponibles.clear();
    while(skip_to_next_info(fd))
    {
	piece tmp = fd;
	disponibles.push_back(tmp);
    }

	// fermeture du fichier

    close(fd);
    return true;
}

static void display_time(const string & message)
{
    time_t now = time(nullptr);
    string heure = ctime(&now);

    heure = heure.substr(0, heure.size()-1);
    cout << heure << " : " << message << endl;
    cout.flush();
}

static void display_solutions(const resultat & filtred_solutions)
{
    resultat::const_reverse_iterator it = filtred_solutions.rbegin();

    while(it != filtred_solutions.rend())
    {
	cout << endl;
	it->affiche();
	++it;
    }
    cout << endl;
}

static void debug_piece(const list<piece> & disponibles)
{
#ifndef NDEBUG

	// affichage des pieces utilisees

    cout << "------------" << endl;
    for(list<piece>::const_iterator it = disponibles.begin(); it != disponibles.end(); it++)
    {
	it->affiche();
	cout << "------------" << endl;
    }

#endif
}

static void display_usage(const char *cmd)
{
    cout << "usage: " << cmd << " <fichier> <num thread>" << endl << endl;
    cout << "\t syntaxe of expected file:" << endl;
    cout << "\t[dimx, dimy]" << endl;
    cout << "\t(x,y)[S] \n\t 1 0 0 ... \n\t 0 0 1 ... \n\t ... \n\n";
    cout << "\tdimx and dimy are the dimensions of the board\n";
    cout << "\tx and y are the dimensions of the piece \"S\" (its symbol),\n\tand the 0 and 1 that follow define its shape\n\t(0 = empty, 1 = full)\n";
    cout << "\tother pieces can be added and must be described following\n\tthe same rule, spaces can be inserted for better\n\treadability\n";
}

static resultat remove_dup(const resultat & solutions)
{
    resultat ret;
    resultat::const_iterator dst;
    resultat::const_iterator src;

    for(src = solutions.begin(); src != solutions.end(); ++src)
    {
	dst = src;
	++dst;
	while(dst != solutions.end() && !src->similaire(*dst))
	    ++dst;
	if(dst == solutions.end()) // dernier element de l'ensemble de solutions similaires
	    ret.push_back(*src);
    }

    return ret;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
	display_usage(argv[0]);
	exit(1);
    }

    try
    {
	list<piece> disponibles;
	unsigned int dimx, dimy;
	const unsigned int team_size = atoi(argv[2]);

	cout  << "Reading the configuration file..." << endl;
	if(!read_from_file(argv[1], disponibles, dimx, dimy))
	    exit(2);
	else
	{
	    resultat solutions;
	    resultat filtred_solutions;
	    vector<calque_set> configuration;
	    vector<robot> workers(team_size);

	    robot::reset_init_flag();
	    cout << "Configuration is correct" << endl;
	    debug_piece(disponibles);

		// preparation de l'algorithme de recherche

	    display_time("creating layers...");
	    travail *work = init_resout(disponibles, dimx, dimy, configuration);
	    if(work == nullptr)
		throw E_MEM;
	    workers[0].receive_work(work, 1);
	    work = nullptr;

		// recherche

	    cout << "Launching " << team_size << " thread" << (team_size > 1 ? "s" : "") << endl;
	    display_time("Searching for all solutions...");
	    vector<robot>::iterator it = workers.begin();
	    if(team_size > 1)
	    {
		while(it != workers.end())
		{
		    it->run();
		    ++it;
		}
	    }
	    else
	    {
		assert(it != workers.end());
		it->run_in_calling_thread();
	    }

		// waiting for all workers to die

	    it = workers.begin();
	    while(it != workers.end())
	    {
		it->join();
		++it;
	    }

		// fusion des solutions
	    it = workers.begin();
	    while(it != workers.end())
	    {
		solutions += it->get_solutions();
		++it;
	    }

		// suppression des solutions dupliquees

	    display_time("Removing duplicated solutions... ");
	    filtred_solutions = remove_dup(solutions);
	    display_time("Research completed");

	    // affichage des solutions

	    cout << solutions.size() << " solutions found (with duplications)" << endl;
	    cout << filtred_solutions.size() << " unique solution(s) found: " << endl;
	    display_solutions(filtred_solutions);

	    display_time("End of execution.");
	}
    }
    catch(int & e)
    {
	cout << "Exception : " << e << endl;
	return e;
    }
    catch(string & e)
    {
	cout << e << endl;
	return 3;
    }
    catch(...)
    {
	cout << "Unknown exception!!!" << endl;
	return 126;
    }

    return 0;
}
