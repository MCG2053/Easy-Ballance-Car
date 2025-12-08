#include "control.h"

uint8_t ReceiveFlag,BreceiveFlag;							//接收标志位

extern PID AnglePID, SpeedPID, TurnPID;
extern int key_date;

void NRF_Control(void)
{
	int8_t DataY,DataX;
    ReceiveFlag=NRF24L01_Receive();
    if (!ReceiveFlag) return;
    DataX=(int8_t)NRF24L01_RxPacket[4];
    DataY=(int8_t)NRF24L01_RxPacket[5];
    //停止
    if(abs(DataX)<=5 && abs(DataY)<=5)
    {
        SpeedPID.Vtarget = 0;
        TurnPID.Ttarget = 0;  // 保持当前角度
    }
    //向前向后
    if(DataY>5 && abs(DataX)<25)
    {
        SpeedPID.Vtarget = +4;
        TurnPID.Ttarget = 0;  // 保持方向
    }else if(DataY<-5 && abs(DataX)<5)
    {
        SpeedPID.Vtarget = -4;
        TurnPID.Ttarget = 0;  // 保持方向
    }
    //左转右转
    if(DataX<-5 && abs(DataY)<5)
    {
        // 左转：设置相对角度目标
        TurnPID.Ttarget = -1000;
    }
    else if(DataX>5 && abs(DataY)<5)
    {
        // 右转：设置相对角度目标  
        TurnPID.Ttarget = +1000;
    }
}

void NRF_Debug(void)
{
    ReceiveFlag=NRF24L01_Receive();
	//2.4G模块调试三项	
    if (!ReceiveFlag) return;		//接收标志位为1，表示接收成功
    //三环调节切换
    switch(NRF24L01_RxPacket[1])
    {
        case 1:	//直立环PD
                Oled_Flag='C';
                switch(NRF24L01_RxPacket[2])
                {
                    case 1:if(NRF24L01_RxPacket[0]==0){AnglePID.Kp+=10;}else if(NRF24L01_RxPacket[0]==1){AnglePID.Kp-=10;}break;
                    case 2:if(NRF24L01_RxPacket[0]==0){AnglePID.Kd+=10;}else if(NRF24L01_RxPacket[0]==1){AnglePID.Kd-=10;}break;
                    default:break;
                }
                break;
        case 2: //速度环PI
                Oled_Flag='D';
                switch(NRF24L01_RxPacket[2])
                {
                    case 1:if(NRF24L01_RxPacket[0]==0){SpeedPID.Vkp-=10;}else if(NRF24L01_RxPacket[0]==1){SpeedPID.Vkp+=10;}break;
                    case 2:if(NRF24L01_RxPacket[0]==0){SpeedPID.Vki-=0.05;}else if(NRF24L01_RxPacket[0]==1){SpeedPID.Vki+=0.05;}break;
                    default:break;
                }
                break;
        case 3: //转向环PD
                Oled_Flag='E';
                switch(NRF24L01_RxPacket[2])
                {
                    case 1:if(NRF24L01_RxPacket[0]==0){TurnPID.Tkp+=0.1;}else if(NRF24L01_RxPacket[0]==1){TurnPID.Tkp-=0.1;}break;
                    case 2:if(NRF24L01_RxPacket[0]==0){TurnPID.Tkd+=0.01;}else if(NRF24L01_RxPacket[0]==1){TurnPID.Tkd-=0.01;}break;								
                    default:break;
                }
                break;
        default:break;
    }
    //远程手动校准角度
    if(NRF24L01_RxPacket[3]==1)
    {
        key_date=0;//关闭PID计算，关闭电机输出
        Delay_ms(3000);
        GY25_SetMode(GY25Mode_SET_Roll_and_Pitch);Delay_ms(50);
        GY25_SetMode(GY25Mode_SET_Yaw);Delay_ms(50);
        GY25_SetMode(GY25Mode_Inquire_AUOT);Delay_ms(50);
        key_date=1;//开启PID计算，开启电机输出
    }
}


// 简化的蓝牙解析函数（直接在main函数中使用）
void BT_Control(void)
{
    BreceiveFlag=Bluetooth_GetRxFlag();
    if(!BreceiveFlag)return;
    if(strcmp(Serial_RxPacket,"STOP")==0)
    {
        SpeedPID.Vtarget = 0;  // 直接给正数前进
        TurnPID.Ttarget = 0;  // 保持方向
    }
    if(strcmp(Serial_RxPacket,"FORWARD")==0)
    {
        SpeedPID.Vtarget = +5;  // 直接给正数前进
        TurnPID.Ttarget = 0;  // 保持方向
    }
    else if(strcmp(Serial_RxPacket,"BACKWARD")==0)
    {
        SpeedPID.Vtarget = -5;  // 直接给负数后退！关键修改
        TurnPID.Ttarget = 0;  // 保持方向
    }
    if(strcmp(Serial_RxPacket,"LEFT")==0)
    {
        TurnPID.Ttarget = +1000;
    }
    else if(strcmp(Serial_RxPacket,"RIGHT")==0)
    {
        TurnPID.Ttarget = -1000;
    }
}
