/*********************************************************************/
// cap_puzzle - a puzzle resolution program
// Copyright (C) 2002-2018 Denis Corbin
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// to contact the author : http://dar.linux.free.fr/email.html
/*********************************************************************/

#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#include "fd_tools.hpp"

static bool is_empty(char a)
{
    return a == ' ' || a == '\n' || a == '\t' || a == '\r';
}

bool skip_to_next_info(int fd)
{
    char a;
    bool once = false;
    unsigned int lu;

    while((lu = read(fd, &a, 1)) == 1 && is_empty(a))
	;

    if(lu == 1 && !is_empty(a))
	lseek(fd, -1, SEEK_CUR);

    return lu == 1;
}


void read_next(unsigned char x, int fd)
{
    unsigned char a;
    bool once = false;

    while(read(fd, &a, 1) == 1 && a != x)
	;
}

unsigned int read_int_from(int fd)
{
    const unsigned int len = 100;
    char buffer[len + 1];
    unsigned curs = 0;

    skip_to_next_info(fd);
    while(read(fd, buffer+curs, 1) == 1 && isdigit(buffer[curs]))
	curs++;

    if(lseek(fd, 0, SEEK_CUR) > 0)
	lseek(fd, -1, SEEK_CUR);

    buffer[curs] = '\0';

    if(curs == 0)
	return 0;
    else
	return atoi(buffer);
}


unsigned char read_char_from(int fd)
{
    unsigned char ret = '?';

    skip_to_next_info(fd);
    read(fd, &ret, 1);

    return ret;
}
