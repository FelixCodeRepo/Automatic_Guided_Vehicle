#ifndef __DIRECTIONAL_CONTROL_H__
#define __DIRECTIONAL_CONTROL_H__


#include "Parameter_List.h"
#include "motor.h"

#define CAR_POSITION_MAX	(100)
#define CAR_POSITION_MIN	(-100) 


extern PID PID_Directional;
extern float Directional_err;

extern PID PID_Dir;
extern float Dir_err;

void Directional_control( float actual_Directional, float Ideal_Directional );
void Dir_control( float actual_Directional, float Ideal_Directional );
void Directional_pid_init( void );


#endif
