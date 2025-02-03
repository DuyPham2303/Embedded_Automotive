#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA

#include "misc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
	char* name;
	uint8_t age;
}Info;
void delay_ms(uint32_t ms);
void DMA_Config(void);
void RCC_Config(void);
void GPIO_Config(void);
void TIM_Config(void);
void UART_HW_Init(void);
void USART_TransmitByte(uint8_t byte);
uint8_t USART_ReceiveByte(void);
void USART_Send(uint8_t* pdata , uint8_t len);
void USART_Receive(void);
void NVIC_Config(void);
Info* parse_info(void);
void free_info(Info* info);
#define TX_PIN 						GPIO_Pin_9
#define RX_PIN 						GPIO_Pin_10
#define RCC_UART 					RCC_APB2Periph_USART1
#define RCC_TIM						RCC_APB1Periph_TIM2
#define TIM_SOURCE				TIM2
#define USART_SOURCE			USART1
#define UART_PORT					GPIOA
#define BUFF_SIZE					30

char buff_rx[BUFF_SIZE];


char buff_tx[30];
char* control_cmd[] = {"set PA0","reset PA0","set PA1","reset PA1"};
char* status_cmd[] = {"PA0 is set\n","PA0 is reset\n","PA1 is set\n","PA1 is reset\n","wrong cmd\n"};
char* status = "STM32 received:";
uint8_t cnt = 0;
uint8_t rx_index = 0;
uint16_t tim_cnt_10ms = 0,tim_cnt_1s = 0,len = 0;

void TIM2_IRQHandler(){
		if(TIM_GetITStatus(TIM_SOURCE,TIM_IT_Update) != RESET){
			tim_cnt_1s++;
			TIM_ClearITPendingBit(TIM_SOURCE,TIM_IT_Update);
		}
}
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	while(1){
		if(tim_cnt_1s % 2) GPIO_SetBits(GPIOC,GPIO_Pin_13);
		else 							 GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
}
void free_info(Info* info){
	free(info->name);
}
void RCC_Config(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_TIM ,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_UART,ENABLE);
}

