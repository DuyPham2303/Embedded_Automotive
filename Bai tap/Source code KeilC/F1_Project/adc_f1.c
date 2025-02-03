#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "kalman.h"
#include <stdint.h>
void RCC_Config(void);
void GPIO_Config(void);
void ADC_Config(void);
void TIM_Config(void);
void delay_ms(uint32_t ms, uint8_t option);
#define RCC_TIM				RCC_APB2Periph_TIM1
#define RCC_ADC				RCC_APB2Periph_ADC1
#define RCC_GPIO			RCC_APB2Periph_GPIOA
#define ADC_SOURCE		ADC1
#define TIM_SOURCE		TIM1
#define ADC_PORT			GPIOA
#define ADC_PIN				GPIO_Pin_0;

#define US_100				0
#define MS_MODE				1
uint16_t adc_val,medium_val,filter_val;
//uint16_t pre_adc,pre_final;
uint16_t sum;
double k_mea,k_est,k_q;
uint8_t status = 0;
//uint16_t error_adc = 0,error_final = 0;
int main(){
	RCC_Config();
	GPIO_Config();
	ADC_Config();
	TIM_Config();
	while(1){
		if(status){
			for(int i = 0; i < 10; i++){
				adc_val = ADC_GetConversionValue(ADC_SOURCE);
				filter_val = updateEstimate(adc_val);
				delay_ms(1,US_100);
				sum += adc_val;
			}
			medium_val = sum/10;
			delay_ms(100,MS_MODE);
			sum = 0;
		}
		else{
			SimpleKalmanFilter(k_mea,k_est,k_q);
			adc_val = 0;
			medium_val = 0;
		}
	}
}	
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_ADC | RCC_TIM | RCC_GPIO, ENABLE);
}
void GPIO_Config(){
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin = ADC_PIN;
	gpio_init.GPIO_Mode = GPIO_Mode_AIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_PORT,&gpio_init);
}
void ADC_Config(){
	ADC_InitTypeDef adc_init;
	adc_init.ADC_Mode = ADC_Mode_Independent;
	adc_init.ADC_ContinuousConvMode = ENABLE;
	adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc_init.ADC_ScanConvMode = DISABLE;
	adc_init.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init.ADC_NbrOfChannel = 1;
	ADC_Init(ADC_SOURCE,&adc_init);
	
	ADC_RegularChannelConfig(ADC_SOURCE,ADC_Channel_0,1,ADC_SampleTime_1Cycles5);
	ADC_Cmd(ADC_SOURCE,ENABLE);
	ADC_SoftwareStartConvCmd(ADC_SOURCE,ENABLE);
}
void TIM_Config(){
	TIM_TimeBaseInitTypeDef timer_init;
	timer_init.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Prescaler = 7200 - 1; //0.1
	timer_init.TIM_Period = 0xffff;
	
	TIM_TimeBaseInit(TIM_SOURCE,&timer_init);
	TIM_Cmd(TIM_SOURCE,ENABLE);
}

void delay_ms(uint32_t ms ,uint8_t option){
	if(option == MS_MODE) ms*=10; 
	TIM_SetCounter(TIM_SOURCE,0);
	while(TIM_GetCounter(TIM_SOURCE) < ms);
}
