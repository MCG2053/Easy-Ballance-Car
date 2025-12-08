#ifndef __Bluetooth_H
#define __Bluetooth_H

//头文件
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

// 外部变量声明
extern char Serial_RxPacket[];

// 函数声明
void Bluetooth_Init(void); //蓝牙串口初始化
uint8_t Bluetooth_GetRxFlag(void); //蓝牙串口标志位获取

#endif