void DMA_Config(){
	DMA_InitTypeDef DMA_usartInit;
	DMA_usartInit.DMA_Mode = DMA_Mode_Normal;
	DMA_usartInit.DMA_M2M = DMA_M2M_Disable;
	DMA_usartInit.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_usartInit.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_usartInit.DMA_MemoryBaseAddr = (uint32_t)buff_rx;
	DMA_usartInit.DMA_BufferSize = sizeof(buff_rx);
	DMA_usartInit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_usartInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_usartInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_usartInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_usartInit.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(DMA1_Channel5,&DMA_usartInit);
	DMA_Cmd(DMA1_Channel5,ENABLE);
	USART_DMACmd(USART_SOURCE,USART_DMAReq_Rx,ENABLE);
	
}
void NVIC_Config(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitTypeDef NVIC_usartInit;
	NVIC_usartInit.NVIC_IRQChannel = USART1_IRQn;
	NVIC_usartInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_usartInit.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_usartInit.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_usartInit);
	NVIC_InitTypeDef NVIC_timerInit;
	NVIC_timerInit.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_timerInit.NVIC_IRQChannelSubPriority = 0;
	NVIC_timerInit.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_timerInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_timerInit);
}
void UART_HW_Init(){
	USART_InitTypeDef uart_hw_init;
	uart_hw_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart_hw_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_hw_init.USART_Parity = USART_Parity_No;   
	uart_hw_init.USART_StopBits =  USART_StopBits_1;
	uart_hw_init.USART_WordLength = USART_WordLength_8b;
	uart_hw_init.USART_BaudRate = 9600;
	
	USART_Init(USART_SOURCE,&uart_hw_init);
	USART_Cmd(USART_SOURCE,ENABLE);
	USART_ITConfig(USART_SOURCE,USART_IT_RXNE,ENABLE);
}
void GPIO_Config(){
		GPIO_InitTypeDef uart_gpio_init,led_init;
		//TX config
		uart_gpio_init.GPIO_Pin = TX_PIN;
		uart_gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
		uart_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART_PORT,&uart_gpio_init);
		//Rx config
		uart_gpio_init.GPIO_Pin = RX_PIN;
		uart_gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		uart_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART_PORT,&uart_gpio_init);
	
		led_init.GPIO_Pin = GPIO_Pin_13;
		led_init.GPIO_Mode = GPIO_Mode_Out_PP;
		led_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC,&led_init);
}
uint8_t USART_ReceiveByte(void){
	uint8_t temp = 0x00;
    // Wait until data is received (RXNE flag is set)
    while (USART_GetFlagStatus(USART_SOURCE,USART_FLAG_RXNE) == RESET);

    // Read the received data
	temp = USART_ReceiveData(USART_SOURCE);
	return temp;
}
void USART_Receive(){
		uint8_t byte = USART_ReceiveByte();
		static uint8_t index = 0;
		if(byte != '\n'){
			buff_rx[index++] = byte;
		}
		else{
			buff_rx[index] = '\0';
			if(strcmp(buff_rx,control_cmd[0]) == 0){
				GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
				USART_Send((uint8_t*)status_cmd[0],strlen(status_cmd[0]));
			}
			else if(strcmp(buff_rx,control_cmd[1]) == 0){
				GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
				USART_Send((uint8_t*)status_cmd[1],strlen(status_cmd[1]));
			}
			else if(strcmp(buff_rx,control_cmd[2]) == 0){
				GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
				USART_Send((uint8_t*)status_cmd[2],strlen(status_cmd[2]));
			}
			else if(strcmp(buff_rx,control_cmd[3]) == 0){
				GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
				USART_Send((uint8_t*)status_cmd[3],strlen(status_cmd[3]));
			}
			else{
				USART_Send((uint8_t*)status_cmd[4],strlen(status_cmd[4]));
			}
			index = 0;
		}
}
void USART_TransmitByte(uint8_t byte) {
    // Wait until the transmit data register is empty (TXE flag is set)
    while (USART_GetFlagStatus(USART_SOURCE, USART_FLAG_TXE) == RESET);

    // Transmit the byte
    USART_SendData(USART_SOURCE, byte);

    // Wait until transmission is complete (TC flag is set)
    while (USART_GetFlagStatus(USART_SOURCE, USART_FLAG_TC) == RESET);
}
void USART_Send(uint8_t* pdata , uint8_t len){
		for(uint8_t i = 0 ; i < len ;i++){
				USART_TransmitByte(pdata[i]);
		}
}
void delay_ms(uint32_t ms){
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < ms * 10);
}
void TIM_Config(){
	TIM_TimeBaseInitTypeDef tim_Init;
	tim_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_Init.TIM_Prescaler = 7200 - 1; //0.1 ms per count 
	tim_Init.TIM_Period = 10000 - 1; 
	tim_Init.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM_SOURCE,&tim_Init);
	TIM_Cmd(TIM_SOURCE,ENABLE);
	TIM_ITConfig(TIM_SOURCE,TIM_IT_Update,ENABLE);
}
void USART1_IRQHandler(){
		//wait until data available in Rx buffer -> IT flag is enabled
		if(USART_GetITStatus(USART_SOURCE,USART_IT_RXNE) != RESET){
			uint8_t byte = USART_ReceiveData(USART_SOURCE);
			if(byte != '\n'){
				buff_rx[rx_index++] = byte;
			}
			else{
				buff_rx[rx_index] = '\0';
				sprintf(buff_tx,"%s %s\n",status,buff_rx);
				USART_Send((uint8_t*)buff_tx,strlen(buff_tx));
				rx_index = 0;
			}
		}
		USART_ClearITPendingBit(USART_SOURCE,USART_IT_RXNE);
}