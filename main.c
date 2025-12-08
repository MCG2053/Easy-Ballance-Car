//头文件
#include "SysInit.h" 
#include "Safity.h"
#include "Control.h"
#include "Menu.h"

int key_date=1;

float SpeedLeft, SpeedRight; 
float AveSpeed, DifSpeed;

float AvePWM, DifPWM=0.0;
int16_t PWML, PWMR;

/*角度环PID参数*/
PID AnglePID = {	
	.Kp = -150,// 250*0.6==150,
	.Kd = -800,// 650*0.6==390,
	.target=0,
};

/*速度环PID参数*/ 
PID SpeedPID = {	
	.Vkp = -380,
	.Vki = -380/200,// 340/200==1.7
};

/*转向环PID参数*/
PID TurnPID = {	
	.Tkp = -1,
	.Tkd = -0.1,
};

//主函数
int main(void)
{
    Sys_Init();
	while (1)
	{
		Safity();		//安全装置
		NRF_Control();  //2.4G远程控制
		NRF_Debug();	//2.4G远程调试
        BT_Control();
		Oled_Menu();    //OLED显示
		OLED_Update();  //OLED刷新
	}
}

void DMA1_Channel3_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC3) != RESET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC3); // 清除中断标志
		// 临时拷贝防止DMA覆盖
        uint8_t tempBuf[8];
		memcpy(tempBuf,DataBuf,sizeof(tempBuf));
		if(tempBuf[0]==0xAA&&tempBuf[7]==0x55)
		{
            int16_t yaw_raw = (int16_t)(((uint16_t)tempBuf[1] << 8) | (uint16_t)tempBuf[2]);
            int16_t pitch_raw = (int16_t)(((uint16_t)tempBuf[3] << 8) | (uint16_t)tempBuf[4]);
            int16_t roll_raw = (int16_t)(((uint16_t)tempBuf[5] << 8) | (uint16_t)tempBuf[6]);
            // 转换为int32_t（自动符号扩展，负数正确保留）
            yaw = (int32_t)yaw_raw;
            pitch = (int32_t)pitch_raw;
            roll = (int32_t)roll_raw;
			Read_State=0;
		}
		else
		{
			Read_State++;
			if(Read_State>500)
			{
				GY25_SetMode(GY25Mode_Inquire_AUOT);
			}
		}
		
		/*PID控制*/
		if (key_date)
		{
			/*通过编码器获取电机旋转速度 计算后单位m/s*/
			SpeedLeft = -Encoder_Get(1);
			SpeedRight = Encoder_Get(2);
			
			/*获取均速和差速*/
			AveSpeed = (SpeedLeft + SpeedRight) * 0.5;
			DifSpeed = SpeedLeft - SpeedRight;
			
			AnglePID.target=(float)pitch/100.0;
			SpeedPID.Vmeasure=AveSpeed;
			TurnPID.Tmeasure=DifSpeed;
			
			AvePWM=Vertical_PID(&AnglePID)+Velocity_PID(&SpeedPID,3000.0);
			DifPWM=Turn_PID(&TurnPID,2000.0);
            
			// 计算左右轮独立PWM
			PWML = AvePWM + DifPWM;
			PWMR = AvePWM - DifPWM;
			
			// 分别限幅
			if (PWML > 7000) PWML = 7000;
			else if (PWML < -7000) PWML = -7000;
			if (PWMR > 7000) PWMR = 7000;  
			else if (PWMR < -7000) PWMR = -7000;
			
			/*PWM输出至电机*/
			Motor_SetPWM(PWML,PWMR);
		}
		else
		{
			//电机标志位清零
			key_date = 0;
			// 立即停止电机并重置积分
			Motor_SetPWM(0,0);
			Motor_SetPWM(0,0);
			// 重置PID积分项
			Reset_intergal(&SpeedPID);
		}
		
	}
}
