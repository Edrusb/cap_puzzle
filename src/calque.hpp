#ifndef CALQUE_HPP
#define CALQUE_HPP

#include <vector>
#include "espace.hpp"
#include "etat.hpp"
#include "cellule.hpp"

using namespace std;

    /// calque is a binary espace (empty/busy) which also provides an optimized way to get the list of busy cells

class calque : public espace<etat>
{
public:
    calque(unsigned int x, unsigned int y) : espace<etat>(x, y) { busy.clear(); };

    bool operator == (const calque & ref);
    const vector<cellule> & get_busy_cellules() const { return busy; };
    void clear() { espace<etat>::clear(vide); busy.clear(); };

	// surcharge des methode de espace

    void set_etat(unsigned int x, unsigned int y, etat val);
    void clear(etat val);
    void change_val(const etat val, etat new_val);

private:
    vector<cellule> busy; // list is sorted

    void update_busy_cellules();
    void sort();
};

#endif
