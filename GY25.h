#ifndef __GY25_H
#define __GY25_H
#include "stm32f10x.h"                  // Device header

extern int32_t pitch,roll,yaw; //俯仰角，翻滚角，航向角
extern int32_t Read_State;
extern uint8_t DataBuf[8];

//GY25模式枚举结构体
enum SY25_Mode{
	GY25Mode_Inquire_MAN=0xA551,
	GY25Mode_Inquire_AUOT=0xA552,
	GY25Mode_Inquire_AUOT_ASCII=0xA553,
	GY25Mode_SET_Roll_and_Pitch=0xA554,
	GY25Mode_SET_Yaw=0xA555
};

//函数声明
void GY25_Init(void); //初始化
void GY25_SetMode(enum SY25_Mode mode); //查询模式设置
void GY25_Usart_SendByte(uint8_t Byte);
void GY25_ClearBuffer(void);
#endif
