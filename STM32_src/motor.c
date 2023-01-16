#include "motor.h"

int16_t Speed_control_l = 500;
int16_t Speed_control_r = 500;

int8_t Direc_flag = -1;

int16_t DirectionalOUT = 0;

void Reversible(void)		//初始化电机正反转引脚
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = MOTOR_IN1 | MOTOR_IN2 | MOTOR_IN3 | MOTOR_IN4;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOC, MOTOR_IN1, Bit_SET);						//默认向上
	GPIO_WriteBit(GPIOC, MOTOR_IN2, Bit_SET);
	GPIO_WriteBit(GPIOC, MOTOR_IN3, Bit_SET);
	GPIO_WriteBit(GPIOC, MOTOR_IN4, Bit_SET);
}


void motor_init(void)
{
	Reversible();
	Tim_Pwm_Out_Init();
}

void motor_control(uint8_t ch, uint8_t dir, uint16_t duty)
{
	if (ch == left)	
	{
		if (dir == go)
		{
			GPIO_WriteBit(GPIOC, MOTOR_IN1, Bit_RESET);
			GPIO_WriteBit(GPIOC, MOTOR_IN2, Bit_SET);		
		}
		else if (dir == back)				
		{
			GPIO_WriteBit(GPIOC, MOTOR_IN1, Bit_SET);
			GPIO_WriteBit(GPIOC, MOTOR_IN2, Bit_RESET);
		}
		TIM3->CCR1 = duty;
	}
	else if (ch == right)
	{
		if (dir == go)
		{
			GPIO_WriteBit(GPIOC, MOTOR_IN3, Bit_SET);
			GPIO_WriteBit(GPIOC, MOTOR_IN4, Bit_RESET);	
						
		}
		else if (dir == back)
		{
			GPIO_WriteBit(GPIOC, MOTOR_IN3, Bit_RESET);
			GPIO_WriteBit(GPIOC, MOTOR_IN4, Bit_SET);	
		}
		TIM3->CCR2 = duty;
	}	
}

void SetMotorVoltage(int16_t fLeftVoltage, int16_t fRightVoltage)
{				
	if (fLeftVoltage > 0)
	{
		motor_control(left, go, fLeftVoltage);
	}
	else
	{
		fLeftVoltage = 0 - fLeftVoltage;
		motor_control(left, back, fLeftVoltage);
	}

	if (fRightVoltage > 0)
	{
		motor_control(right, go, fRightVoltage);
	}
	else
	{
		fRightVoltage = 0 - fRightVoltage;
		motor_control(right, back, fRightVoltage);
	}                    

}

void MotorOutput(void)
{
	int16_t fLeftVal, fRightVal;
			
//	if (flag_speed)
//		Speed_control = -300;
//	else
//		Speed_control = 0;
	
	fLeftVal = (int16_t)(Direc_flag *(Speed_control_l + DirectionalOUT));
	fRightVal = (int16_t)(Direc_flag *(Speed_control_r - DirectionalOUT));
		
	if (fLeftVal > 0) 
		fLeftVal += MOTOR_OUT_DEAD_VAL;
	else if (fLeftVal < 0) 
		fLeftVal -= MOTOR_OUT_DEAD_VAL;
		
	if (fRightVal > 0) 
		fRightVal += MOTOR_OUT_DEAD_VAL;
	else if (fRightVal < 0) 
		fRightVal -= MOTOR_OUT_DEAD_VAL;
		
	if (fLeftVal > MOTOR_OUT_MAX)
		fLeftVal = MOTOR_OUT_MAX;
	if (fLeftVal < MOTOR_OUT_MIN)
		fLeftVal = MOTOR_OUT_MIN;
		
	if (fRightVal > MOTOR_OUT_MAX)
		fRightVal = MOTOR_OUT_MAX;
	if (fRightVal < MOTOR_OUT_MIN)
		fRightVal = MOTOR_OUT_MIN;
		
	SetMotorVoltage(fLeftVal, fRightVal);
}

void motor_stop(void)																								//锁死电机
{
	GPIO_WriteBit(GPIOC, MOTOR_IN1, Bit_RESET);
	GPIO_WriteBit(GPIOC, MOTOR_IN2, Bit_RESET);
	GPIO_WriteBit(GPIOC, MOTOR_IN3, Bit_RESET);
	GPIO_WriteBit(GPIOC, MOTOR_IN4, Bit_RESET);
}









