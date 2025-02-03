#ifndef STM32F4_SPL_SPI_H_ 
#define STM32F4_SPL_SPI_H_
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define SPI_CS_Pin		GPIO_Pin_12
#define SPI_SCK_Pin 	GPIO_Pin_13
#define SPI_MISO_Pin 	GPIO_Pin_14
#define SPI_MOSI_Pin 	GPIO_Pin_15

#define SPI_PORT			GPIOB
#define SPI_RCC 			RCC_APB1Periph_SPI2
#define SPI_PORT_RCC	RCC_AHB1Periph_GPIOB
#define SPI_SOURCE		SPI2

typedef struct
{
  SPI_TypeDef    *pSPIx;   
  SPI_InitTypeDef  Init;   
}SPI_HandleTypeDef;

void SPL_SPI_Transmit(SPI_HandleTypeDef *hspi, void* pTxData,uint16_t Size);
void SPL_SPI_Receive(SPI_HandleTypeDef *hspi,void* pRxData,uint16_t Size);
void SPL_SPI_TxRx(SPI_HandleTypeDef* hspi,uint8_t* pTxData,uint8_t* pRxData,uint16_t Size);
void SPL_SPI_RxTx(SPI_HandleTypeDef* hspi,uint8_t* pTxData,uint8_t* pRxData,uint16_t Size);
#endif