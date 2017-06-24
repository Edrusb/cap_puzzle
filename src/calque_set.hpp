#ifndef CALQUE_SET_HPP
#define CALQUE_SET_HPP

#include <vector>

#include "pieces.hpp"
#include "calque.hpp"
#include "position.hpp"

class calque_set
{
public:
    calque_set(const piece & p, unsigned int x, unsigned int y);

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
