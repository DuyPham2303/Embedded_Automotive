#include "stm32f4_spl_spi.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>
void GPIO_Config(void);
void SPI_Config(void);

uint8_t cnt = 0;
char str[] = "Pham Cao Duy";
uint8_t buff_tx[20];
uint8_t buff_rx[20];

SPI_HandleTypeDef spi2;
int main(){
	GPIO_Config();
	SPI_Config();
	TIM_Config();
	while(1){
		sprintf((char*)buff_tx,"%s:%d\n",str,cnt++);
		SPL_SPI_TxRx(&spi2,buff_tx,buff_rx,sizeof(buff_tx));
		delay_ms(1000);
	}
}
void GPIO_Config(){
	RCC_AHB1PeriphClockCmd(SPI_PORT_RCC,ENABLE);
	RCC_APB1PeriphClockCmd(SPI_RCC,ENABLE);
	
	GPIO_InitTypeDef masterGPIO_Init;
	masterGPIO_Init.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_MISO_Pin;
	masterGPIO_Init.GPIO_Mode =  GPIO_Mode_AF;
	masterGPIO_Init.GPIO_OType = GPIO_OType_PP;
	//masterGPIO_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	masterGPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT, &masterGPIO_Init);
	
	
	GPIO_PinAFConfig(SPI_PORT,GPIO_PinSource15, GPIO_AF_SPI2);
	GPIO_PinAFConfig(SPI_PORT,GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(SPI_PORT,GPIO_PinSource14, GPIO_AF_SPI2);
}

void SPI_Config(){
	spi2.pSPIx = SPI_SOURCE;
	spi2.Init.SPI_Mode =  SPI_Mode_Master;
	spi2.Init.SPI_Direction =  SPI_Direction_2Lines_FullDuplex;
	spi2.Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; 
	spi2.Init.SPI_CPHA = SPI_CPHA_1Edge;
	spi2.Init.SPI_CPOL = SPI_CPOL_Low;
	spi2.Init.SPI_DataSize = SPI_DataSize_8b;
	spi2.Init.SPI_FirstBit = SPI_FirstBit_MSB;
	spi2.Init.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI_SOURCE,&spi2.Init);
	SPI_Cmd(SPI_SOURCE,ENABLE);
	
}
