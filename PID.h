#ifndef __PID_H
#define __PID_H

//头文件
#include "stm32f10x.h"                  // Device header

/*
    kp ki kd后缀 = 比例项 积分项 微分项
    measure后缀 = 实际值
    target后缀 = 目标值
    err err1 errd后缀 = 本次误差 先前误差 误差差值
    calculate = 速度差
    filter filter1 filters = 一阶速度滤波后当前速度值 先前速度值 总速度值
    coefficient = 滤波系数 //此项建议直接在PID.c文件中编辑
*/
typedef struct{
	//直立环参数
	float Kp;
	float Kd;
	float measure;
	float target;
	float err;
	float err1;
	float errd;
	//速度环参数
	float Vkp;
	float Vki;
	float Vmeasure;
	float Vtarget;
	float calculate;
	float filter;
	float filter1;
	float filters;
	float coefficient;
	//转向环参数
	float Tkp;
	float Tkd;
	float KPout;    //kp输出
	float KDout;    //kd输出
	float Tmeasure;
	float Ttarget;
	float Terr;
	float Terr1;
	float Terrd;
	float out;     //总输出
}PID;

//函数声明
void PID_Init(PID *p);  //初始化
int Vertical_PID(PID *p);   //直立环
int Velocity_PID(PID *p,int max);   //速度环
int Turn_PID(PID *p,int max);   //转向环(转速差)
void Reset_intergal(PID *p);    //积分重置
#endif
