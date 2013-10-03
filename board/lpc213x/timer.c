#include "timer.h"
#include "board.h"

static int our_unit;

void init_timers(int unit)
{
    int tmp;

    our_unit = unit;
    if (unit == UNIT_CYCLES) {
        tmp = 1;
    } else {
        if (unit == UNIT_MICROSECONDS) tmp = CONFIG_FREQUENCY / 1000000;
        if (unit == UNIT_MILLISECONDS) tmp = CONFIG_FREQUENCY / 1000;
        tmp *= (PLLSTAT & 0x1F) + 1;
        if ((VPBDIV & 3) == 0) tmp >>= 2;
        if ((VPBDIV & 3) == 2) tmp >>= 1;
    }

    T1TCR = 0; T1PC = 0; T1TC = 0; T1PR = tmp - 1;

    T0TCR = 0; T0PC = 0; T0TC = 0; T0PR = tmp - 1;
}

void start_timer(int ch)
{
    if (ch) {
        T1TCR = 1;
    } else {
        T0TCR = 1;
    }
}

void restart_timer(int ch)
{
    if (ch) {
        T1TCR = 0; T1PC = 0; T1TC = 0; T1TCR = 1;
    } else {
        T0TCR = 0; T0PC = 0; T0TC = 0; T0TCR = 1;
    }
}

uint stop_timer(int ch)
{
    int tmp;

    if (ch) {
        T1TCR = 0; tmp = T1TC;
    } else {
        T0TCR = 0; tmp = T0TC;
    }

    if (our_unit == UNIT_CYCLES) {
        if ((VPBDIV & 3) == 0) tmp *= 4;
        if ((VPBDIV & 3) == 2) tmp *= 2;
    }
    return tmp;
}
