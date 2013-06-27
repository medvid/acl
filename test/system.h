#ifndef SYSTEM_H
#define SYSTEM_H

#define FREQUENCY 12000000

#define UNIT_CYCLES 0
#define UNIT_MICROSECONDS 1
#define UNIT_MILLISECONDS 2

#include "acl.h"

void init_serial(void);
void put_str(char *p);
void put_char(int ch);
int  get_char(void);
void put_hex(unsigned int hex);
void put_int(unsigned int x);
void put_vect(unsigned int *p, unsigned int len);
void put_str(char *p);

void init_timers(int unit);
void restart_timer(int timer);
void start_timer(int timer);
uint stop_timer(int timer);

void put_val(char *p, uint value);
#endif
