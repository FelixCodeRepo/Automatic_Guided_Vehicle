#ifndef _PATH_RECOGNITION_H__
#define _PATH_RECOGNITION_H__




#include "adc.h"
#include "stdlib.h"
#include <stdbool.h>

#include "Mag_Sen.h"
#include "usart.h"
#include "motor.h"
#include "usart.h"

extern float Direction_output;							//·��ʶ�������ֵ--�ⲿ����
extern bool coin_flag;
extern float mag[8];






void route_plan(void);
void Directional_plan(void);







#endif
