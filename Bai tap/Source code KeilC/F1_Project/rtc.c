#include "ds3231.h"
#include "timer.h"
DS3231_Handle rtc;
uint8_t hour = 23 ,min = 59,sec = 40;
uint8_t cnt = 0;
int main(){
	DS3231_GPIO_Init();
	//DS3231_I2C_Init();
	TIM_Config();
	DS3231_SetTime(&rtc,hour,min,sec);
	while(1){
		DS3231_GetTime(&rtc);
		delay_ms(1000);
	}
}

