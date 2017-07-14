#ifndef PIECES_HPP
#define PIECES_HPP

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

#include "erreurs.hpp"
#include "etat.hpp"
#include "espace.hpp"

using namespace std;

class piece : public espace<etat>
{
public:
    piece(unsigned char symbole, unsigned int x, unsigned y) : espace<etat>(x, y) { symbol = symbole; };
    piece(int fd);

    unsigned char get_symbol() const { return symbol; };

    bool operator < (const piece & ref) const { return get_size_x() < ref.get_size_x(); };

private:
    unsigned char symbol;
};

#endif
