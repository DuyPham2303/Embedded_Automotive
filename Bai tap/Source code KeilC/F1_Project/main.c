#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "timer.h"
void GPIO_ToggleBit(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void RCC_Config(void);
void GPIO_Conig(void);
uint8_t cnt;
int main(){
	RCC_Config();
	GPIO_Conig();
	TIM_Config();
	while(1){
		GPIO_ToggleBit(GPIOB,GPIO_Pin_12);
		GPIO_ToggleBit(GPIOB,GPIO_Pin_13);
		GPIO_ToggleBit(GPIOB,GPIO_Pin_14);
		GPIO_ToggleBit(GPIOB,GPIO_Pin_15);
		delay_ms(500);
	}
}	

void RCC_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
}
void GPIO_ToggleBit(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  GPIOx->ODR ^= PortVal;
}

void GPIO_Conig(void){
	GPIO_InitTypeDef ledpin;
	ledpin.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	ledpin.GPIO_Mode = GPIO_Mode_Out_PP;
	ledpin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&ledpin);
}
