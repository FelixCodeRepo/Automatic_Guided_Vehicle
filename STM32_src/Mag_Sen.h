#ifndef __MAGIC_SEN_H
#define __MAGIC_SEN_H

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "stm32f10x_usart.h"

#include "stdint.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "sys.h"
#include <stdbool.h>
#include "my_key.h"
#include "stmflash.h"

/***********************************************/


extern uint8_t mag_data[8];
extern uint8_t LDC_max_v[8];
extern uint8_t LDC_min_v[8];


void Mag_Sen_Init(void);
bool sen_get_data(u8 *buf);

void Calculate_k(void);
void flash_init(void);

#endif
