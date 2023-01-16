#include "Directional_control.h"																					//�������--����--ǰ��--��ת


PID PID_Directional;
PID PID_Dir;


float Directional_err_old = 0;
float Directional_err = 0;
float Dir = 0;					//����

void Directional_control( float actual_Directional, float Ideal_Directional ) //���ҷ������PD
{
	Directional_err = actual_Directional - Ideal_Directional;
	
	Dir += Directional_err;																							//������Ҫ��
	if (Dir > CAR_POSITION_MAX)
		Dir = CAR_POSITION_MAX;
	if (Dir < CAR_POSITION_MIN)
		Dir = CAR_POSITION_MIN;

	PID_Directional.pout = PID_Directional.P * Directional_err;
	PID_Directional.iout = PID_Directional.I * Dir;		//i*����
	PID_Directional.dout = PID_Directional.D * (Directional_err - Directional_err_old);
	Directional_err_old = Directional_err; //���¾�ֵ

	PID_Directional.OUT = PID_Directional.pout + PID_Directional.iout + PID_Directional.dout;
	DirectionalOUT = PID_Directional.OUT;
}

float Dir_err_old = 0;
float Dir_err = 0;
float Dir1 = 0;					//����

void Dir_control( float actual_Directional, float Ideal_Directional ) //ֱ�߿���PD
{
	Dir_err = actual_Directional - Ideal_Directional;
	
	Dir1 += Dir_err;																							//������Ҫ��
	if (Dir1 > CAR_POSITION_MAX)
		Dir1 = CAR_POSITION_MAX;
	if (Dir1 < CAR_POSITION_MIN)
		Dir1 = CAR_POSITION_MIN;

	PID_Dir.pout = PID_Dir.P * Dir_err;
	PID_Dir.iout = PID_Dir.I * Dir1;		//i*����
	PID_Dir.dout = PID_Dir.D * (Dir_err - Dir_err_old);
	Dir_err_old = Dir_err; //���¾�ֵ

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

