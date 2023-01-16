#include "control.h"


u16 Event_cnt1 = 0, Event_cnt2 = 0, Event_cnt3 = 0;
u8 SenValue;
bool LED_FLAG = false;
u8 id;

void Event_Loop( void ) //�¼�ѭ��  ���ڿ���ִ��ʱ�䳤�ĺ���
{
	if (Event_cnt1 >= 5)
	{
		Bluetooth_data_deal();
//		Printf(USART1, "s0:%.0f  s1:%.0f  s2:%.0f  s3:%.0f  s4:%.0f  s5:%.0f  s6:%.0f  s7:%.0f\n", mag[0], mag[1], mag[2], mag[3], mag[4], mag[5], mag[6], mag[7]);
//		ANO_DT_Send_Senser(PID_Directional.pout,PID_Directional.iout,PID_Directional.dout,PID_Directional.OUT,Directional_err);
		Event_cnt1 = 0;
	}
	if (Event_cnt2 >= 7)
	{
		Event_cnt2 = 0;
	}
	if (Event_cnt3 >= 300)
	{
		Event_cnt3 = 0;
	}

	if (Rx2Flag)
	{
		Rx2Flag = 0;
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)!LED_FLAG);														//led����ʾˢ����
		id = ID_Switch();
		TX_Date[2] = id;
		Send(TX_Date);																								//�ϱ�����
	}
	RX_Process();																									//Ӧ�����
}


void Simulation_multithreading( void )
{
	static u8 counter = 0;
	static u16 cou_100 = 0;

	counter++; //�ڲ������ۼ�
	cou_100++;
	Event_cnt1++;
	Event_cnt2++;
	Event_cnt3++;
	Sys_Counter++;																									//armͨѶӦ��200ms��ʱ����

	switch (counter)
	{
		case 1:
		{
			sen_get_data(&SenValue);																				//�õ�8λ�Ŵ�����ģ����
			route_plan();																							//·������
//			Directional_control(Direction_output, 0);																//pid���
			break;
		}
		case 2:
		{
			if (flag_motor) 																						//�������¿���
			{
				MotorOutput();
			}
			else 
			{
				TIM3->CCR1 = 0;
				TIM3->CCR2 = 0;
				motor_stop();																						//�������
			}
			counter = 0;
			break;
		}
		default: break;
	}

	if (cou_100 >= 200)			//400ms
	{
		KEY_Scan();				//����ɨ��
		cou_100 = 0;
	}
}

