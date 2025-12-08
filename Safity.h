#ifndef __Safity_H
#define __Safity_H

#include "stm32f10x.h"                  // D1evice header
#include "Motor.h"
#include "PID.h"
#include "GY25.h"

extern PID SpeedPID;
extern int key_date;
extern int16_t PWML, PWMR;

void Safity(void);
#endif
