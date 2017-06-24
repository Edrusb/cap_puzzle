#include "calque_set.hpp"
#include "pieces.hpp"
#include "board.hpp"
#include <list>
#include <unistd.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

struct in_place
{
    unsigned int calque_set_num;
    unsigned int calque_num;
};


void init_resoud2(list<piece> pi, unsigned int dimx, unsigned int dimy,  vector<calque_set> & configuration, vector<unsigned int> & avail, vector<board> & solutions)
{
    list<piece>::iterator it = pi.begin();

    configuration.clear();
    while(it != pi.end())
    {
	cout << "Recherche des calques pour la piece " << it->get_symbol() << endl;
	cout.flush();
	configuration.push_back(calque_set(*it, dimx, dimy));
	it++;
    }

    avail.clear();
    for(unsigned int i = 0; i < configuration.size() ; i++)
	avail.push_back(i);

    solutions.clear();
}

struct ensemble
{
    unsigned int calque_set_index;
    const vector <cellule> *busy_cellules;
    unsigned char symbol;
};

void resoud2(board & current, const vector<calque_set> & configuration, vector<unsigned int> & avail, vector<board> & solutions, unsigned int level)
{
    signed int free_x, free_y;
    vector<ensemble> dispo;
    ensemble tmp;

	// verification qu'il reste encore des pièces à mettre

    if(avail.size() == 0)
    {
	time_t now = time(nullptr);
	solutions.push_back(current);
	cout << "Solution " << solutions.size() << " trouvee le " << ctime(&now);
	cout.flush();
	return;
    }


	// recherche d'une place libre (une seule suffit) à remplir dans le tableau en cours (si plus de place libre -> pas de solution au problème)

    if(!current.find_free_space(free_x, free_y))
	throw E_IMPOSSIBLE; // tout l'espace est utilise pourtant il reste des pieces a ajouter !



	// recherche des calques disponibles pour les pièces non utilisées

    for(register unsigned int piid = 0; piid < avail.size(); piid++)
    {
	assert(avail[piid] < configuration.size());
	const calque_set & current_calque_set = configuration[avail[piid]];
	unsigned int num_calque = current_calque_set.get_total_num();

	for(unsigned int caid = 0; caid < num_calque; caid++)
	    if(current_calque_set.read_calque(caid).get_etat(free_x, free_y) == plein)
	    {
		tmp.calque_set_index = avail[piid];
		tmp.busy_cellules = & current_calque_set.read_calque(caid).get_busy_cellules();
		tmp.symbol = current_calque_set.get_symbol();
		dispo.push_back(tmp);
	    }
    }


	// pour chaque calque, insertion du calque, mise à jour des used/no_used, recursion, suppression du calque

    for(register unsigned int ca = 0 ; ca < dispo.size(); ca++)
    {
	    // ajout de la pièce au tableau courant
	if(current.add(*dispo[ca].busy_cellules, dispo[ca].symbol))
	{
	    vector<unsigned int>::iterator it = find(avail.begin(), avail.end(), dispo[ca].calque_set_index);

#ifndef NDEBUG
	    if(level <= 3)
	    {
		time_t now = time(nullptr);
		cout << "[level " << level << "]  " << ca+1 << " / " << dispo.size() << " : " << ctime(&now) << endl;
	    }
#endif
	    if(it == avail.end())
		E_BUG;
	    avail.erase(it);
	    resoud2(current, configuration, avail, solutions, level+1); // recursion
	    avail.push_back(dispo[ca].calque_set_index);
	    current.remove(*dispo[ca].busy_cellules, dispo[ca].symbol);
	}
	    // else // ajout non effectue car en conflit avec une autre piece deja en place
    }
}


bool read_from_file(const char *name, list<piece> & disponibles, unsigned int & x, unsigned int & y)
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

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
	cout << "usage: " << argv[0] << " <fichier>" << endl << endl;
	cout << "\t syntaxe du fichier attendu :" << endl;
	cout << "\t[dimx, dimy]" << endl;
	cout << "\t(x,y)[S] \n\t 1 0 0 ... \n\t 0 0 1 ... \n\t ... \n\n";
	cout << "\tdimx et dimy sont les dimensions du tableau\n";
	cout << "\tx et y sont les dimensions de la piece S son symbole et les 0 et 1 qui suivent sa forme (0 = vide, 1 = plein)\n";
	cout << "\td'autres pièces peuvent être décrites selon le même schema, on peut insérer des espace ou l'on veut\n";
	exit(1);
    }

    try
    {
	list<piece> disponibles;
	vector<board> solutions;
	vector<board> filtred_solutions;
	unsigned int dimx, dimy;

	    // lecture du fichier de configuration (-> disponibles, dimx, dimy, board)

	cout  << "Lecture du fichier de configuration..." << endl;
	cout.flush();
	if(!read_from_file(argv[1], disponibles, dimx, dimy))
	    exit(2);
	board socle(dimx, dimy, '.');
	cout << "Configuration correcte" << endl;

#ifndef NDEBUG

	    // affichage des pieces utilisees

	cout << "------------" << endl;
	for(list<piece>::iterator it = disponibles.begin(); it != disponibles.end(); it++)
	{
	    it->affiche();
	    cout << "------------" << endl;
	}

#endif

	    // algorithme de recherche

	time_t now = time(nullptr);
	cout << "Debut le " << ctime(&now);
	cout << "creation des calques ..." << endl;
	cout.flush();
	vector<calque_set> configuration;
	vector<unsigned int> avail;
	init_resoud2(disponibles, dimx, dimy, configuration, avail, solutions);

	now = time(nullptr);
	cout << "Debut des recherches de solutions le " << ctime(&now);
	cout.flush();
	resoud2(socle, configuration, avail, solutions, 1);
	now = time(nullptr);
	cout << "Fin des recherches de solutions le " << ctime(&now);

	cout << "Suppression des solutions dupliquees ... " << endl;
	cout.flush();

	filtred_solutions.clear();
	for(unsigned int src = 0; src < solutions.size() ; src++)
	{
	    unsigned int dst = src+1;
	    while(dst < solutions.size() && !solutions[src].similaire(solutions[dst]))
		dst++;
	    if(dst >= solutions.size()) // dernier element de l'ensemble de solutions similaires
		filtred_solutions.push_back(solutions[src]);
	}
	now = time(nullptr);
	cout << "Fin des la suppression de solutuon dupliquees : " << ctime(&now);

	    // affichage des solutions

	cout << solutions.size() << " solutions trouvees (avec duplications)" << endl;
	cout << filtred_solutions.size() << " solutions trouvees : " << endl;
	while(filtred_solutions.size() > 0)
	{
	    cout << endl;
	    filtred_solutions.back().affiche();
	    filtred_solutions.pop_back();
	}
	cout << endl;

	now = time(nullptr);
	cout << "Fin du programme " << ctime(&now);

	return 0;
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
}
