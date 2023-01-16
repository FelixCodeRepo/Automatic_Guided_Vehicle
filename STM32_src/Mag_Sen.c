#include "Mag_Sen.h"

static u8 sen_cmd[13] = {0xAB, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD};
u8 rx_data[13];
bool Flag_sen_date = false;

uint8_t  LDC_max_v[8] = { 30, 30, 30, 30, 30, 30, 30, 30 };															//��flash��ֵʱ��max-minΪ0�����ֳ�0  
uint8_t  LDC_min_v[8] = { 10, 10, 10, 10, 10, 10, 10, 10 }; 														//��ʼ��Сֵ


void sen_make_check(unsigned char *buf)
{
	int i;
	unsigned char temp = 0;
	for (i = 1; i <= 10; i++)
		temp += buf[i];
	
	buf[11] = temp;
}

bool sen_do_check(unsigned char *buf)
{
	int i;
	unsigned char sum = 0;
	for (i = 1; i <= 10; i++)
		sum += buf[i];
	if (buf[11] == sum)
		return true;
	else
		return false;
}

void uart_init3(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ��USART3
	USART_DeInit(USART3); //��λ����1

	//USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound; //һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);

	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //�����ж�

	USART_Cmd(USART3, ENABLE); //ʹ�ܴ���
}

void Mag_Sen_Init(void)
{
	uart_init3(115200);
}

unsigned char temp[20];

void USART3_IRQHandler(void)                	//����3�жϷ������
{
	static u8 RX_Counter=0;
	u8 i;
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		temp[RX_Counter] = USART_ReceiveData(USART3);	//������յ�������
		RX_Counter++;
		if (RX_Counter >= 13)
		{
			for (i = 0; i < 7; i++)
			{
				if(temp[i] == 0xAB && temp[i + 12] == 0xCD)
				{
					memcpy(rx_data, temp + i, 13);
					memset(temp, 0, 20);
					Flag_sen_date = true;
					RX_Counter = 0;
					break;
				}
			}
		}
		
		if(RX_Counter >= 18)
			RX_Counter = 0;
	}
}

