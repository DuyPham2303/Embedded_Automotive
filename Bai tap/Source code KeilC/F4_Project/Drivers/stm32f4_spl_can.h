#ifndef STM32F1_SPL_CAN_H
#define STM32F1_SPL_CAN_H
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f4xx_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f4xx_can.h"              // Keil::Device:StdPeriph Drivers:CAN

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
