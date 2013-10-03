#ifndef STDIO_H
#define STDIO_H

#include "acl.h"

void init_serial(void);
void put_str(char *p);
void put_char(char c);
int  get_char(void);
void put_hex(unsigned int hex);
void put_int(unsigned int x);
void put_vect(unsigned int *p, unsigned int len);
void put_str(char *p);
void put_val(char *p, uint value);

#endif
