#ifndef __RFID_H
#define __RFID_H

#include "stm32f10x.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "stm32f10x_usart.h"
#include "sys.h"


#define UART2_RX_BUF_LEN	22


extern u8 Rx2Flag;

void RFID_Init(void);
u8 ID_Switch(void);

#endif //__RFID_H

