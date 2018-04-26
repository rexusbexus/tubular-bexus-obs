#ifndef _2_MODE_H
#define _2_MODE_H

#include <cstdint>

#define standbyMode uint8_t(0)
#define normalAscent uint8_t(1)
#define normalDescent uint8_t(2)
#define safeMode uint8_t(3)
#define manual uint8_t(4)

extern uint8_t state;

void initMode();
uint8_t getMode();
void setMode(uint8_t curMode);

#endif
