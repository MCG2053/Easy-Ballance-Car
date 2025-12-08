#include "Menu.h"

uint8_t Oled_Flag='A';

void Oled_Menu(void)
{
    if(BreceiveFlag)
    {
        if(strcmp(Serial_RxPacket,"A")==0)Oled_Flag = 'A';
        if(strcmp(Serial_RxPacket,"B")==0)Oled_Flag = 'B';
        if(strcmp(Serial_RxPacket,"C")==0)Oled_Flag = 'C';
        if(strcmp(Serial_RxPacket,"D")==0)Oled_Flag = 'D';
        if(strcmp(Serial_RxPacket,"E")==0)Oled_Flag = 'E';
    }
    switch(Oled_Flag)
    {
        case 'A'://显示俯仰、翻滚、转向角度
                OLED_Clear();
                OLED_ShowString(0,0,"  Angle  Page  ",OLED_8X16);
                OLED_ShowString(0,16,"Pitch",OLED_8X16);
                OLED_ShowString(0,32,"Row",OLED_8X16);
                OLED_ShowString(0,48,"Yaw",OLED_8X16);
                OLED_ShowSignedNum(48,16,pitch/100,3,OLED_8X16);
                OLED_ShowSignedNum(48,32,roll/100,3,OLED_8X16);	
                OLED_ShowSignedNum(48,48,yaw/100,3,OLED_8X16);
                break;
        case 'B'://显示左右轮PWM以及电机速度
                OLED_Clear();
                OLED_ShowString(0,0,"  Motor  Page  ",OLED_8X16);
                OLED_ShowString(0,16,"PL",OLED_8X16);
                OLED_ShowString(0,32,"PR",OLED_8X16);
                OLED_ShowString(72,16,"SL",OLED_8X16);
                OLED_ShowString(72,32,"SR",OLED_8X16);
                OLED_ShowSignedNum(16,16,PWML,4,OLED_8X16);
                OLED_ShowSignedNum(16,32,PWMR,4,OLED_8X16);
                OLED_ShowSignedNum(88,16,-Encoder_Get(1),2,OLED_8X16);
                OLED_ShowSignedNum(88,32,Encoder_Get(2),2,OLED_8X16);
                break;
        case 'C'://角度环参数显示
                OLED_Clear();
                OLED_ShowString(0,0,"  Angle  Test  ",OLED_8X16);
                OLED_ShowString(0,16,"P",OLED_8X16);
                OLED_ShowString(0,32,"D",OLED_8X16);
                OLED_ShowString(56,16,"Mea",OLED_8X16);
                OLED_ShowString(56,32,"Tar",OLED_8X16);
                OLED_ShowFloatNum(8,16,AnglePID.Kp,3,0,OLED_8X16);
                OLED_ShowFloatNum(8,32,AnglePID.Kd,3,0,OLED_8X16);
                OLED_ShowFloatNum(80,16,AnglePID.measure,2,2,OLED_8X16);
                OLED_ShowFloatNum(80,32,AnglePID.target,2,2,OLED_8X16);
                break;
        case 'D'://速度环参数显示
                OLED_Clear();
                OLED_ShowString(0,0,"  Speed  Test  ",OLED_8X16);
                OLED_ShowString(0,16,"P",OLED_8X16);
                OLED_ShowString(0,32,"I",OLED_8X16);
                OLED_ShowString(56,16,"Mea",OLED_8X16);
                OLED_ShowString(56,32,"Tar",OLED_8X16);
                OLED_ShowFloatNum(8,16,SpeedPID.Vkp,3,0,OLED_8X16);
                OLED_ShowFloatNum(8,32,SpeedPID.Vki,1,1,OLED_8X16);
                OLED_ShowFloatNum(80,16,SpeedPID.Vmeasure,2,2,OLED_8X16);
                OLED_ShowFloatNum(80,32,SpeedPID.Vtarget,2,2,OLED_8X16);
                break;
        case 'E'://转向环参数显示
                OLED_Clear();
                OLED_ShowString(0,0,"  Turn    Test  ",OLED_8X16);
                OLED_ShowString(0,16,"P",OLED_8X16);
                OLED_ShowString(0,32,"D",OLED_8X16);
                OLED_ShowString(56,16,"Mea",OLED_8X16);
                OLED_ShowString(56,32,"Tar",OLED_8X16);
                OLED_ShowFloatNum(8,16,TurnPID.Tkp,1,1,OLED_8X16);
                OLED_ShowFloatNum(8,32,TurnPID.Tkd,1,1,OLED_8X16);
                OLED_ShowFloatNum(80,16,TurnPID.Tmeasure,1,0,OLED_8X16);
                OLED_ShowFloatNum(80,32,TurnPID.Ttarget,4,0,OLED_8X16);
                break;
        default:break;
    }
	//电机启动状态
	if(key_date)OLED_ShowChar(120,48,'N',OLED_8X16);
    else OLED_ShowChar(120,48,'F',OLED_8X16);
}
