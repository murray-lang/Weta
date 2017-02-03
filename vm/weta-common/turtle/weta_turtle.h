#ifndef __WETA_TURTLE_H__
#define __WETA_TURTLE_H__

#include "weta_platform.h"
#include "../hw/hw.h"

void weta_turtle_forward(Hardware* hw, int16_t mm);
void weta_turtle_backward(Hardware* hw, int16_t mm);
void weta_turtle_left(Hardware* hw, int16_t degrees);
void weta_turtle_right(Hardware* hw, int16_t degrees);
void weta_turtle_penup(void);
void weta_turtle_pendown(void);

#endif  // __WETA_TURTLE_H__