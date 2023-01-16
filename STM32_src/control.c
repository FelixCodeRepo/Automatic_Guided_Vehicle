#include "control.h"


u16 Event_cnt1 = 0, Event_cnt2 = 0, Event_cnt3 = 0;
u8 SenValue;
bool LED_FLAG = false;
u8 id;

void Event_Loop( void ) //事件循环  用于控制执行时间长的函数
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
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)!LED_FLAG);														//led灯提示刷到卡
		id = ID_Switch();
		TX_Date[2] = id;
		Send(TX_Date);																								//上报卡号
	}
	RX_Process();																									//应答分析
}


void Simulation_multithreading( void )
{
	static u8 counter = 0;
	static u16 cou_100 = 0;

	counter++; //内部计数累加
	cou_100++;
	Event_cnt1++;
	Event_cnt2++;
	Event_cnt3++;
	Sys_Counter++;																									//arm通讯应答200ms计时变量

	switch (counter)
	{
		case 1:
		{
			sen_get_data(&SenValue);																				//得到8位磁传感器模拟量
			route_plan();																							//路径分区
//			Directional_control(Direction_output, 0);																//pid输出
			break;
		}
		case 2:
		{
			if (flag_motor) 																						//按键按下开车
			{
				MotorOutput();
			}
			else 
			{
				TIM3->CCR1 = 0;
				TIM3->CCR2 = 0;
				motor_stop();																						//锁死电机
			}
			counter = 0;
			break;
		}
		default: break;
	}

	if (cou_100 >= 200)			//400ms
	{
		KEY_Scan();				//键盘扫描
		cou_100 = 0;
	}
}

