#ifndef __Control_H
#define __Control_H

#include "stm32f10x.h"                  // D1evice header
#include "Delay.h"
#include "OLED.h"
#include "GY25.h"
#include "PID.h"
#include "Menu.h"
#include "NRF24L01.h"
#include "Bluetooth.h"
#include "string.h"
#include <stdlib.h>

extern uint8_t BreceiveFlag;

void NRF_Control(void);
void NRF_Debug(void);
void BT_Control(void);
#endif
