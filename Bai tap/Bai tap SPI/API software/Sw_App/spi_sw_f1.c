#include "stm32f1_sw_spi.h"
#include "timer.h"
void GPIO_Config(void);
void SPI_Config(void);
uint8_t buff_rx[20];
uint8_t reset = 0;
int main(){
	GPIO_Config();
	SPI_Config();
	while(1){
		SPI_Sw_Receive(buff_rx,sizeof(buff_rx));
	}
}

void GPIO_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	
	GPIO_InitTypeDef spi_slaveInit;
	spi_slaveInit.GPIO_Pin = SPI_MISO_Pin;
	spi_slaveInit.GPIO_Mode = GPIO_Mode_Out_PP;
	spi_slaveInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT, &spi_slaveInit);	
	
	spi_slaveInit.GPIO_Pin = SPI_MOSI_Pin;
	spi_slaveInit.GPIO_Mode = GPIO_Mode_IPU;
	spi_slaveInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT, &spi_slaveInit);
	
	spi_slaveInit.GPIO_Pin = SPI_CS_Pin | SPI_SCK_Pin;
	spi_slaveInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT, &spi_slaveInit);
}

void SPI_Config(){
	GPIO_WriteBit(SPI_PORT, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_PORT, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_PORT, SPI_MISO_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_PORT, SPI_MOSI_Pin, Bit_RESET);
}