void Ask_Data(void)
{
	int i;

	sen_make_check(sen_cmd);							//����У��

	for (i = 0; i < 13; i++)							//����
	{
		USART_SendData(USART3, sen_cmd[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}

float mag_k[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };			//����ϵ��

#define FILTERWIDTH 1													//�˲����
int16_t mag_FIFO[8][FILTERWIDTH]; //��ֵ�˲��������  

void mag_Filter(uint8_t mag_in[], uint8_t mag_out[])
{
	uint8_t i;
	static uint8_t filter_cnt = 0;
	int32_t sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;

	mag_FIFO[0][filter_cnt] = mag_in[0];
	mag_FIFO[1][filter_cnt] = mag_in[1];
	mag_FIFO[2][filter_cnt] = mag_in[2];
	mag_FIFO[3][filter_cnt] = mag_in[3];
	mag_FIFO[4][filter_cnt] = mag_in[4];
	mag_FIFO[5][filter_cnt] = mag_in[5];
	mag_FIFO[6][filter_cnt] = mag_in[6];
	mag_FIFO[7][filter_cnt] = mag_in[7];

	for (i = 0; i < FILTERWIDTH; i++)
	{
		sum0 += mag_FIFO[0][i];
		sum1 += mag_FIFO[1][i];
		sum2 += mag_FIFO[2][i];
		sum3 += mag_FIFO[3][i];
		sum4 += mag_FIFO[4][i];
		sum5 += mag_FIFO[5][i];
		sum6 += mag_FIFO[6][i];
		sum7 += mag_FIFO[7][i];
	}

	mag_out[0] = sum0 / FILTERWIDTH;
	mag_out[1] = sum1 / FILTERWIDTH;
	mag_out[2] = sum2 / FILTERWIDTH;
	mag_out[3] = sum3 / FILTERWIDTH;
	mag_out[4] = sum4 / FILTERWIDTH;
	mag_out[5] = sum5 / FILTERWIDTH;
	mag_out[6] = sum6 / FILTERWIDTH;
	mag_out[7] = sum7 / FILTERWIDTH;

	filter_cnt++;
	if (filter_cnt >= FILTERWIDTH)
	{
		filter_cnt = 0;
	}
}

uint8_t mag_data[8] = {0};
u8 last_date;
bool sen_get_data(u8 *buf)  //���ؿ���������
{
	Ask_Data();

	if (Flag_sen_date == true)
	{
		Flag_sen_date = false;							//��ձ�־λ

		if (sen_do_check(rx_data))
		{
			*buf = rx_data[10];							//���
			last_date = rx_data[10];

			memcpy(mag_data, rx_data + 2, 8);			//��ȡģ����
//			mag_Filter(rx_data + 2, mag_data);			//�˲�
			
			return true;
		}
		else
		{
			*buf = last_date;
			return false;
		}
	}

	return false;
}



//void Calculate_k(void)
//{
//	uint8_t i = 0;
//	uint16_t num[8] = { 0 };
//	float k[8] = { 0 };
//	float average = 0;
//
//	for (i = 0; i < 100; i++)																//����ʼ���
//	{
//		delay_ms(10);
//	}
//
//	for (i = 0; i < 8; i++)
//	{
//		num[i] = LDC_max_v[i] - LDC_min_v[i];
//	}
//
//	average = (num[0] + num[1] + num[2] + num[3]+ num[4] + num[5] + num[6] + num[7]) / 8;
//
//	for (i = 0; i < 8; i++)
//	{
//		k[i] = average / num[i];
//	}
//
//	for (i = 0; i < 8; i++)
//	{
//		mag_k[i] = k[i];
//	}
//}

void flash_init(void)
{
	uint16_t i, j;
	bool flag;
	bool LED_FLAG = 0;

	if (key_get(KEY0) == KEY_DOWN)										//��ʾ�ɼ���Ŵ����������ֵ����Сֵ
	{
		for (i = 0; i < 100; i++)												//����adc�ĳ�ʼ���
		{
			delay_ms(10);
		}

		while (1)
		{
			for (i = 0; i < 100; i++)
			{
				flag = 0;

				for (j = 0; j < 8; j++)									//�ҳ����ֵ����Сֵ
				{
					if (mag_data[j] > LDC_max_v[j])
					{
						LDC_max_v[j] = mag_data[j];
						flag = 1;
					}
				}

				for (j = 0; j < 8; j++)
				{
					if ((mag_data[j] < LDC_min_v[j]))
					{
						LDC_min_v[j] = mag_data[j];
						flag = 1;
					}
				}

				if (flag)
				{
					
				}
				delay_ms(4);
			}

			GPIO_WriteBit(GPIOB, GPIO_Pin_4, (!LED_FLAG) == 0 ? Bit_RESET : Bit_SET);			//�ڲɼ������Сֵʱled20 1s��˸һ��

			if (KEY_DOWN == key_get(KEY1))							//����while
				break;
		}

		STMFLASH_Write(FLASH_MAX_ADDR, (u16*)LDC_max_v, 8);
		delay_ms(10);

		STMFLASH_Write(FLASH_MIN_ADDR, (u16*)LDC_min_v, 8);
		delay_ms(10);
	}
	else 	//BM1û�а���--����ԭ��flash��ֵ
	{
		STMFLASH_Read(FLASH_MAX_ADDR, (u16*)LDC_max_v, 8);
		delay_ms(10);

		STMFLASH_Read(FLASH_MIN_ADDR, (u16*)LDC_min_v, 8);
		delay_ms(100);

		//��ʾ��ǰ�����ֵ����Сֵ

	}

	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);						//flash�ɹ���led20����
}
