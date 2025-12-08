#include "PID.h"           

//结构体初始化
void PID_Init(PID *p)
{
	//直立环参数
	p->err=0;
	p->err1=0;
	p->errd=0;
	p->measure=0;
	//速度环参数
	p->Vmeasure=0;
	p->calculate=0;
	p->filter=0;
	p->filter1=0;
	p->filters=0;
	p->coefficient=0;
	//转向环参数
	p->Tmeasure=0;
	p->Ttarget=0;//输入值
	p->Terr=0;
}

//直立环
int Vertical_PID(PID *p)
{
	p->err=p->measure - p->target;		//角度误差
	
	// 角度死区：避免在平衡点附近微小震荡
    if(p->err < 0.3f && p->err > -0.3f)p->err = 0;
	
	p->errd=p->err - p->err1;		//误差差值	
	
	p->err1=p->err;		//记录最后一次误差
	
	return (int)(p->Kp * p->err + p->Kd * p->errd);
}

//速度环
int Velocity_PID(PID *p,int max)
{
	p->coefficient=0.3;		//滤波系数
	p->calculate = p->Vmeasure - p->Vtarget;		//速度偏差
		
	// 速度死区：小误差时不响应
    if(p->calculate < 0.5f && p->calculate > -0.5f)p->calculate = 0;
	
	p->filter = p->coefficient * p->calculate + (1 - p->coefficient) * p->filter1;	//一阶速度滤波
	
	p->filters += p->filter;	//速度叠加
	
	//叠加限幅
	if(p->filters > max)p->filters = max;
	if(p->filters < -max)p->filters = - max;
	
	p->filter1=p->filter;	//记录最后一次速度
	
	return (int)(p->Vkp * p->filter + p->Vki * p->filters);
}

//转向环<电机差速>
int Turn_PID(PID *p,int max)
{
	p->Terr=p->Tmeasure - p->Ttarget;		//转向偏差
	
	p->Terrd=p->Terr - p->Terr1;		//误差差值
	
	p->KPout=p->Tkp * p->Terr;	//比例项计算
	
	p->KDout=p->Tkd * p->Terrd;	//微分项计算
	
	p->out=p->KPout + p->KDout;	//总输出
	
	//输出限幅
	if(p->out > max)p->out = max;
	if(p->out < -max)p->out = - max;
	
	p->Terr1=p->Terr;		//记录最后一次误差
	
	return (int)p->out;
}

//重置积分
void Reset_intergal(PID *p)
{
	p->filter1=0;
	p->filter=0;
	p->filters=0;
}
