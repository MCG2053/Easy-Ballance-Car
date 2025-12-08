#include "Safity.h"

void Safity(void)
{
	static uint16_t T=0;
	// 摔倒检测
	if ((pitch/100.0) > 50 || (pitch/100.0) < -50)
	{
		//电机标志位清零
		key_date = 0;
		// 立即停止电机
		Motor_SetPWM(0,0);
		Motor_SetPWM(0,0);
		// 重置PID积分项
		Reset_intergal(&SpeedPID);
	}

	//抬起检测
	if(key_date)
	{
		if(PWML>=7000 && PWMR>=7000)T++;else T=0;
		if (T>=60)
		{
			//电机标志位清零
			key_date = 0;
			// 立即停止电机
			Motor_SetPWM(0,0);
			Motor_SetPWM(0,0);
			// 重置PID积分项
			Reset_intergal(&SpeedPID);
		}
	}
}
