#ifndef STM32F1_SW_SPI_
#define STM32F1_SW_SPI_
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define SPI_CS_Pin GPIO_Pin_12
#define SPI_SCK_Pin GPIO_Pin_13
#define SPI_MISO_Pin GPIO_Pin_14
#define SPI_MOSI_Pin GPIO_Pin_15

#define SPI_PORT 	GPIOB
#define SPI_RCC 	RCC_APB2Periph_GPIOB

void SPI_Sw_Transmit(uint8_t* pTxData,uint16_t Size);
void SPI_Sw_Receive(uint8_t* pRxData,uint16_t Size);
#endif
