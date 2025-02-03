#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include <stdint.h>
#include <stdbool.h>
#define  APB1_CLK	0
#define  APB2_CLK	1
uint32_t Get_PCLK1_Freq(void) {
    SystemCoreClockUpdate(); // C?p nh?t SystemCoreClock
   int32_t pclk1, hclk, prescaler;

    hclk = SystemCoreClock; // L?y t?n s? HCLK
    prescaler = (RCC->CFGR >> 10) & 0x07; // L?y APB1 prescaler

    if (prescaler < 4) {
        pclk1 = hclk; // N?u không chia
    } else {
        pclk1 = hclk >> (prescaler - 3); // N?u có chia (t? 2 tr? lên)
    }

    return pclk1;
}
uint32_t Get_PCLK2_Freq(void) {
    SystemCoreClockUpdate(); // C?p nh?t SystemCoreClock
   int32_t pclk2, hclk, prescaler;

    hclk = SystemCoreClock; // L?y t?n s? HCLK
    prescaler = (RCC->CFGR >> 13) & 0x07; // L?y APB1 prescaler

    if (prescaler < 4) {
        pclk2 = hclk; // N?u không chia
    } else {
        pclk2 = hclk >> (prescaler - 3); // N?u có chia (t? 2 tr? lên)
    }

    return pclk2;
}
uint32_t Get_Timer_Clock_Freq(TIM_TypeDef *TIMx,bool PCLK_SOURCE) {
    uint32_t timer_clock,pclk,prescaler;
		if(PCLK_SOURCE == APB1_CLK) {
			pclk = Get_PCLK1_Freq();
			prescaler = (RCC->CFGR >> 10) & 0x07;
		}
		else{
				pclk = Get_PCLK2_Freq();
				prescaler = (RCC->CFGR >> 13) & 0x07;
		}

		if (prescaler < 4) {
				timer_clock = pclk;
		} else {
				timer_clock = pclk * 2;
		}
    return timer_clock;
}
