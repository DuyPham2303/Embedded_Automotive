#ifndef TIMER_H_
#define TIMER_H_
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#define TIM_SOURCE	TIM2
void TIM_Config(void);
void delay_ms(uint32_t ms);
void delay_10us(uint32_t us10);
#endif
