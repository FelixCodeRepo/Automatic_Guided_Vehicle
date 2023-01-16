#include "Path_Recognition.h"



float Direction_output;							//·��ʶ�������ֵ


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


void route_plan(void)															//������ʶ��
{
	uint8_t max_0to3 = 0; 														//ͨ��0��3������ź���С���

	uint8_t i;

	for (i = 0; i < 8; i++)														//��һ������
	{
		sensor_to_one[i] = (float)(mag_data[i] - LDC_min_v[i]) / (float)(LDC_max_v[i] - LDC_min_v[i]);
		if (sensor_to_one[i] <= 0.0)  sensor_to_one[i] = 0.001;					//������0����
		if (sensor_to_one[i] > 1.0)  sensor_to_one[i] = 1.0;
		mag[i] = 100 * sensor_to_one[i];										//AD[i]Ϊ��һ�����ֵ  ��ΧΪ0-100
	}

	Directional_plan();															//��·��ת�����

/*����·�����������*/
	
	if(go_start == true)
	{
		/*���ݲ���*/
		for (i = 1; i<8; i++)														//�ҳ�0��7����ǿ�Ĵ�����
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
			Direction_output = 100 * (mag[3] - mag[4]) / (mag[4] + mag[3]) * k3;	//3��4���Ч�����ã�����ʹ��2��5
			Dir_control(Direction_output, 0);																//pid���
		}
	}
	else if(left_start == true)
	{
		max_0to3 = 5;
		for (i = 6; i<8; i++)														//�ҳ�5��7����ǿ�Ĵ�����
		{
			if (mag[i] > mag[max_0to3])
				max_0to3 = i;
		}
		if(mag[max_0to3] < 20)
		{
			mag[max_0to3] = 20;
		}
		
		Direction_output = -mag[max_0to3] * k5;
		Directional_control(Direction_output, 0);																//pid���
	}
	else if(right_start == true)
	{
		max_0to3 = 0;
		for (i = 1; i<3; i++)														//�ҳ�0��2����ǿ�Ĵ�����
		{
			if (mag[i] > mag[max_0to3])
				max_0to3 = i;
		}
		
		if(mag[max_0to3] < 20)
		{
			mag[max_0to3] = 20;
		}
		
		Direction_output = mag[max_0to3] * k2;
		Directional_control(Direction_output, 0);																//pid���
	}
		
/*����·�����������*/	
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
		//����ͣ
	}

	if (mag[0] < 20 && mag[1] < 20 && mag[2] < 20 && mag[3] < 20 && mag[4] < 20 && mag[5] < 20 && mag[6] < 20 && mag[7] < 20)
	{
		Flag_Speed = 0;				//ͣ������������
	}

	fun();
	
	if (Flag_Dir == 0x00)		//��ǰ��
	{
		Direc_flag = 1;
	}
	else
	{
		Direc_flag = -1;		//����
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
		if(Flag_Way == 0x02)				//��ת
		{
			right_start = true;
			go_start = false;
		}
		else if(Flag_Way == 0x01)		//��ת
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
  else if ((mag[0] > 50 || mag[1] > 37 && mag[2] > 40) && (mag[3] > 40 && mag[4] > 40) || (right_n >= 2))  //ֱ�ߺ���ת
	{
		if (Flag_Way == 0x03)					//ֱ��
		{
			go_start = true;
			right_start = false;
		}
		else if(Flag_Way == 0x02)			//��ת
		{
			right_start = true;
			go_start = false;
		}
	}
  else if ((mag[7] > 50 || mag[6] > 37 && mag[5] > 40) && (mag[3] > 40 && mag[4] > 40) || (left_n >= 2))	//ֱ�ߺ���ת
	{
		if (Flag_Way == 0x03)					//ֱ��
		{
			go_start = true;
			left_start = false;
		}
		else if(Flag_Way == 0x01)		//��ת
		{
			left_start = true;
			go_start = false;
		}
	}
	
	if (Flag_Way == 0x03)					//ֱ�������������·��
	{
		go_start = true;
		left_start = false;
		right_start = false;
	}

	if(((mag[3] > 40 && mag[4] > 40) && (mag[7] < 30 && mag[6] < 30 || mag[5] < 30) || (Flag_Way == 0x00) || (Flag_Way == 0x03)) && left_start == true)					//��תת�����
	{
		left_start = false;
		go_start = true;
	}
	
	if(((mag[3] > 40 && mag[4] > 40) && (mag[0] < 30 && mag[1] < 30 || mag[2] < 30) ||  (Flag_Way == 0x00) || (Flag_Way == 0x03)) && right_start == true)					//��תת�����
	{
		right_start = false;
		go_start = true;
	}
}

