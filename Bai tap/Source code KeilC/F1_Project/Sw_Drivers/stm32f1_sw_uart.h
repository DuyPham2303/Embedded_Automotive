#ifndef STM32F1_SW_UART_H_
#define	STM32F1_SW_UART_H_
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

#define TX_PIN					GPIO_Pin_2
#define RX_PIN					GPIO_Pin_3
#define CLK_1bit				10 //baudrate = 100us
#define CLK_1_half_bit	(CLK_1bit + (CLK_1bit/2))
#define CLK_half_bit		(CLK_1bit/2)
#define USART_RCC				RCC_APB2Periph_GPIOA 
#define USART_PORT			GPIOA

typedef enum{
	Parity_Mode_NONE,
	Parity_Mode_ODD,
	Parity_Mode_EVEN
}Parity_Mode;

void USART_Sw_Send(uint8_t* pTxData,uint16_t Size);
void USART_Sw_Receive(uint8_t* pRxData,uint16_t Size);
void USART_Sw_Init();
#endif