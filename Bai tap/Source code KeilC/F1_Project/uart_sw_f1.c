#include "stm32f1_sw_uart.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>
void GPIO_Config(void);
char str[] = "hello duysolo";
uint8_t cnt = 0 ;
uint8_t buff[15];
int main(){
	GPIO_Config();
	USART_Sw_Init();
	while(1){
		sprintf((char*)buff,"%s %d\n",str,cnt++);
		USART_Sw_Send(buff,sizeof(buff));
		cnt++;
		for(uint32_t i = 0 ; i < 100000;i++){
			delay_10us(1);
		}
	}
}
void GPIO_Config(){
		RCC_APB2PeriphClockCmd(USART_RCC,ENABLE);
	
		GPIO_InitTypeDef uart_gpio_init;
		//TX config
		uart_gpio_init.GPIO_Pin = TX_PIN;
		uart_gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
		uart_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART_PORT,&uart_gpio_init);
	
		//Rx config
		uart_gpio_init.GPIO_Pin = RX_PIN;
		uart_gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		uart_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART_PORT,&uart_gpio_init);
}