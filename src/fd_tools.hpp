#ifndef FD_TOOLS_HPP
#define FD_TOOLS_HPP

extern bool skip_to_next_info(int fd);
extern void read_next(unsigned char x, int fd);
extern unsigned int read_int_from(int fd);
extern unsigned char read_char_from(int fd);

#endif
