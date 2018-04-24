#ifndef _2_MODE_H
#define _2_MODE_H

#include <FreeRTOS_ARM.h>
#include "_1_init.h"

#define standbyMode int8_t(0)
#define normalAscent int8_t(1)
#define normalDescent int8_t(2)
#define safeMode int8_t(3)
#define manual int8_t(4)

static int8_t state;

void initMode();
int getMode();
static void setMode(int8_t curMode);

#endif
