#ifndef TIMER_H
#define TIMER_H

/* Timer interface */

#include "acl.h"

#define UNIT_CYCLES 0
#define UNIT_MICROSECONDS 1
#define UNIT_MILLISECONDS 2

void init_timers(int unit);

void start_timer(int ch);

void restart_timer(int ch);

uint stop_timer(int ch);

#endif
