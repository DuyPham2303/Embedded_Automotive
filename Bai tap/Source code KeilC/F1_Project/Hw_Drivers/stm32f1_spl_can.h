#ifndef STM32F1_SPL_CAN_H
#define STM32F1_SPL_CAN_H
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_can.h"              // Keil::Device:StdPeriph Drivers:CAN
#define CAN_RCC				RCC_APB1Periph_CAN1
#define CAN_PORT_RCC	RCC_APB2Periph_GPIOA
#define CAN_TX				GPIO_Pin_12		
#define CAN_RX				GPIO_Pin_11
#define CAN_PORT			GPIOA
#define CAN_SOURCE		CAN1
typedef struct{
 CAN_TypeDef* CANx;
 CAN_InitTypeDef Init;
 union{
	 CanRxMsg Rx_msg;
	 CanTxMsg Tx_msg;
 }CAN_MsgType;
}CAN_Handle;
void CAN_TransmitData(CAN_Handle* pCAN_handle,uint32_t ID,uint8_t* data, uint8_t length);
void CAN_ReceiveData(CAN_Handle* pCAN_handle,uint32_t ID,uint8_t*data);
#endif
