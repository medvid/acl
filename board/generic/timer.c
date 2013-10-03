#include "timer.h"
#include "board.h"

static int our_unit;

void init_timers(int unit)
{
    our_unit = unit;
}

void start_timer(int ch)
{
    if (ch) {
        return;
    } else {
        return;
    }
}

void restart_timer(int ch)
{
    if (ch) {
        return;
    } else {
        return;
    }
}

uint stop_timer(int ch)
{
    if (ch) {
        return 0;
    } else {
        return 0;
    }
}
