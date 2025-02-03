#ifndef DS3231_H_
#define DS3231_H_
void GPIO_Config(void);
void I2C_Config(void);
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f1_spl_i2c.h"
#include "stm32f1_sw_i2c.h"
#define I2C_SOURCE			I2C1
#define I2C_RCC					RCC_APB1Periph_I2C1
#define I2C_SCL_Pin			GPIO_Pin_6
#define I2C_SDA_Pin			GPIO_Pin_7
#define RCC_GPIO_I2C  	RCC_APB2Periph_GPIOB
#define I2C_PORT				GPIOB
#define DS3231_ADD			0x68
typedef struct{
	uint8_t TxbuffTime[3]; 
	uint8_t RxbuffTime[3];
	uint8_t second;
	uint8_t min;
	uint8_t hour;
}DS3231_Handle;
void DS3231_GPIO_Init();
void DS3231_I2C_Init();
void DS3231_SetTime(DS3231_Handle* DS3231,uint8_t hour,uint8_t min,uint8_t sec);
void DS3231_GetTime(DS3231_Handle*);
#endif


