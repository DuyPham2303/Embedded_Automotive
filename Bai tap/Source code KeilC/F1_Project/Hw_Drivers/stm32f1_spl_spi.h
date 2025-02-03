#ifndef	STM32F1_SPL_SPI_H_
#define	STM32F1_SPL_SPI_H_
#include "stm32f10x.h"                  
#include "stm32f10x_spi.h" 
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

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
