#ifndef CALQUE_SET_HPP
#define CALQUE_SET_HPP

#include <vector>

#include "pieces.hpp"
#include "calque.hpp"
#include "position.hpp"

    /// set of calques each containing a possible position if the gven piece in a board of given dimensions

class calque_set
{
public:
    calque_set(const piece & p, //< piece de base pour le jeu de calques
	       unsigned int x,  //< dimensions du tableau dans lequel doit etre placee la pice
	       unsigned int y); //< dimensions du tableau dans lequel doit etre placee la pice

    const calque & read_calque(unsigned int x) const;
    unsigned int get_total_num() const { return possible.size(); }
    unsigned int get_size_x() const { return dimx; };
    unsigned int get_size_y() const { return dimy; };
    unsigned char get_symbol() const { return symbol; };

    const vector<cellule> & get_busy_cellules_for_calque(unsigned int index) const;

private:
    unsigned char symbol;
    unsigned dimx, dimy;
    vector<calque> possible;             //< all possible calques for the given piece and given board dimensions

    bool update_calque_with(const piece & p, const position & pos, calque & result);
    void uniques();
};

#endif
