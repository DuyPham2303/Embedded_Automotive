#include "stm32f4_sw_spi.h"
#include "timer.h"
#include <stdio.h>
void GPIO_Config(void);
void SPI_Config(void);
uint8_t cnt = 0;
char str[] = "hello world";
uint8_t buff_tx[20];
uint8_t buff_rx[20];
int main(){
GPIO_Config();
SPI_Config();
TIM_Config();
	while(1){
		sprintf((char*)buff_tx,"%s:%d",str,cnt++);
		SPI_Sw_Transmit(buff_tx,sizeof(buff_tx));
		//GPIO_ToggleBits(SPI_PORT,SPI_CS_Pin);
		delay_ms(1000);
}	
} 

void GPIO_Config(){
	RCC_AHB1PeriphClockCmd(SPI_PORT_RCC, ENABLE);
	GPIO_InitTypeDef MasterGPIO_Init;
	MasterGPIO_Init.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	MasterGPIO_Init.GPIO_Mode =  GPIO_Mode_OUT;
	MasterGPIO_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	MasterGPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT, &MasterGPIO_Init);
	
	MasterGPIO_Init.GPIO_Pin = SPI_MISO_Pin;
	MasterGPIO_Init.GPIO_Mode = GPIO_Mode_IN;
	MasterGPIO_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	MasterGPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT, &MasterGPIO_Init);
}
void SPI_Config(){
	GPIO_WriteBit(SPI_PORT, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_PORT, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_PORT, SPI_MISO_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_PORT, SPI_MOSI_Pin, Bit_RESET);
}
