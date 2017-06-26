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
static void display_solutions(const vector<board> & filtred_solutions);
static void debug_piece(const list<piece> & disponibles);
static void display_usage(const char *cmd);
static vector<board> remove_dup(const vector<board> & solutions);
static void new_robots(vector<robot *> & ref, unsigned int num);
static void delete_robots(vector<robot *> & ref);

static travail *init_resout(const list<piece> & pi,
			    unsigned int dimx,
			    unsigned int dimy,
			    vector<calque_set> & configuration)
{
    list<piece>::const_iterator it = pi.begin();

    configuration.clear();
    while(it != pi.end())
    {
	cout << "Recherche des calques pour la piece " << it->get_symbol() << endl;
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

static void display_solutions(const vector<board> & filtred_solutions)
{
    vector<board>::const_reverse_iterator it = filtred_solutions.rbegin();

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
    cout << "usage: " << cmd << " <fichier>" << endl << endl;
    cout << "\t syntaxe du fichier attendu :" << endl;
    cout << "\t[dimx, dimy]" << endl;
    cout << "\t(x,y)[S] \n\t 1 0 0 ... \n\t 0 0 1 ... \n\t ... \n\n";
    cout << "\tdimx et dimy sont les dimensions du tableau\n";
    cout << "\tx et y sont les dimensions de la piece S son symbole et les 0 et 1 qui suivent sa forme (0 = vide, 1 = plein)\n";
    cout << "\td'autres pièces peuvent être décrites selon le même schema, on peut insérer des espace ou l'on veut\n";
}

static vector<board> remove_dup(const vector<board> & solutions)
{
    vector<board> ret;

    for(unsigned int src = 0; src < solutions.size() ; src++)
    {
	unsigned int dst = src+1;
	while(dst < solutions.size() && !solutions[src].similaire(solutions[dst]))
	    dst++;
	if(dst >= solutions.size()) // dernier element de l'ensemble de solutions similaires
	    ret.push_back(solutions[src]);
    }

    return ret;
}

static void new_robots(vector<robot *> & ref, unsigned int num)
{
    robot *tmp = nullptr;

    for(unsigned int i = 0 ; i < num ; ++i)
    {
	tmp = new (nothrow) robot();
	if(tmp == nullptr)
	    throw E_MEM;
	ref.push_back(tmp);
    }
}

static void delete_robots(vector<robot *> & ref)
{
    while(ref.size() > 0)
    {
	if(ref.back() != nullptr)
	    delete ref.back();
	ref.pop_back();
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
	display_usage(argv[0]);
	exit(1);
    }

    try
    {
	list<piece> disponibles;
	unsigned int dimx, dimy;
	const unsigned int team_size = 1;

	cout  << "Lecture du fichier de configuration..." << endl;
	if(!read_from_file(argv[1], disponibles, dimx, dimy))
	    exit(2);
	else
	{
	    resultat solutions;
	    resultat filtred_solutions;
	    vector<calque_set> configuration;
	    vector<robot *> workers;

	    try
	    {
		new_robots(workers, team_size);

		cout << "Configuration correcte" << endl;
		debug_piece(disponibles);

		    // preparation de l'algorithme de recherche

		display_time("creation des calques ...");
		travail *work = init_resout(disponibles, dimx, dimy, configuration);
		if(work == nullptr)
		    throw E_MEM;
		assert(workers[0] != nullptr);
		workers[0]->receive_work(work, 1);
		work = nullptr;

		    // recherche

		display_time("Debut des recherches de solutions...");
		vector<robot *>::iterator it = workers.begin();
		while(it != workers.end())
		{
		    assert(*it != nullptr);
		    (*it)->run();
		    ++it;
		}

		    // waiting for all workers to die

		it = workers.begin();
		while(it != workers.end())
		{
		    assert(*it != nullptr);
		    (*it)->join();
		    ++it;
		}

		    // fusion des solutions
		it = workers.begin();
		while(it != workers.end())
		{
		    assert(*it != nullptr);
		    solutions += (*it)->get_solutions();
		    ++it;
		}
	    }
	    catch(...)
	    {
		delete_robots(workers);
		throw;
	    }
	    delete_robots(workers);

		// suppression des solutions dupliquees

	    display_time("Suppression des solutions dupliquees ... ");
	    filtred_solutions = remove_dup(solutions);
	    display_time("Recherche terminee");

	    // affichage des solutions

	    cout << solutions.size() << " solutions trouvees (avec duplications)" << endl;
	    cout << filtred_solutions.size() << " solutions uniques trouvees : " << endl;
	    display_solutions(filtred_solutions);

	    display_time("Fin du programme.");
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
	cout << "Exception inconnue !!!" << endl;
	return 126;
    }

    return 0;
}
