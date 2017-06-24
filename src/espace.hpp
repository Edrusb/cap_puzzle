#ifndef ESPACE_HPP
#define ESPACE_HPP

#include <assert.h>
#include <iostream>
#include <istream>

using namespace std;

#include "erreurs.hpp"
#include "fd_tools.hpp"


template <class T> class espace
{
public:
    espace(unsigned x, unsigned y);
    espace(int fd); // read x and y from a plain text file
    espace(const espace & ref) { copy_from(ref); };
    espace & operator = (const espace & ref) { detruit(); copy_from(ref); };
    ~espace() { detruit(); };
    bool operator == (const espace & ref) const;

    unsigned int get_size_x() const { return size_x; };
    unsigned int get_size_y() const { return size_y; };
    virtual inline T get_etat(unsigned int x, unsigned int y) const;
    virtual inline void set_etat(unsigned int x, unsigned int y, T val);
    virtual void clear(T val);
    virtual void change_val(const T val, T new_val);

    void affiche() const;

private:
    T **tableau;
    unsigned int size_x, size_y;

    void copy_from(const espace & ref);
    void detruit();
    void init(unsigned x, unsigned y);
};

template <class T> espace<T>::espace(unsigned x, unsigned y)
{
    init(x, y);
}

template <class T> espace<T>::espace(int fd)
{
    unsigned int x, y;

    read_next('(', fd);
    x = read_int_from(fd);
    read_next(',', fd);
    y = read_int_from(fd);
    read_next(')', fd);

    init(x, y);
}

template <class T> void espace<T>::detruit()
{
    if(size_x > 0 && size_y > 0)
    {
	for(unsigned int c = 0; c < size_x; c++)
	    delete [] tableau[c];
	delete [] tableau;
    }
}

template <class T>void espace<T>::copy_from(const espace & ref)
{
    init(ref.size_x, ref.size_y);
    for(register unsigned int x = 0; x < ref.size_x; x++)
	for(register unsigned int y = 0; y < ref.size_y; y++)
	    tableau[x][y] = ref.tableau[x][y];
}

template <class T> inline T espace<T>::get_etat(unsigned int x, unsigned int y) const
{
    assert(x < size_x && y < size_y);
    return tableau[x][y];
}

template <class T> inline void espace<T>::set_etat(unsigned int x, unsigned int y, T val)
{
    assert(x < size_x && y < size_y);
    tableau[x][y] = val;
}

template <class T> void espace<T>::clear(T val)
{
    for(register unsigned int x = 0; x < size_x; x++)
	for(register unsigned int y = 0; y < size_y; y++)
	    set_etat(x, y, val);
}

template <class T> void espace<T>::change_val(const T val, T new_val)
{
    for(register unsigned int x = 0; x < size_x; x++)
	for(register unsigned int y = 0; y < size_y; y++)
	    if(get_etat(x,y) == val)
		set_etat(x, y, new_val);
}

template <class T> void espace<T>::init(unsigned x, unsigned y)
{
    size_x = x;
    size_y = y;

    if(x > 0 && y > 0)
    {
	tableau = new T*[size_x];
	if(tableau == nullptr)
	    throw E_MEM;
	for(unsigned int c = 0; c < size_x; c++)
	    tableau[c] = nullptr;

	try
	{
	    for(unsigned int c = 0; c < size_x; c++)
	    {
		tableau[c] = new T[size_y];
		if(tableau[c] == nullptr)
		    throw E_MEM;
	    }
	}
	catch(...)
	{
	    for(unsigned int c = 0; c < size_x; c++)
		if(tableau[c] != nullptr)
		    delete[] tableau[c];
	    delete[] tableau;
	    throw;
	}
    }
    else
	tableau = nullptr;
}

template <class T> bool espace<T>::operator == (const espace & ref) const
{
    if(size_x != ref.size_x || size_y != ref.size_y)
	return false;
    for(register unsigned int x = 0; x < size_x; x++)
	for(register unsigned int y = 0; y < size_y; y++)
	    if(tableau[x][y] != ref.tableau[x][y])
		return false;
    return true;
}

template <class T> void espace<T>::affiche() const
{
    for(register unsigned int y = 0; y < get_size_y(); y++)
    {
	for(register unsigned int x = 0; x < get_size_x(); x++)
	    cout << (unsigned char)(tableau[x][y]);
	cout << endl;
    }
}

#endif
