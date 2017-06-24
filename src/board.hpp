#include "espace.hpp"
#include "calque.hpp"
#include "erreurs.hpp"
#include "rotation.hpp"

class board : public espace<unsigned char>
{
public:
    board(unsigned x, unsigned y, unsigned char init) : espace<unsigned char>(x, y), empty(init) { clear(init); };
    board(const board & ref): espace<unsigned char>(ref) { empty = ref.empty; };
    board(board && ref): espace(ref) { empty = move(ref.empty); };
    board & operator = (const board & ref);

    bool operator == (const board & ref) const { return espace<unsigned char>::operator ==(ref); };
    bool add(const vector<cellule> & ou, unsigned char quoi);
    void remove(const vector<cellule> & ou, unsigned char quoi);
    bool find_free_space(signed int & x, signed int & y) const;

    bool similaire(const board & ref) const; // retourne true si même board par rotation et/ou retournement (pile/face)

private:
    unsigned char empty;

    board transform(rotation rot, retournement ret) const;
};
