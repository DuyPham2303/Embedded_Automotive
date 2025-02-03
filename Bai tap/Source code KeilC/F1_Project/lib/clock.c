#include "stm32f10x.h" // Thu vi?n cho STM32F1 trong KeilC
#include "clock.h"
uint32_t Get_HCLK(void) {
    return SystemCoreClock;
}

uint32_t Get_PCLK1(void) {
    // �?c gi� tr? h? s? chia APB1 t? thanh ghi RCC_CFGR
    uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> 8;
    uint32_t divider;

    if (ppre1 == 0) {
        // Kh�ng c� h? s? chia
        divider = 1;
    } else {
        // C�c h? s? chia t? 0b100 tr? di tuong duong v?i chia 2, 4, 8, 16
        divider = 1 << ((ppre1 - 0x4) + 1);
    }

    return Get_HCLK() / divider;
}

uint32_t Get_PCLK2(void) {
    // �?c gi� tr? h? s? chia APB2 t? thanh ghi RCC_CFGR
    uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> 11;
    uint32_t divider;

    if (ppre2 == 0) {
        // Kh�ng c� h? s? chia
        divider = 1;
    } else {
        // C�c h? s? chia t? 0b100 tr? di tuong duong v?i chia 2, 4, 8, 16
        divider = 1 << ((ppre2 - 0x4) + 1);
    }

    return Get_HCLK() / divider;
}

uint32_t Get_TIMCLK(uint8_t timer_num) {
    // L?y clock cho c�c timer d?a tr�n bus m� ch�ng thu?c v? (APB1 ho?c APB2)
    uint32_t tim_clk;
    
    if (timer_num == 1 || timer_num == 8) {
        // TIM1 v� TIM8 n?m tr�n APB2
        tim_clk = Get_PCLK2();
        
        // N?u APB2 c� h? s? chia, TIMCLK s? g?p d�i PCLK2
        if ((RCC->CFGR & RCC_CFGR_PPRE2) >= 0x4) {
            tim_clk *= 2;
        }
    } else {
        // C�c timer c�n l?i n?m tr�n APB1
        tim_clk = Get_PCLK1();
        
        // N?u APB1 c� h? s? chia, TIMCLK s? g?p d�i PCLK1
        if ((RCC->CFGR & RCC_CFGR_PPRE1) >= 0x4) {
            tim_clk *= 2;
        }
    }
    
    return tim_clk;
}
