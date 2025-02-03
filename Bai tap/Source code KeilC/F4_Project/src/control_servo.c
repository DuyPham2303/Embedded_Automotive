#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"
#include "stm32f4_spl_spi.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define SPI_CS_Pin GPIO_Pin_12
#define SPI_SCK_Pin GPIO_Pin_13
#define SPI_MISO_Pin GPIO_Pin_14
#define SPI_MOSI_Pin GPIO_Pin_15

#define SPI_PORT 			GPIOB
#define SPI_PORT_RCC	RCC_AHB1Periph_GPIOB
#define SPI_RCC 			RCC_APB1Periph_SPI2
#define SPI_SOURCE		SPI2

#define TIM_RCC 			RCC_APB1Periph_TIM2
#define TIM_SOURCE 		TIM2
#define PWM_PIN				GPIO_Pin_1
#define PWM_CHANNEL		TIM_OCMode_PWM1
#define PWM_PORT			GPIOA
	
#define DMA_RCC				RCC_AHB1Periph_DMA1

void RCC_Config(void);
void GPIO_Config(void);
void SPI_Config(void);
void TIM_Config(void);
void DMA_Config(void);
void delay_ms(uint32_t time);
long map(long x, long in_min, long in_max, long out_min, long out_max);
int convertPulseToAngle(int pulse, int pulse_min, int pulse_max);
#define ANGLE_NEG_90		50
#define ANGLE_POS_90		250
#define ANGLE_0					150	
#define SERVO_PERIOD		2000 // 20ms / 0.01 ms

uint16_t duty_cycle_servo = 0,adc_val = 0;
uint8_t servo_angle = 0;
SPI_HandleTypeDef hspi2;

int main(){
	RCC_Config();
	GPIO_Config();
	SPI_Config();
	DMA_Config();
	TIM_Config();
	while(1){
		//SPL_SPI_Receive(&hspi2,&adc_val,sizeof(adc_val));
		duty_cycle_servo = map(adc_val,0,4095,ANGLE_NEG_90,ANGLE_POS_90);
		servo_angle = convertPulseToAngle(duty_cycle_servo,ANGLE_NEG_90,ANGLE_POS_90);
		TIM_SetCompare2(TIM_SOURCE,duty_cycle_servo);
	}
}
void RCC_Config(void){
		RCC_AHB1PeriphClockCmd(SPI_PORT_RCC | RCC_AHB1Periph_GPIOA | DMA_RCC ,ENABLE);
	  RCC_APB1PeriphClockCmd(SPI_RCC | TIM_RCC,ENABLE);
}
void GPIO_Config(){
	GPIO_InitTypeDef slave_gpio_init,pwm_gpio_init;
	slave_gpio_init.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_MISO_Pin;
	slave_gpio_init.GPIO_Mode =  GPIO_Mode_AF;
	slave_gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	slave_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;

	pwm_gpio_init.GPIO_Pin = PWM_PIN;
	pwm_gpio_init.GPIO_Mode = GPIO_Mode_AF;
	pwm_gpio_init.GPIO_OType = GPIO_OType_PP;
	pwm_gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pwm_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWM_PORT,&pwm_gpio_init);
	
	GPIO_PinAFConfig(PWM_PORT,GPIO_PinSource1, GPIO_AF_TIM2);
	GPIO_PinAFConfig(SPI_PORT,GPIO_PinSource15, GPIO_AF_SPI2);
	GPIO_PinAFConfig(SPI_PORT,GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(SPI_PORT,GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_Init(SPI_PORT, &slave_gpio_init);

}

void SPI_Config(){
	hspi2.pSPIx = SPI_SOURCE;
	hspi2.Init.SPI_Mode =  SPI_Mode_Slave;
	hspi2.Init.SPI_Direction =  SPI_Direction_2Lines_FullDuplex;
	hspi2.Init.SPI_CPHA = SPI_CPHA_1Edge;
	hspi2.Init.SPI_CPOL = SPI_CPOL_Low;
	hspi2.Init.SPI_DataSize = SPI_DataSize_16b;
	hspi2.Init.SPI_FirstBit = SPI_FirstBit_MSB;
	hspi2.Init.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI_SOURCE,&hspi2.Init);
	SPI_Cmd(SPI_SOURCE,ENABLE);
}
void DMA_Config(){
		DMA_InitTypeDef DMA_spiInit;

		// Configure DMA for SPI2 RX
		DMA_DeInit(DMA1_Stream3);  // Deinitialize the DMA stream

		DMA_spiInit.DMA_Channel = DMA_Channel_0;  // SPI2 RX channel
		DMA_spiInit.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;  // SPI2 Data Register address
		DMA_spiInit.DMA_Memory0BaseAddr = (uint32_t)&adc_val;  // Memory address where data will be stored
		DMA_spiInit.DMA_DIR = DMA_DIR_PeripheralToMemory;  // Data transfer direction (Peripheral to Memory)
		DMA_spiInit.DMA_BufferSize = 1;  // Transfer 1 item (16 bits)
		DMA_spiInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // No increment for peripheral address
		DMA_spiInit.DMA_MemoryInc = DMA_MemoryInc_Disable;  // No memory address increment
		DMA_spiInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  // 16-bit data size
		DMA_spiInit.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  // 16-bit data size
		DMA_spiInit.DMA_Mode = DMA_Mode_Circular;  // Continuous transfer mode
		DMA_spiInit.DMA_Priority = DMA_Priority_High;  // Set priority
		DMA_spiInit.DMA_FIFOMode = DMA_FIFOMode_Disable;  // Disable FIFO
		DMA_spiInit.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;  // FIFO threshold
		DMA_spiInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;  // Single burst
		DMA_spiInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  // Single burst
		DMA_Init(DMA1_Stream3, &DMA_spiInit);
		
		DMA_Init(DMA1_Stream3,&DMA_spiInit);
		DMA_Cmd(DMA1_Stream3,ENABLE);
		SPI_I2S_DMACmd(SPI_SOURCE,SPI_DMAReq_Rx,ENABLE);
}
void TIM_Config(){
		TIM_TimeBaseInitTypeDef TIM2Init;
		TIM2Init.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM2Init.TIM_Prescaler = 276 - 1; // 0.01ms per count
		TIM2Init.TIM_Period = SERVO_PERIOD - 1;
		TIM2Init.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM_SOURCE,&TIM2Init);
		
		TIM_OCInitTypeDef pwm_ServoInit;
		pwm_ServoInit.TIM_OCMode = PWM_CHANNEL;
		pwm_ServoInit.TIM_OutputState = TIM_OutputState_Enable;
		pwm_ServoInit.TIM_Pulse = duty_cycle_servo;
		pwm_ServoInit.TIM_OCPolarity = TIM_OCPolarity_High;
	
		TIM_OC2Init(TIM_SOURCE,&pwm_ServoInit);
	
		TIM_OC2PreloadConfig(TIM_SOURCE,TIM_OCPreload_Enable);
	
		TIM_Cmd(TIM_SOURCE,ENABLE);
}
void delay_ms(uint32_t time){
	TIM_SetCounter(TIM_SOURCE,0);
	while(TIM_GetCounter(TIM_SOURCE) <  time * 10);
}
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
int convertPulseToAngle(int pulse, int pulse_min, int pulse_max) {
    return (pulse - pulse_min) * 180 / (pulse_max - pulse_min);
}