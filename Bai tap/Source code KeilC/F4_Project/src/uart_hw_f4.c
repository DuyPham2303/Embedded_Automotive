#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
void delay_ms(uint32_t ms);
void RCC_Config(void);
void GPIO_Config(void);
void TIM_Config(void);
void UART_HW_Init(void);
void USART1_TransmitByte(uint8_t byte);
uint8_t USART1_ReceiveByte(void);
void USART_Send(uint8_t* pdata,uint16_t len);
#define TX_PIN 			GPIO_Pin_10
#define RX_PIN 			GPIO_Pin_11
#define MY_USART		USART3
#define MY_TIMER		TIM2
#define RCC_USART		RCC_APB1Periph_USART3 
#define RCC_TIM			RCC_APB1Periph_TIM2
#define GPIO_AF			GPIO_AF_USART3
#define GPIO_AF_PIN	GPIO_PinSource7
#define USART_PORT	GPIOB

uint8_t data[] = {1,2,3,4,56,7,8,9};
char str[] = "hello world";
char buff[20];
uint8_t len = sizeof(data) / sizeof(data[0]);
uint8_t cnt = 0;
uint8_t buff_rx
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	UART_HW_Init();
	while(1){
		//sprintf(buff,"%s:%d\n",str,cnt++);
		//USART_Send((uint8_t*)buff,strlen(buff));
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
		delay_ms(1000);
	}
}
void RCC_Config(){
	RCC_APB1PeriphClockCmd(RCC_USART | RCC_TIM,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_USART,ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD,ENABLE);
}
void UART_HW_Init(){
	USART_InitTypeDef uart_hw_init;
	uart_hw_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart_hw_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_hw_init.USART_Parity = USART_Parity_No;    
	uart_hw_init.USART_StopBits =  USART_StopBits_1;
	uart_hw_init.USART_WordLength = USART_WordLength_8b;
	uart_hw_init.USART_BaudRate = 9600;
	
	USART_Init(MY_USART,&uart_hw_init);
	USART_Cmd(MY_USART,ENABLE);
}
void GPIO_Config(){
		GPIO_InitTypeDef uart_gpio_init,ledpin;
		uart_gpio_init.GPIO_Pin = TX_PIN;
		uart_gpio_init.GPIO_Mode = GPIO_Mode_AF;
		uart_gpio_init.GPIO_OType = GPIO_OType_PP;
		uart_gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
		uart_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		
		//TX config
		GPIO_Init(USART_PORT,&uart_gpio_init);
	
		uart_gpio_init.GPIO_Pin = RX_PIN;
		uart_gpio_init.GPIO_Mode = GPIO_Mode_IN;
		uart_gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
		uart_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		
		//Rx config
		GPIO_Init(USART_PORT,&uart_gpio_init);
	
		GPIO_PinAFConfig(USART_PORT,GPIO_AF_PIN,GPIO_AF);
		GPIO_PinAFConfig(USART_PORT,GPIO_AF_PIN,GPIO_AF);
	
		ledpin.GPIO_Pin = GPIO_Pin_12;
		ledpin.GPIO_Mode = GPIO_Mode_OUT;
		ledpin.GPIO_OType = GPIO_OType_PP;
		ledpin.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOD,&ledpin);
}

uint8_t USART_ReceiveByte(void){
	uint8_t temp = 0x00;
    // Wait until data is received (RXNE flag is set)
    while (USART_GetFlagStatus(MY_USART, USART_FLAG_RXNE) == RESET);

    // Read the received data
	temp = USART_ReceiveData(MY_USART);
	return temp;
}

void USART_TransmitByte(uint8_t byte) {
    // Wait until the transmit data register is empty (TXE flag is set)
    while (USART_GetFlagStatus(MY_USART, USART_FLAG_TXE) == RESET);

    // Transmit the byte
    USART_SendData(MY_USART, byte);

    // Wait until transmission is complete (TC flag is set)
    while (USART_GetFlagStatus(MY_USART, USART_FLAG_TC) == RESET);
		
}
void USART_Send(uint8_t* pdata,uint16_t len){
	for(int i = 0 ; i < len ; i++){
			USART_TransmitByte(pdata[i]);
	}
}
void delay_ms(uint32_t ms){
	TIM_SetCounter(MY_TIMER,0);
	while(TIM_GetCounter(MY_TIMER) < ms);
}
void TIM_Config(){
	TIM_TimeBaseInitTypeDef Timer2_Init;
	Timer2_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer2_Init.TIM_Prescaler = 26920 - 1; // i.063 ms per clk
	Timer2_Init.TIM_Period = 0xffffffff; 
	Timer2_Init.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(MY_TIMER,&Timer2_Init);
	TIM_Cmd(MY_TIMER,ENABLE);
}