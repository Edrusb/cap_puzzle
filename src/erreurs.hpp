#define E_SYNTAX 1
#define E_MEM 2
#define E_BUG { cout << "BUG : ligne " << __LINE__ << endl; throw 3; }
#define E_IMPOSSIBLE 3
