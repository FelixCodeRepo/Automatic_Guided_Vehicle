#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "stm32f10x.h"
#include "Path_Recognition.h"
#include "Directional_control.h"
#include "Bluetooth.h"
#include "motor.h"
#include "my_led.h"
#include "my_key.h"
#include "usart.h"
#include "Mag_Sen.h"
#include "RFID.h"

extern u8 TX_Date[DATA_Len];

void Event_Loop( void );
void Simulation_multithreading( void );

#endif
