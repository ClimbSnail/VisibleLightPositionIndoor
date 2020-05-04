#ifndef __TIMER_H
#define __TIMER_H	 
#include "sys.h"

#define LED1_PWM TIM3->CCR2
#define LED2_PWM TIM3->CCR3
#define LED3_PWM TIM3->CCR4


extern u8 start;
void TIM1_PWM_Init(u16 arr,u16 psc);

void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);

#endif

