#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	PWM_Init();
}

void left_positive(void) //右轮正转
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);
}

void left_negative(void) //右轮反转
{
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}

void right_positive(void) //左轮正传
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

void right_negative(void) //左轮反转
{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
void Motor_SetPWM(int16_t LeftDuty, int16_t RightDuty)
{
    // 左电机控制
    if (LeftDuty >= 0) {
        left_positive();
        PWM_SetCompare1(LeftDuty);
    } else {
        left_negative();
        PWM_SetCompare1(-LeftDuty);
    }
    
    // 右电机控制  
    if (RightDuty >= 0) {
        right_positive();
        PWM_SetCompare2(RightDuty);
    } else {
        right_negative();
        PWM_SetCompare2(-RightDuty);
    }
}
