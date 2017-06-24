#ifndef ROTATION_HPP
#define ROTATION_HPP

enum rotation
{
    nord = 'N',    //< pas de rotation
    est = 'E',     //< rotation 90° dans le sens des aiguilles d'une montre
    sud = 'S',     //< demi tour
    ouest ='O'     //< rotation 90% dans le sens inverse des aiguilles d'une montre
};

enum retournement
{
    pile = 'p',    //< pas d'inversion de face
    face = 'f'     //< retournement de face
};

extern bool suivant(rotation & arg);

#endif
