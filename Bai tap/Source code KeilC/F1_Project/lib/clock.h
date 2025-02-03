#ifndef CLOCK_H_
#define	CLOCK_H_
#include<stdint.h>
uint32_t Get_TIMCLK(uint8_t timer_num);
uint32_t Get_PCLK2(void);
uint32_t Get_PCLK1(void);
uint32_t Get_HCLK(void);
#endif /*__CLOCK_H*/	
