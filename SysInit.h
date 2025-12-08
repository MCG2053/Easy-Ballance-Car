#ifndef __SysInit_H
#define __SysInit_H

#include "stm32f10x.h"                  // D1evice header
#include "Delay.h"	
#include "OLED.h"
#include "GY25.h"
#include "PWM.h"
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"
#include "Bluetooth.h"
#include "NRF24L01.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern PID AnglePID,SpeedPID,TurnPID;
extern int key_date;
extern float SpeedLeft, SpeedRight; 
extern float AveSpeed, DifSpeed;
extern float AvePWM, DifPWM;
extern int16_t PWML, PWMR;

void Sys_Init(void);
void Calibrator(void);
#endif
