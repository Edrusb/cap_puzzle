#ifndef ERREURS_HPP
#define ERREURS_HPP

#define E_SYNTAX 1
#define E_MEM 2
#define E_BUG if(true) { cout << "BUG : ligne " << __LINE__ << endl; throw 3; }
#define E_IMPOSSIBLE 4

#endif
