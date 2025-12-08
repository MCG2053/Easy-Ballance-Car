#ifndef __Menu_H
#define __Menu_H
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PID.h"
#include "NRF24L01.h"
#include "Bluetooth.h"
#include "GY25.h"
#include "Encoder.h"
#include "Control.h"
#include "string.h"

extern PID AnglePID,SpeedPID,TurnPID;
extern int key_date;
extern int16_t PWML, PWMR;
extern uint8_t Oled_Flag;

void Oled_Menu(void);
#endif
