#ifndef __MOTOR_H__
#define __MOTOR_H__


#include <stdint.h>
#include "tim_pwm_out.h"
#include "Directional_control.h"	



#define left 0x06			//×ó
#define right 0x03

#define go 1					//前
#define back 0

#define MOTOR_OUT_DEAD_VAL 0								//电机的死区
#define MOTOR_OUT_MAX 900										//最大95%pwm
#define MOTOR_OUT_MIN -900


#define MOTOR_IN1 			GPIO_Pin_3
#define MOTOR_IN2 			GPIO_Pin_2
#define MOTOR_IN3				GPIO_Pin_0
#define MOTOR_IN4 			GPIO_Pin_13


extern int8_t Direc_flag;
extern int16_t DirectionalOUT;


void motor_init(void);
void motor_control(uint8_t ch, uint8_t dir, uint16_t duty);
void MotorOutput(void);
void SetMotorVoltage(int16_t fLeftVoltage, int16_t fRightVoltage);
void motor_stop(void);







#endif

