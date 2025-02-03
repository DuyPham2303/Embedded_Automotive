#ifndef FLASH_H_
#define FLASH_H_
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_flash.h"            // Keil::Device:StdPeriph Drivers:Flash

#define PAGE_SIZE 			    1024
#define FIRST_PAGE			 		((uint32_t)0x08000000)
#define PAGE_ADDRESS(page)  (FIRST_PAGE + (PAGE_SIZE * page))
#define PAGE_ADDRESS_62 PAGE_ADDRESS(61)
#define PAGE_ADDRESS_63 PAGE_ADDRESS(62)
#define PAGE_ADDRESS_64 PAGE_ADDRESS(63)

void Flash_Erase(uint32_t addresspage);
void Flash_WriteInt(uint32_t address,uint16_t value);
void Flash_WriteFloat(uint32_t address,float value);
void Flash_WriteArr(uint32_t address,uint8_t *data, int Size);

int Flash_ReadInt(uint32_t address);
void Flash_ReadArr(uint32_t address,uint8_t* Arr,uint16_t Size);
float Flash_ReadFloat(uint32_t address);
#endif




