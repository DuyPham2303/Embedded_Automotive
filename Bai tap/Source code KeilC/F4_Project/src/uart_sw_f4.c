#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum{
	Parity_Mode_NONE,
	Parity_Mode_ODD,
	Parity_Mode_EVEN
}Parity_Mode;

void RCC_Config(void);
void GPIO_Config(void);
void TIM_Config(void);
void UART_SW_init();
void UART_Send_1byte(char data);
void UART_Send(char* data,uint8_t len);
uint8_t UART_Receive_1byte();
uint16_t Parity_Generate(uint8_t data, Parity_Mode Mode);
uint8_t Parity_Check(uint8_t data, Parity_Mode Mode);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void delay_s(uint32_t s);
void clock(uint8_t duration);
uint32_t Get_PCLK1_Freq(void);
uint32_t Get_PCLK2_Freq(void);
uint32_t Get_Timer_Clock_Freq(TIM_TypeDef *TIMx,bool PCLK_SOURCE);


#define TX_PIN					GPIO_Pin_2
#define RX_PIN					GPIO_Pin_3
#define UART_RCC				RCC_AHB1Periph_GPIOA
#define CLK_1bit				 11 //127
#define CLK_1_half_bit	(CLK_1bit + (CLK_1bit/2))
#define CLK_half_bit		(CLK_1bit/2)
#define TIM_SOURCE		TIM1
#define TIM_RCC				RCC_APB2Periph_TIM1

#define  APB1_CLK	0
#define  APB2_CLK	1
uint8_t data[] = {0x01,0x2,0x3,0x40,0x5,0x6};
char str[] = "hello\n";
uint8_t len = sizeof(data) / sizeof(data[0]);
uint8_t cnt = 0;
char buff[20];
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();	
	UART_SW_init();
	while(1){
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
		//delay_us(1);
		clock(CLK_1bit);
	}
	/*
	while(1){
		sprintf(buff,"%s %d\n",str,cnt++);
		UART_Send(str,strlen(str));
		delay_ms(1000);
	}
	*/
}
void RCC_Config(){
	RCC_AHB1PeriphClockCmd(UART_RCC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(TIM_RCC,ENABLE);	
}
void UART_SW_init(){
	GPIO_SetBits(GPIOA,TX_PIN);
	delay_us(1);	
}
void UART_Send_1byte(char data){
	GPIO_ResetBits(GPIOA,TX_PIN);
	clock(CLK_1bit);
	for(uint8_t i = 0 ; i < 8 ; i++){
			if(data & (1 << i)){ // send LSB first
				GPIO_SetBits(GPIOA,TX_PIN); 
			}
			else GPIO_ResetBits(GPIOA,TX_PIN);
			clock(CLK_1bit);
	}
	GPIO_SetBits(GPIOA,RX_PIN);
	clock(CLK_1bit);
}
void UART_Send(char* data,uint8_t len){
	for(uint8_t i = 0 ; i < len ; i++){
		UART_Send_1byte(data[i]);
	}
}
uint8_t UART_Receive_1byte(){
		uint8_t rx_data;
		//Wait for start bit from Tx
		while(GPIO_ReadInputDataBit(GPIOA,RX_PIN));
		clock(CLK_1_half_bit);
		for(uint8_t i = 0 ; i < 8 ;i++){
			if(GPIO_ReadInputDataBit(GPIOA,RX_PIN)){
				rx_data |= 1;
			}
			rx_data <<=1;
			clock(CLK_1bit);
		}
		while(!GPIO_ReadInputDataBit(GPIOA,RX_PIN));
		clock(CLK_half_bit);
		return rx_data;
}	
void GPIO_Config(){
		GPIO_InitTypeDef uart_gpio_init,ledpin;
		uart_gpio_init.GPIO_Pin = TX_PIN;
		uart_gpio_init.GPIO_Mode = GPIO_Mode_OUT;
		uart_gpio_init.GPIO_OType = GPIO_OType_PP;
		uart_gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
		uart_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		
		//TX config
		GPIO_Init(GPIOA,&uart_gpio_init);
	
		uart_gpio_init.GPIO_Pin = RX_PIN;
		uart_gpio_init.GPIO_Mode = GPIO_Mode_IN;
		uart_gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
		uart_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		
		//Rx config
		GPIO_Init(GPIOA,&uart_gpio_init);
	
		ledpin.GPIO_Pin = GPIO_Pin_12;
		ledpin.GPIO_Mode = GPIO_Mode_OUT;
		ledpin.GPIO_OType = GPIO_OType_PP;
		ledpin.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOD,&ledpin);
}
uint16_t Parity_Generate(uint8_t data, Parity_Mode Mode){
	uint8_t count = 0;
	uint8_t data1 = data;
	for(int i = 0; i < 8; i++){
		if(data1 & 0x01){
			count++;
		}
		data1 >>= 1;
	}
	switch(Mode){
		case Parity_Mode_NONE:
			return data; 
			break;
		case Parity_Mode_ODD:
			if(count % 2){
				return ((data << 1) | 1);
			} else {
				return (data<<1);
			}
			break;
		case Parity_Mode_EVEN:
			if(!(count % 2)){
				return ((data << 1) | 1);
			} else {
				return (data << 1);
			}
			break;
		default:
			return data;
			break;
	}
}
uint8_t Parity_Check(uint8_t data, Parity_Mode Mode){
	uint8_t count = 0;
	for(int i = 0; i < 8; i++){
		if(data & 0x01){
			count++;
		}
		data >>= 1;
	}
	switch(Mode){
		case Parity_Mode_NONE:{
			return 1; 
			break;
		}
		case Parity_Mode_ODD:{
			return (count % 2);
			break;
		}
		case Parity_Mode_EVEN:{
			return (!(count % 2));
			break;
		}
		default:{
			return 0;
			break;
		}
	}
}
void TIM_Config(){
	TIM_TimeBaseInitTypeDef Timer1_Init;
	Timer1_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer1_Init.TIM_Prescaler = 538 - 1 ; // 1us
	Timer1_Init.TIM_Period = 0xffff; 
	Timer1_Init.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM_SOURCE,&Timer1_Init);
	TIM_Cmd(TIM_SOURCE,ENABLE);
	
}
void delay_us(uint32_t us){
	TIM_SetCounter(TIM_SOURCE,0);
	while(TIM_GetCounter(TIM_SOURCE) < us);
}
void delay_ms(uint32_t ms){
	for(uint32_t i = 0 ; i < ms*1000 ; i++){
		delay_us(1);
	}
}
void delay_s(uint32_t s){
	for(uint32_t i = 0 ; i < s*1000 ; i++){
		delay_ms(1);
	}
}
void clock(uint8_t duration){
	for(uint8_t i = 0 ; i < duration ; i++){
		delay_us(1);
	}
}