#include "stm32f1_sw_uart.h"
#include "timer.h"
static void clock(uint8_t duration){
	for(uint8_t i = 0 ; i < duration ; i++){
		delay_10us(1);
	}
}
/*
uint16_t Parity_Generate(uint8_t data, Parity_Mode Mode){
	uint8_t count = 0;
	uint8_t data1 = data;
	for(int i = 0; i < 8; i++){
		if(data1 & 0x01){
			count++;
		}
		data1 >>= 1;
	}
	switch(Mode){
		case Parity_Mode_NONE:
			return data; 
			break;
		case Parity_Mode_ODD:
			if(count % 2){
				return ((data << 1) | 1);
			} else {
				return (data<<1);
			}
			break;
		case Parity_Mode_EVEN:
			if(!(count % 2)){
				return ((data << 1) | 1);
			} else {
				return (data << 1);
			}
			break;
		default:
			return data;
			break;
	}
}
uint8_t Parity_Check(uint8_t data, Parity_Mode Mode){
	uint8_t count = 0;
	for(int i = 0; i < 8; i++){
		if(data & 0x01){
			count++;
		}
		data >>= 1;
	}
	switch(Mode){
		case Parity_Mode_NONE:{
			return 1; 
			break;
		}
		case Parity_Mode_ODD:{
			return (count % 2);
			break;
		}
		case Parity_Mode_EVEN:{
			return (!(count % 2));
			break;
		}
		default:{
			return 0;
			break;
		}
	}
}
*/
static void USART_Send_1byte(char data){
	GPIO_ResetBits(GPIOA,TX_PIN);
	clock(CLK_1bit);
	for(uint8_t i = 0 ; i < 8 ; i++){
			if(data & (1 << i)){ // send LSB first
				GPIO_SetBits(GPIOA,TX_PIN); 
			}
			else GPIO_ResetBits(GPIOA,TX_PIN);
			clock(CLK_1bit);
	}
	GPIO_SetBits(GPIOA,RX_PIN);
	clock(CLK_1bit);
}
static uint8_t USART_Receive_1byte(){
		uint8_t rx_data;
		//Wait for start bit from Tx
		while(GPIO_ReadInputDataBit(GPIOA,RX_PIN));
		clock(CLK_1_half_bit);
		for(uint8_t i = 0 ; i < 8 ;i++){
			if(GPIO_ReadInputDataBit(GPIOA,RX_PIN)){
				rx_data |= 1;
			}
			rx_data <<=1;
			clock(CLK_1bit);
		}
		while(!GPIO_ReadInputDataBit(GPIOA,RX_PIN));
		clock(CLK_half_bit);
		return rx_data;
}	
void USART_Sw_Init(){
	GPIO_SetBits(USART_PORT,TX_PIN);
	delay_10us(1);
}
void USART_Sw_Send(uint8_t* pTxData,uint16_t Size){
	while(Size > 0){
		USART_Send_1byte(*pTxData);
		pTxData++;
		Size--;
	}
}
void USART_Sw_Receive(uint8_t* pRxData,uint16_t Size){
	while(Size > 0){
		*pRxData = USART_Receive_1byte();
		pRxData++;
		Size--;
	}
}