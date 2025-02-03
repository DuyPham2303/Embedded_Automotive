#include "timer.h"
void TIM_Config(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseInitTypeDef timer_init;
	timer_init.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Prescaler = 72 - 1;
	timer_init.TIM_Period = 0xffff;

	TIM_TimeBaseInit(TIM_SOURCE,&timer_init);
	TIM_Cmd(TIM_SOURCE,ENABLE);
}
void delay_ms(uint32_t ms){
	TIM_SetCounter(TIM_SOURCE,0);
	while(TIM_GetCounter(TIM_SOURCE) < 10*ms);
}

