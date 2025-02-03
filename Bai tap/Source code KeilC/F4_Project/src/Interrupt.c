#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_exti.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include <misc.h>
#include <stdint.h>
#include <stdbool.h>
void RCC_Config(void);
void GPIO_Config(void);
void NVIC_Config(void);
void EXTI_Config(void);
void TIM_Config(void);

uint16_t press = 0;
volatile uint16_t tim_cnt_1ms = 0,tim_cnt_1s = 0;
int main(){
RCC_Config();
GPIO_Config();
NVIC_Config();
//EXTI_Config();
TIM_Config();
	while(1){
			if((tim_cnt_1s % 2)){
			GPIO_WriteBit(GPIOD,GPIO_Pin_14,Bit_SET);
			}
			else 	GPIO_WriteBit(GPIOD,GPIO_Pin_14,Bit_RESET);
	}
	//return 0;
}
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){	  
				press += 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}
void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		tim_cnt_1ms++;
		if(tim_cnt_1ms == 10){
			tim_cnt_1s++;
			tim_cnt_1ms = 0;
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

void RCC_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
}
void GPIO_Config()
{
	GPIO_InitTypeDef ledpin_Init,buttonpin_Init;
	
	ledpin_Init.GPIO_Pin = GPIO_Pin_14;
	ledpin_Init.GPIO_Speed = GPIO_Speed_50MHz;
	ledpin_Init.GPIO_OType = GPIO_OType_PP;
	ledpin_Init.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD,&ledpin_Init);
	
	buttonpin_Init.GPIO_Pin = GPIO_Pin_0;
	buttonpin_Init.GPIO_Mode = GPIO_Mode_IN;
	buttonpin_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	buttonpin_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&buttonpin_Init);

}
void TIM_Config(){
	TIM_TimeBaseInitTypeDef Timer2_Init;
	Timer2_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer2_Init.TIM_Prescaler = 26920 - 1; 
	Timer2_Init.TIM_Period = 100; 
	Timer2_Init.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2,&Timer2_Init);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	
}
void EXTI_Config(){
	EXTI_InitTypeDef EXTI0_Init;
	EXTI0_Init.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI0_Init.EXTI_Line = EXTI_Line0;
	EXTI0_Init.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI0_Init.EXTI_LineCmd = ENABLE;

	EXTI_Init(&EXTI0_Init); 

}
void NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}	
