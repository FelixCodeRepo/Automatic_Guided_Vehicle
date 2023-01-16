#include "Directional_control.h"																					//方向控制--左右--前后--旋转


PID PID_Directional;
PID PID_Dir;


float Directional_err_old = 0;
float Directional_err = 0;
float Dir = 0;					//积分

void Directional_control( float actual_Directional, float Ideal_Directional ) //左右方向控制PD
{
	Directional_err = actual_Directional - Ideal_Directional;
	
	Dir += Directional_err;																							//积分需要调
	if (Dir > CAR_POSITION_MAX)
		Dir = CAR_POSITION_MAX;
	if (Dir < CAR_POSITION_MIN)
		Dir = CAR_POSITION_MIN;

	PID_Directional.pout = PID_Directional.P * Directional_err;
	PID_Directional.iout = PID_Directional.I * Dir;		//i*积分
	PID_Directional.dout = PID_Directional.D * (Directional_err - Directional_err_old);
	Directional_err_old = Directional_err; //更新旧值

	PID_Directional.OUT = PID_Directional.pout + PID_Directional.iout + PID_Directional.dout;
	DirectionalOUT = PID_Directional.OUT;
}

float Dir_err_old = 0;
float Dir_err = 0;
float Dir1 = 0;					//积分

void Dir_control( float actual_Directional, float Ideal_Directional ) //直走控制PD
{
	Dir_err = actual_Directional - Ideal_Directional;
	
	Dir1 += Dir_err;																							//积分需要调
	if (Dir1 > CAR_POSITION_MAX)
		Dir1 = CAR_POSITION_MAX;
	if (Dir1 < CAR_POSITION_MIN)
		Dir1 = CAR_POSITION_MIN;

	PID_Dir.pout = PID_Dir.P * Dir_err;
	PID_Dir.iout = PID_Dir.I * Dir1;		//i*积分
	PID_Dir.dout = PID_Dir.D * (Dir_err - Dir_err_old);
	Dir_err_old = Dir_err; //更新旧值

	PID_Dir.OUT = PID_Dir.pout + PID_Dir.iout + PID_Dir.dout;
	DirectionalOUT = PID_Dir.OUT;
}

void Directional_pid_init( void )
{
	PID_Directional.P = 16;		//10.5
	PID_Directional.I = -0.07;
	PID_Directional.D = 170;
	
	PID_Dir.P = 16;		//10.5
	PID_Dir.I = -0.07;
	PID_Dir.D = 170;
//	PID_Directional.P = 12.5;
//	PID_Directional.I = -0.035;
//	PID_Directional.D = 90;
}

