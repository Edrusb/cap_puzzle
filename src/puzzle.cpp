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

typedef vector<board> resultat;

static travail init_resout(const list<piece> & pi,              //< list of piece to play with
			   unsigned int dimx,                   //< dimension of the board
			   unsigned int dimy,                   //< dimension of the board
			   vector<calque_set> & configuration); //< set of all valid calques derived from each piece
static void resout(travail & work,         //< what to solve
		   resultat & solutions,   //< record of all completed solution found so far
		   unsigned int level);    //< recursion level
static bool read_from_file(const char *name, list<piece> & disponibles, unsigned int & x, unsigned int & y);
static void display_time(const string & message);
static void display_solutions(const vector<board> & filtred_solutions);
static void debug_piece(const list<piece> & disponibles);
static void display_usage(const char *cmd);
static vector<board> remove_dup(const vector<board> & solutions);

static travail init_resout(const list<piece> & pi,
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

    return travail(dimx, dimy, configuration);
}

static void resout(travail & work,
		   resultat & solutions,
		   unsigned int level)
{
    vector<ensemble> dispo;

	// verification qu'il reste encore des pieces a mettre

    if(work.all_placed())
    {
	time_t now = time(nullptr);

	solutions.push_back(work.current);
	cout << "Solution " << solutions.size() << " trouvee le " << ctime(&now);
	cout.flush();
	return;
    }

    work.find_candidates(dispo);

	// pour chaque calque, insertion du calque, mise a jour des used/no_used, recursion, suppression du calque

    for(register unsigned int ca = 0 ; ca < dispo.size(); ca++)
    {
	if(work.push_candidate(dispo[ca]))
	{
	    resout(work, solutions, level+1); // recursion
	    work.pop_candidate(dispo[ca]);
	}
	    // else // ajout non effectue car en conflit avec une autre piece deja en place
    }
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

	cout  << "Lecture du fichier de configuration..." << endl;
	if(!read_from_file(argv[1], disponibles, dimx, dimy))
	    exit(2);
	else
	{
	    vector<board> solutions;
	    vector<board> filtred_solutions;
	    vector<calque_set> configuration;

	    cout << "Configuration correcte" << endl;
	    debug_piece(disponibles);

		// preparation de l'algorithme de recherche

	    display_time("creation des calques ...");
	    travail work = init_resout(disponibles, dimx, dimy, configuration);

		// recherche

	    display_time("Debut des recherches de solutions...");
	    resout(work, solutions, 1);

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
