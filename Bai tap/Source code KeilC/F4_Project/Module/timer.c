#include "timer.h"
void TIM_Config(){
	RCC_APB1PeriphClockCmd(TIM_RCC,ENABLE);
	TIM_TimeBaseInitTypeDef TimerInit;
	TimerInit.TIM_ClockDivision =  TIM_CKD_DIV1;
	TimerInit.TIM_Prescaler = 26920 - 1 ; // 1ms
	TimerInit.TIM_CounterMode = TIM_CounterMode_Up;
	TimerInit.TIM_Period = 0xffffffff;
	
	TIM_TimeBaseInit(TIM_SOURCE,&TimerInit);
	TIM_Cmd(TIM_SOURCE,ENABLE);
}
void delay_ms(uint32_t time){
	TIM_SetCounter(TIM_SOURCE,0);
	while(TIM_GetCounter(TIM_SOURCE) < time){}
}


