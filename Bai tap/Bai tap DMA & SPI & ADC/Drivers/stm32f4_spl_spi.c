#include "stm32f4_spl_spi.h"
#include <stdint.h>
void SPL_SPI_Transmit(SPI_HandleTypeDef *hspi,void* pTxData,uint16_t Size){
	if(hspi->Init.SPI_DataSize == SPI_DataSize_16b){
		uint16_t* pTxData16b = (uint16_t*)pTxData;
		while(Size > 1){	
				while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(hspi->pSPIx,*pTxData16b);
				Size -= 2;
				pTxData16b++;
			}
	}
	else{
		uint8_t* pTxData8b = (uint8_t*)pTxData;
		while(Size > 0){
			while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_TXE) == RESET);
			SPI_I2S_SendData(hspi->pSPIx,*pTxData8b);
			Size--;
			pTxData8b++;
		}
	}
	while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_BSY) == SET);
}
void SPL_SPI_Receive(SPI_HandleTypeDef* hspi,void* pRxData,uint16_t Size){
	SPI_NSSInternalSoftwareConfig(hspi->pSPIx,SPI_NSSInternalSoft_Reset);
	if(hspi->Init.SPI_DataSize == SPI_DataSize_16b){
		uint16_t* pRxData16b = (uint16_t*)pRxData;
		while(Size > 1){	
				while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_RXNE) == RESET);
				*pRxData16b = SPI_I2S_ReceiveData(hspi->pSPIx);
				Size -= 2;
				pRxData16b++;
			}
	}
	else{
		uint8_t* pRxData8b = (uint8_t*)pRxData;
		while(Size > 0){
			while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_RXNE) == RESET);
			*pRxData8b = (uint8_t)SPI_I2S_ReceiveData(hspi->pSPIx);
			Size--;
			pRxData8b++;
		}
	}
	SPI_NSSInternalSoftwareConfig(hspi->pSPIx,SPI_NSSInternalSoft_Set);
}
void SPL_SPI_TxRx(SPI_HandleTypeDef* hspi,uint8_t* pTxData,uint8_t* pRxData,uint16_t Size){
	while(Size > 0){
		while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(hspi->pSPIx,*pTxData);
		while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_RXNE) == RESET);
		*pRxData = (uint8_t)SPI_I2S_ReceiveData(hspi->pSPIx);
		pTxData++,pRxData++;
		Size--;
	}
	while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_BSY) == SET);
}
void SPL_SPI_RxTx(SPI_HandleTypeDef* hspi,uint8_t* pTxData,uint8_t* pRxData,uint16_t Size){
	while(Size > 0){
			while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_RXNE) == RESET);
			*pRxData = (uint8_t)SPI_I2S_ReceiveData(hspi->pSPIx);
			while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_TXE) == RESET);
			SPI_I2S_SendData(hspi->pSPIx,*pTxData);
			pTxData++,pRxData++;
			Size--;
	}
	while(SPI_I2S_GetFlagStatus(hspi->pSPIx,SPI_I2S_FLAG_BSY) == SET);
}
