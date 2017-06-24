#ifndef CELLULE_HPP
#define CELLULE_HPP

    /// defines coordinates in a 2 dimensions table

struct cellule
{
    cellule(unsigned int vx, unsigned int vy) { x = vx; y = vy; };

    unsigned int x;
    unsigned int y;
    bool operator < (const cellule & ref) const { return x < ref.x || (x == ref.x && y < ref.y); };
    bool operator == (const cellule & ref) const { return x == ref.x && y == ref.y; };
};

#endif
