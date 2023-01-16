#include "RFID.h"


u8 Uart2RxBuf[UART2_RX_BUF_LEN];
u8 Uart2RxDataConut = 0;
u8 Rx2Flag = 0;


void uart2_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ��USART2
	USART_DeInit(USART2); //��λ����1

	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound; //һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //�����ж�

	USART_Cmd(USART2, ENABLE); //ʹ�ܴ���
}

void RFID_Init(void)
{
	uart2_init(9600);
}


/* Usart2 Interrupt Function */
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		Uart2RxBuf[Uart2RxDataConut] = USART_ReceiveData(USART2);
		Uart2RxDataConut++;
		
		if ((Uart2RxDataConut == 22) && (Uart2RxBuf[0] == 0x04))//�������
		{
			Uart2RxDataConut = 0;
			Rx2Flag = 1;
		}
	}
}


u8 ID_Switch(void)
{
	if(Uart2RxBuf[20] >0 && Uart2RxBuf[20] <60)
	{
		return Uart2RxBuf[20];
	}
	return 0;
}
