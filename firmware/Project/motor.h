#ifndef MOTOR_H__
#define MOTOR_H__

#include "menu.h"

void Motor_SetMicrostepping(uint8_t microstep);
void Motor_SetDirection(int8_t dir);
void Motor_SetEnabled(uint8_t en);
void Motor_Init(void);
void Motor_Stop(void);
void Motor_Start(int steps, uint32_t freq);

void Motor_ItemStartStop(struct MenuItem* item);

#endif