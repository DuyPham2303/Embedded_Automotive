#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include <stdint.h>

void TIM_Config(void);
void ADC_Config(void);
void GPIO_Config(void);
void RCC_Config(void);
void delay_ms(uint32_t ms);
#define MY_ADC			ADC1
#define ADC_RCC			RCC_APB2Periph_ADC1
#define RCC_TIM			RCC_APB1Periph_TIM2
#define MY_TIMER		TIM2

uint16_t adc_val;
uint16_t Read_ADC(void) {
    // B?t d?u chuy?n d?i ADC
    ADC_SoftwareStartConv(ADC1);
    
    // Ð?i cho d?n khi k?t thúc chuy?n d?i
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    
    // Ð?c giá tr? ADC
    return ADC_GetConversionValue(ADC1);
}
uint8_t cnt;
int main(){
	ADC_Config();
	GPIO_Config();
	RCC_Config();
	TIM_Config();
	while(1){
		adc_val = ADC_GetConversionValue(MY_ADC);
		GPIO_ToggleBits(GPIOA,GPIO_Pin_12);
		//cnt++;
		delay_ms(1000);
	}
}


void RCC_Config(){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(ADC_RCC,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_TIM,ENABLE);
}
void ADC_Config(){
	ADC_CommonInitTypeDef ADC_CommonInit;
	ADC_CommonInit.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInit.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInit.ADC_Prescaler =  ADC_Prescaler_Div4;
	ADC_CommonInit.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonStructInit(&ADC_CommonInit);
	
	ADC_InitTypeDef	ADC_InitStruct;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_6b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_Init(MY_ADC,&ADC_InitStruct);
	
	ADC_RegularChannelConfig(MY_ADC,ADC_Channel_1,1,ADC_SampleTime_3Cycles);
	ADC_Cmd(MY_ADC,ENABLE);
	ADC_SoftwareStartConv(MY_ADC);
}
void GPIO_Config(){
	GPIO_InitTypeDef adc_gpio_init,ledinit;
	adc_gpio_init.GPIO_Mode = GPIO_Mode_AIN;
	adc_gpio_init.GPIO_Pin = GPIO_Pin_1;
	adc_gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&adc_gpio_init);
	
	ledinit.GPIO_Mode = GPIO_Mode_OUT;
	ledinit.GPIO_OType = GPIO_OType_PP;
	ledinit.GPIO_Pin = GPIO_Pin_12;
	ledinit.GPIO_PuPd = GPIO_PuPd_DOWN;
	ledinit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&ledinit);
}
void delay_ms(uint32_t ms){
	TIM_SetCounter(MY_TIMER,0);
	while(TIM_GetCounter(MY_TIMER) < ms * 10);
}
void TIM_Config(){
	TIM_TimeBaseInitTypeDef Timer2_Init;
	Timer2_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer2_Init.TIM_Prescaler = 2692 - 1; 
	Timer2_Init.TIM_Period = 0xffffffff; 
	Timer2_Init.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(MY_TIMER,&Timer2_Init);
	TIM_Cmd(MY_TIMER,ENABLE);
}
