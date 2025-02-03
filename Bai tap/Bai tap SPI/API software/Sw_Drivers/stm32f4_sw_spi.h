#ifndef STM32F4_SW_SPI_
#define STM32F4_SW_SPI_
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f4xx_rcc.h"
#define SPI_CS_Pin 		GPIO_Pin_12
#define SPI_SCK_Pin 	GPIO_Pin_13
#define SPI_MISO_Pin 	GPIO_Pin_14
#define SPI_MOSI_Pin 	GPIO_Pin_15

#define SPI_PORT 		 GPIOB
#define SPI_PORT_RCC RCC_AHB1Periph_GPIOB

void SPI_Sw_Transmit(uint8_t* pTxData,uint16_t Size);
void SPI_Sw_Receive(uint8_t* pRxData,uint16_t Size);

#endif