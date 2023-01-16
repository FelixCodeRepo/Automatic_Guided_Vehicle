#include "Path_Recognition.h"



float Direction_output;							//路径识别后的输出值


//float k0 = 0.7;
//float k1 = 0.7;
//float k2 = 0.7;
//float k3 = 1.0;
//float k5 = 0.7;
//float k6 = 0.7;
//float k7 = 0.7;

//float k0 = 1.2;
//float k1 = 1.1;
float k2 = 1.0;
float k3 = 1.0;
float k5 = 1.0;
//float k6 = 1.1;
//float k7 = 1.2;

bool right_start = false;
bool left_start = false;
bool go_start = true;


void fun(void);
void Directional_plan(void);

float mag[8], sensor_to_one[8];


void route_plan(void)															//赛道的识别
{
	uint8_t max_0to3 = 0; 														//通道0到3的最大标号和最小标号

	uint8_t i;

	for (i = 0; i < 8; i++)														//归一化处理
	{
		sensor_to_one[i] = (float)(mag_data[i] - LDC_min_v[i]) / (float)(LDC_max_v[i] - LDC_min_v[i]);
		if (sensor_to_one[i] <= 0.0)  sensor_to_one[i] = 0.001;					//消除除0错误
		if (sensor_to_one[i] > 1.0)  sensor_to_one[i] = 1.0;
		mag[i] = 100 * sensor_to_one[i];										//AD[i]为归一化后的值  范围为0-100
	}

	Directional_plan();															//叉路口转向控制

/*根据路径给出输出量*/
	
	if(go_start == true)
	{
		/*数据查找*/
		for (i = 1; i<8; i++)														//找出0到7的最强的传感器
		{
			if (mag[i] > mag[max_0to3])
				max_0to3 = i;
		}
		
		if(mag[3] < 35 && mag[4] < 35)
		{
			if(max_0to3 >= 4)
			{
				Direction_output = -mag[max_0to3] * k5;	
			}
			else if(max_0to3 <= 3)
			{
				Direction_output = mag[max_0to3] * k2;	
			}
			Directional_control(Direction_output, 0);	
		}
		else
		{
			Direction_output = 100 * (mag[3] - mag[4]) / (mag[4] + mag[3]) * k3;	//3和4如果效果不好，可以使用2和5
			Dir_control(Direction_output, 0);																//pid输出
		}
	}
	else if(left_start == true)
	{
		max_0to3 = 5;
		for (i = 6; i<8; i++)														//找出5到7的最强的传感器
		{
			if (mag[i] > mag[max_0to3])
				max_0to3 = i;
		}
		if(mag[max_0to3] < 20)
		{
			mag[max_0to3] = 20;
		}
		
		Direction_output = -mag[max_0to3] * k5;
		Directional_control(Direction_output, 0);																//pid输出
	}
	else if(right_start == true)
	{
		max_0to3 = 0;
		for (i = 1; i<3; i++)														//找出0到2的最强的传感器
		{
			if (mag[i] > mag[max_0to3])
				max_0to3 = i;
		}
		
		if(mag[max_0to3] < 20)
		{
			mag[max_0to3] = 20;
		}
		
		Direction_output = mag[max_0to3] * k2;
		Directional_control(Direction_output, 0);																//pid输出
	}
		
/*根据路径给出输出量*/	
	if (Direction_output > 200)
		Direction_output = 200;
	else if (Direction_output < -200)
		Direction_output = -200;
}

void Directional_plan(void)
{
	if (Flag_Speed == 0x00)
	{
		flag_motor = false;
	}
	else if (Flag_Speed == 0x01)
	{
		flag_motor = true;
	}
	else if (Flag_Speed == 0x02)
	{
		//减速停
	}

	if (mag[0] < 20 && mag[1] < 20 && mag[2] < 20 && mag[3] < 20 && mag[4] < 20 && mag[5] < 20 && mag[6] < 20 && mag[7] < 20)
	{
		Flag_Speed = 0;				//停车，丢掉磁条
	}

	fun();
	
	if (Flag_Dir == 0x00)		//向前走
	{
		Direc_flag = 1;
	}
	else
	{
		Direc_flag = -1;		//后退
	}
}

void fun(void)
{
	u8 i;
	u8 left_n = 0, right_n = 0;
	
	for(i = 0; i<3; i++)
	{
		if(mag[i] > 50)
		{
			right_n++;
		}
	}
	
	for(i = 5; i<8; i++)
	{
		if(mag[i] > 50)
		{
			left_n++;
		}
	}
	
	if(mag[2] > 50 && mag[3] > 60 && mag[4] > 60 && mag[5] > 50)
	{
		if(Flag_Way == 0x02)				//右转
		{
			right_start = true;
			go_start = false;
		}
		else if(Flag_Way == 0x01)		//左转
		{
			left_start = true;
			go_start = false;
		}
		else if(Flag_Way == 0x03)
		{
			go_start = true;
			left_start = false;
			right_start = false;
		}
	}
  else if ((mag[0] > 50 || mag[1] > 37 && mag[2] > 40) && (mag[3] > 40 && mag[4] > 40) || (right_n >= 2))  //直走和右转
	{
		if (Flag_Way == 0x03)					//直行
		{
			go_start = true;
			right_start = false;
		}
		else if(Flag_Way == 0x02)			//右转
		{
			right_start = true;
			go_start = false;
		}
	}
  else if ((mag[7] > 50 || mag[6] > 37 && mag[5] > 40) && (mag[3] > 40 && mag[4] > 40) || (left_n >= 2))	//直走和左转
	{
		if (Flag_Way == 0x03)					//直行
		{
			go_start = true;
			left_start = false;
		}
		else if(Flag_Way == 0x01)		//左转
		{
			left_start = true;
			go_start = false;
		}
	}
	
	if (Flag_Way == 0x03)					//直行命令忽略两边路段
	{
		go_start = true;
		left_start = false;
		right_start = false;
	}

	if(((mag[3] > 40 && mag[4] > 40) && (mag[7] < 30 && mag[6] < 30 || mag[5] < 30) || (Flag_Way == 0x00) || (Flag_Way == 0x03)) && left_start == true)					//左转转弯结束
	{
		left_start = false;
		go_start = true;
	}
	
	if(((mag[3] > 40 && mag[4] > 40) && (mag[0] < 30 && mag[1] < 30 || mag[2] < 30) ||  (Flag_Way == 0x00) || (Flag_Way == 0x03)) && right_start == true)					//右转转弯结束
	{
		right_start = false;
		go_start = true;
	}
}

