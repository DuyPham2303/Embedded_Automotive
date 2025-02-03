#ifndef STM32F1_SW_I2C_H_
#define STM32F1_SW_I2C_H_
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
typedef enum{
	OK,NOT_OK
}status;	
typedef enum{
	ACK,NACK
}ACK_Bit;
typedef enum{
	WRITE_BIT,
	READ_BIT
}bit_RW;
#define I2C_GPIO	GPIOB
#define I2C_SCL		GPIO_Pin_6
#define I2C_SDA		GPIO_Pin_7

#define WRITE_SDA_0 	GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_1 	GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SCL_0 	GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_1 	GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define READ_SDA_VAL 	GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)

status I2C_Sw_Send(uint8_t* pTxData,uint16_t Size,uint8_t SlaveAdd);
status I2C_Sw_Receive(uint8_t* pRxData,uint16_t Size,uint8_t SlaveAdd);
#endif

