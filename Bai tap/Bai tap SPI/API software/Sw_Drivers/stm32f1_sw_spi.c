#include "stm32f1_sw_spi.h"
#include "timer.h"
static void Clock(){
	GPIO_WriteBit(SPI_PORT, SPI_SCK_Pin, Bit_SET);
	delay_ms(5);
	GPIO_WriteBit(SPI_PORT, SPI_SCK_Pin, Bit_RESET);
	delay_ms(5);
}
static void SPI_Send_1byte(uint8_t data_tx){
uint8_t u8Mask = 0x80;	// 0b10000000
	uint8_t tempData;
	//pull low CS pin to start transmission process
	GPIO_WriteBit(SPI_PORT, SPI_CS_Pin,Bit_RESET);
	delay_ms(1);
	for(int i = 0; i < 8; i++){
	//read current bit 
		tempData = data_tx & u8Mask;
	//if current bit is not 0 -> send bit high on MOSI pin
		if(tempData){
			GPIO_WriteBit(SPI_PORT, SPI_MOSI_Pin, Bit_SET);
			delay_ms(5); // wait a bit until bit is sent successfully
		} else{
			GPIO_WriteBit(SPI_PORT, SPI_MOSI_Pin, Bit_RESET);
			delay_ms(5);
		}
		data_tx <<= 1;
	//generate clock for sending next bit
		Clock();
	}
	//release CS pin to high to end transmission process
	GPIO_WriteBit(SPI_PORT, SPI_CS_Pin,Bit_SET);;
	delay_ms(1);
}
static uint8_t SPI_Receive_1byte(void){
	uint8_t data_rx = 0x00;	//0b0000 0000
	uint8_t temp = 0x00;
	//check whether CS pin is driven LOW or not
	while(GPIO_ReadInputDataBit(SPI_PORT, SPI_CS_Pin) == SET);
	//check whether SCK line produce HIGH pulse or not
	while(GPIO_ReadInputDataBit(SPI_PORT, SPI_SCK_Pin) == RESET);
	for(int i = 0; i < 8; i++){ 
	//check again to ensure that CLK is high -> data is arrived -> can read
		if(GPIO_ReadInputDataBit(SPI_PORT, SPI_SCK_Pin)){
	//check SCK line continuously until it it produces LOW state
			/*
			while (GPIO_ReadInputDataBit(SPI_PORT, SPI_SCK_Pin)){
				temp = GPIO_ReadInputDataBit(SPI_PORT, SPI_MOSI_Pin);
			}
			*/
			data_rx <<= 1; //shift left to add new bit
	//copy to data receive
			data_rx |= temp; // receive next bit 
    }
	//wait until next CLK high is generated to continue read data
		while(!GPIO_ReadInputDataBit(SPI_PORT, SPI_SCK_Pin));
	}
	//wait until CS pin is relased to high -> end of data transaction
	while(GPIO_ReadInputDataBit(SPI_PORT, SPI_CS_Pin) == RESET);
	return data_rx;
}
void SPI_Sw_Transmit(uint8_t* pTxData,uint16_t Size){
	while(Size > 0){
		SPI_Send_1byte(*pTxData);
		pTxData++;
		Size--;
	}
}
void SPI_Sw_Receive(uint8_t* pRxData,uint16_t Size){
	while(Size > 0){
		*pRxData = SPI_Receive_1byte();
		pRxData++;
		Size--;
	}
}

