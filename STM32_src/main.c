#include "main.h"

int16_t fLeftVal = 300,fRightVal = 300;

int main()
{
	NVIC_Configuration();
	delay_init();
	LED_Init();
	KEY_Init();
	Bluetooth_init(115200);																							//����
//	uart_init5(115200);																								//pid����
	Mag_Sen_Init();																									//�Ŵ�����
	RFID_Init();																									//������
	Communication_Init();																							//armͨѶ
	Directional_pid_init();																							//pid��ʼ��
	flash_init();																									//flash����

	motor_init();																									//���pwm��ʼ��
	TIM5_Int_Init(2000, 72);																						//tim5��ʱ2ms
	
	while (1)
	{
		Event_Loop();
	}
}
