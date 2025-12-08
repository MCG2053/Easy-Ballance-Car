#include "SysInit.h"

void Sys_Init(void)
{
    /*模块初始化*/
	OLED_Init();		//OLED初始化
	Motor_Init();		//电机控制初始化
	Encoder_Init();		//电机编码初始化
	Bluetooth_Init();		//蓝牙串口初始化
	NRF24L01_Init();    //2.4G无线模块初始化
	Delay_ms(50);
    GY25_Init();		//陀螺仪初始化
	
	/*设置*/
	GY25_SetMode(GY25Mode_Inquire_AUOT);Delay_ms(50);//自动获取角度模式
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置全局NVIC优先级分组
	
	PID_Init(&AnglePID);
	PID_Init(&SpeedPID);
	PID_Init(&TurnPID);

	uint8_t Test=1;
    
    if(Test)
	{
		Calibrator();	//初始校准
		Test=0;
	}
}

void Calibrator(void)
{
	uint8_t count=3;
	key_date=0;
	OLED_ShowString(0,0,"hold car 3 second",OLED_8X16);
	while(count)
	{
		OLED_ShowNum(32,16,count,1,OLED_8X16);
		OLED_ShowString(48,16,"second",OLED_8X16);
		OLED_Update();
		Delay_ms(1000);
		count--;
	}
	OLED_Clear();
	
	GY25_SetMode(GY25Mode_SET_Roll_and_Pitch);Delay_ms(50);//校准翻滚角俯仰角
	GY25_SetMode(GY25Mode_SET_Yaw);Delay_ms(50);//校准航向角
	GY25_SetMode(GY25Mode_Inquire_AUOT);Delay_ms(50);//自动获取角度模式
	
	OLED_ShowString(32,16,"success",OLED_8X16);
	OLED_Update();
	Delay_ms(500);			
	key_date=1;
    OLED_Clear();
}
