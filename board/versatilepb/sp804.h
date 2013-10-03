#ifndef SP804_H
#define SP804_H

/* ARM Dual-Timer Module (SP804) */

#define TIMER_BASE_ADDR(ch)     (0x101E2000+0x20*(ch))
#define TIMER0_BASE_ADDR        TIMER_BASE_ADDR(0)
#define TIMER1_BASE_ADDR        TIMER_BASE_ADDR(1)

#define TIMER_VAL(ch)           VPint(TIMER_BASE_ADDR(ch)+0x04)
#define TIMER_CTRL(ch)          VPint(TIMER_BASE_ADDR(ch)+0x08)

#define TIMER_ENABLE            (1 << 7)
#define TIMER_MODE_MSK          (1 << 6)
#define TIMER_MODE_FR           (0 << 6)
#define TIMER_MODE_PD           (1 << 6)

#define TIMER_INT_EN            (1 << 5)
#define TIMER_PRS_MSK           (3 << 2)
#define TIMER_PRS_8S            (1 << 3)
#define TIMER_SIZE_MSK          (1 << 2)
#define TIMER_ONE_SHT           (1 << 0)

#endif
