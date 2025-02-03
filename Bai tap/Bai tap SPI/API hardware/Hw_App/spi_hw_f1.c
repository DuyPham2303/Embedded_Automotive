#include "stm32f1_spl_spi.h"
#include "timer.h"
#include <string.h>
#define CS_Pin					GPIO_Pin_12
#define SCK_Pin					GPIO_Pin_13//GPIO_Pin_5
#define MISO_Pin				GPIO_Pin_14//GPIO_Pin_6	
#define MOSI_Pin				GPIO_Pin_15//GPIO_Pin_7
#define SPI_RCC					RCC_APB1Periph_SPI2//RCC_APB2Periph_SPI1
#define SPI_PORT_RCC		RCC_APB2Periph_GPIOB
#define SPI_PORT				GPIOB
#define SPI_SOURCE			SPI2
void GPIO_Config(void);
void SPI_Config(void);
uint8_t buff_tx[20];
uint8_t buff_rx[20];

SPI_HandleTypeDef spi2;
int main(){
	GPIO_Config();
	SPI_Config();
	while(1){
		SPI_NSSInternalSoftwareConfig(SPI_SOURCE,SPI_NSSInternalSoft_Reset);
		SPL_SPI_RxTx(&spi2,buff_tx,buff_rx,sizeof(buff_rx));
		SPI_NSSInternalSoftwareConfig(SPI_SOURCE,SPI_NSSInternalSoft_Set);
	}
}

void GPIO_Config(){
	RCC_APB2PeriphClockCmd(SPI_PORT_RCC,ENABLE);
	RCC_APB1PeriphClockCmd(SPI_RCC, ENABLE);
	
	GPIO_InitTypeDef slave_gpio_init;
	slave_gpio_init.GPIO_Pin = MOSI_Pin | MISO_Pin | SCK_Pin;
	slave_gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	slave_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT,&slave_gpio_init);
}

void SPI_Config(){
	spi2.pSPIx = SPI_SOURCE;
	spi2.Init.SPI_Mode = SPI_Mode_Slave;
	spi2.Init.SPI_Direction =  SPI_Direction_2Lines_FullDuplex;
	spi2.Init.SPI_DataSize = SPI_DataSize_8b;
	spi2.Init.SPI_CPOL = SPI_CPOL_Low;
	spi2.Init.SPI_CPHA = SPI_CPHA_1Edge;
	spi2.Init.SPI_FirstBit = SPI_FirstBit_MSB;
	spi2.Init.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI_SOURCE,&spi2.Init);
	SPI_Cmd(SPI_SOURCE,ENABLE);

}