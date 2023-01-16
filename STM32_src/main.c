#include "main.h"

int16_t fLeftVal = 300,fRightVal = 300;

int main()
{
	NVIC_Configuration();
	delay_init();
	LED_Init();
	KEY_Init();
	Bluetooth_init(115200);																							//蓝牙
//	uart_init5(115200);																								//pid波形
	Mag_Sen_Init();																									//磁传感器
	RFID_Init();																									//读卡器
	Communication_Init();																							//arm通讯
	Directional_pid_init();																							//pid初始化
	flash_init();																									//flash操作

	motor_init();																									//电机pwm初始化
	TIM5_Int_Init(2000, 72);																						//tim5定时2ms
	
	while (1)
	{
		Event_Loop();
	}
}
