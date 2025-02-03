#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f1_spl_spi.h"
#include "stm32f10x_tim.h"
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "kalman.h"
#include <stdint.h>
#include <stdbool.h>

void RCC_Config(void);
void GPIO_Config(void);
void ADC_Config(void);
void SPI_Config(void);
void delay_ms(uint32_t ms);
void TIM_Config(void);
void NVIC_Config(void);
//macro ADC
#define RCC_ADC				RCC_APB2Periph_ADC2
#define RCC_PORT_ADC	RCC_APB2Periph_GPIOA
#define ADC_SOURCE		ADC2
#define ADC_PORT			GPIOA
#define ADC_PIN				GPIO_Pin_0

//macro TIMER
#define TIM_SOURCE		TIM2
#define RCC_TIM				RCC_APB1Periph_TIM2
#define PWM_PIN				GPIO_Pin_9
#define PWM_CHANNEL		TIM_OCMode_PWM1

//macro SPI
#define CS_Pin					GPIO_Pin_12//GPIO_Pin_4
#define SCK_Pin					GPIO_Pin_13//GPIO_Pin_5
#define MISO_Pin				GPIO_Pin_14//GPIO_Pin_6	
#define MOSI_Pin				GPIO_Pin_15//GPIO_Pin_7
#define RCC_SPI					RCC_APB1Periph_SPI2//RCC_APB2Periph_SPI1
#define RCC_PORT_SPI		RCC_APB2Periph_GPIOB
#define SPI_PORT				GPIOB
#define SPI_SOURCE			SPI2

uint16_t final_val = 0;
float k_mea = 0.05,k_est = 0.01,k_q = 0.1;
SPI_HandleTypeDef spi2;
volatile bool flag_10ms = false; 
int main(){
	RCC_Config();
	GPIO_Config();
	ADC_Config();
	TIM_Config();
	NVIC_Config();
	SPI_Config();
	SimpleKalmanFilter(k_mea,k_est,k_q);
	while(1){
		if(flag_10ms){
			uint16_t adc_val = ADC_GetConversionValue(ADC_SOURCE);
			final_val = updateEstimate(adc_val);
			SPL_SPI_Transmit(&spi2,&final_val,sizeof(final_val));
			flag_10ms = false;
		}
	}
}	
void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM_SOURCE,TIM_IT_Update) != RESET){
			flag_10ms = true;
	}
	TIM_ClearITPendingBit(TIM_SOURCE,TIM_IT_Update);
}
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_PORT_SPI | RCC_PORT_ADC | RCC_ADC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_SPI | RCC_TIM,ENABLE);
}
void GPIO_Config(){
	GPIO_InitTypeDef gpio_ADCinit,spi_gpio_init;
	
	gpio_ADCinit.GPIO_Pin = ADC_PIN;
	gpio_ADCinit.GPIO_Mode = GPIO_Mode_AIN;
	gpio_ADCinit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_PORT,&gpio_ADCinit);
	
	spi_gpio_init.GPIO_Pin = MOSI_Pin | MISO_Pin | SCK_Pin;
	spi_gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	spi_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT,&spi_gpio_init);
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
	
	ADC_RegularChannelConfig(ADC_SOURCE,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
	ADC_Cmd(ADC_SOURCE,ENABLE);
	ADC_SoftwareStartConvCmd(ADC_SOURCE,ENABLE);
}
void NVIC_Config(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_timerInit;
	NVIC_timerInit.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_timerInit.NVIC_IRQChannelSubPriority = 0;
	NVIC_timerInit.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_timerInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_timerInit);
}
void SPI_Config(){
	spi2.pSPIx = SPI_SOURCE;
	spi2.Init.SPI_Mode = SPI_Mode_Master;
	spi2.Init.SPI_Direction =  SPI_Direction_2Lines_FullDuplex;
	spi2.Init.SPI_DataSize = SPI_DataSize_16b;
	spi2.Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	spi2.Init.SPI_CPOL = SPI_CPOL_Low;
	spi2.Init.SPI_CPHA = SPI_CPHA_1Edge;
	spi2.Init.SPI_FirstBit = SPI_FirstBit_MSB;
	spi2.Init.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(spi2.pSPIx,&spi2.Init);
	SPI_Cmd(spi2.pSPIx,ENABLE);
}


void TIM_Config(){
	TIM_TimeBaseInitTypeDef timer_init;
	timer_init.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Prescaler = 7200 - 1; //0.1
	timer_init.TIM_Period = 100; // 10ms timer
	TIM_TimeBaseInit(TIM_SOURCE,&timer_init);
	
	TIM_Cmd(TIM_SOURCE,ENABLE);
	TIM_ITConfig(TIM_SOURCE,TIM_IT_Update,ENABLE);
}
void delay_ms(uint32_t ms){
	TIM_SetCounter(TIM_SOURCE,0);
	while(TIM_GetCounter(TIM_SOURCE) < ms * 10);
}










