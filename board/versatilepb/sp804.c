/*
 * (C) Copyright 2003
 * Texas Instruments <www.ti.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002-2004
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * (C) Copyright 2004
 * Philippe Robin, ARM Ltd. <philippe.robin@arm.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+ 
 */

#include "sp804.h"
#include "timer.h"
#include "board.h"

#define TIMER_LOAD_VAL 0xffffffff

static void init_timer(int ch);
static void reset_timer(int ch);

typedef struct timer_data {
    uint tbl;
    uint lastinc;
} td_t;

static td_t gd[2];

#define timestamp(ch) gd[ch].tbl
#define lastdec(ch) gd[ch].lastinc

static int our_unit;

static void init_timer(int ch)
{
    /* 1st disable the Timer */
    TIMER_CTRL(ch) &= ~TIMER_ENABLE;

    /*
     * The Timer Control Register has one Undefined/Shouldn't Use Bit
     * So we should do read/modify/write Operation
     */

    /*
     * Timer Mode : Free Running
     * Interrupt : Disabled
     * Prescale : 8 Stage, Clk/256
     * Tmr Siz : 16 Bit Counter
     * Tmr in Wrapping Mode
     */
    TIMER_CTRL(ch) &= ~(TIMER_MODE_MSK | TIMER_INT_EN | TIMER_PRS_MSK | TIMER_SIZE_MSK | TIMER_ONE_SHT );

    /* init the timestamp and lastdec value */
    reset_timer(ch);
}

void init_timers(int unit)
{
    our_unit = unit;
    init_timer(0);
    init_timer(1);
}

void start_timer(int ch)
{
    TIMER_CTRL(ch) |= (TIMER_ENABLE | TIMER_PRS_8S);
}

uint stop_timer(int ch)
{
    uint now = TIMER_VAL(ch);       /* current tick value */

    TIMER_CTRL(ch) &= !(TIMER_ENABLE);
    return now;
}

void restart_timer(int ch)
{
    /* TODO */
    TIMER_CTRL(ch) |= (TIMER_ENABLE | TIMER_PRS_8S);
}

void reset_timer(int ch)
{
    /* reset time */
    lastdec(ch) = TIMER_VAL(ch);    /* capture current decrementer value time */
    timestamp(ch) = 0;              /* start "advancing" time stamp from 0 */
}

uint get_timer(int ch)
{
    uint now = TIMER_VAL(ch);       /* current tick value */

    if (lastdec(ch) >= now) {           /* normal mode (non roll) */
        /* normal mode */
        timestamp(ch) += lastdec(ch) - now; /* move stamp fordward with absoulte diff ticks */
    } else {                        /* we have overflow of the count down timer */
        /* nts = ts + ld + (TLV - now)
         * ts=old stamp, ld=time that passed before passing through -1
         * (TLV-now) amount of time after passing though -1
         * nts = new "advancing time stamp"...it could also roll and cause problems.
         */
        timestamp(ch) += lastdec(ch) + TIMER_LOAD_VAL - now;
    }
    lastdec(ch) = now;

    return timestamp(ch);
}

/* waits specified delay value and resets timestamp */
void udelay(int ch, unsigned long usec)
{
    uint tmo;
    uint endtime;
    signed long diff;

    if (usec >= 1000) {         /* if "big" number, spread normalization to seconds */
        tmo = usec / 1000;      /* start to normalize for usec to ticks per sec */
        tmo *= CONFIG_SYS_HZ;   /* find number of "ticks" to wait to achieve target */
        tmo /= 1000;            /* finish normalize. */
    } else {                    /* else small number, don't kill it prior to HZ multiply */
        tmo = usec * CONFIG_SYS_HZ;
        tmo /= (1000*1000);
    }

    endtime = get_timer(ch) + tmo;

    do {
        uint now = get_timer(ch);
        diff = endtime - now;
    } while (diff >= 0);
}
