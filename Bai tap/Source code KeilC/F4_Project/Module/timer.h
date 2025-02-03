#ifndef TIMER_H_
#define TIMER_H_
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#define TIM_SOURCE TIM2
#define TIM_RCC		 RCC_APB1Periph_TIM2
void TIM_Config(void);
void delay_ms(uint32_t ms);
#endif 