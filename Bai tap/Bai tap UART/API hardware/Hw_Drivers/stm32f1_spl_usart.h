#ifndef	STM32F1_SPL_USART_H
#define STM32F1_SPL_USART_H
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

typedef struct{
USART_TypeDef* pUSARTx;
USART_InitTypeDef USART_Config;
}USART_Handle;
void SPL_USART_Send(USART_Handle* pUSARTHandle,uint8_t* pTxData,uint16_t Size);
void SPL_USART_Receive(USART_Handle* pUSARTHandle,uint8_t* pRxData,uint16_t Size);
#endif
