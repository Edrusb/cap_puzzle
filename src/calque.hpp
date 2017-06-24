#ifndef CALQUE_HPP
#define CALQUE_HPP

#include <vector>
#include "espace.hpp"
#include "etat.hpp"

using namespace std;

struct cellule
{
    cellule(unsigned int vx, unsigned int vy) { x = vx; y = vy; };

    unsigned int x;
    unsigned int y;
    bool operator < (const cellule & ref) const { return x < ref.x || (x == ref.x && y < ref.y); };
    bool operator == (const cellule & ref) const { return x == ref.x && y == ref.y; };
};

class calque : public espace<etat>
{
public:
    calque(unsigned int x, unsigned int y) : espace<etat>(x, y) { busy.clear(); };
    calque(const calque & ref): espace<etat>(ref) { busy = ref.busy; };
    calque(calque && ref): espace<etat>(ref) { busy = move(ref.busy); };
    calque & operator = (const calque & ref);
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
