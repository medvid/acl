#ifndef BOARD_H
#define BOARD_H

#include "acl.h"

#define CONFIG_BOARD_NAME       "versatilepb"

/* serial */

#define CONFIG_PL011_CLOCK      24000000
#define CONFIG_BAUDRATE         38400
#define CONFIG_CONS_INDEX       0

/* timer */
#define CONFIG_SYS_HZ           (1000000 / 256)

#endif
