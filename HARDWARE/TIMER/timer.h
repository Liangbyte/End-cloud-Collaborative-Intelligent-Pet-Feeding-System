#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void Timer_SRD_Init(u16 arr,u16 psc);//time3定时器设置
void TIM4_Int_Init(u16 arr,u16 psc);//time2



void TIM2_PWM_Init(u16 arr,u16 psc);//PA0 做PWM输出   //arr-定时器周期，psc--定时器时钟分频







#endif

