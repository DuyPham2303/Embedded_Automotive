#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "Flash.h"
#define ADDR_STR_BLINK 0x08008000
void Boot() {
    //STEP 1
    RCC_DeInit();
    //STEP 2
    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | 	SCB_SHCSR_MEMFAULTENA_Msk);
   //STEP 3
    __set_MSP(*(__IO uint32_t*)(ADDR_STR_BLINK));
   //STEP 4
    SCB->VTOR = ADDR_STR_BLINK;
   //STEP 5
    uint32_t JumpAddress = *(__IO uint32_t*)(ADDR_STR_BLINK + 4);
   //STEP 6
    void (*reset_handler)(void) = (void (*)(void)) JumpAddress;
   //STEP 7
    reset_handler();
 }

void Flash_Erase(uint32_t addresspage){
	 FLASH_Unlock();
	 while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	 FLASH_ErasePage(addresspage);
	 while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	 FLASH_Lock();
}
void Flash_WriteInt(uint32_t address,uint16_t value){
	  FLASH_Unlock();
	  while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	  FLASH_ProgramHalfWord(address,value);
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
		FLASH_Lock();
}
void Flash_WriteFloat(uint32_t address,float value){
		FLASH_Unlock();
	  while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
		uint8_t data[4];
		*(float*)data = value;
	  FLASH_ProgramWord(address,*(uint32_t*)data);
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
		FLASH_Lock();
}
void Flash_WriteArr(uint32_t address,uint8_t *data, int Size){
 FLASH_Unlock();
 while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
 uint16_t *ptr = (uint16_t*)data;
 for(int i=0; i< (Size+1)/2 ; i++){
	 FLASH_ProgramHalfWord(address+2*i,*ptr);
	 while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	 ptr++;
 }
 FLASH_Lock();
}
int Flash_ReadInt(uint32_t address){
	return *(volatile uint16_t*)address;
}
void Flash_ReadArr(uint32_t address,uint8_t* Arr,uint16_t Size){
	uint16_t* pArr = (uint16_t*)Arr;
	for(uint16_t i = 0 ; i < (Size + 1)/2 ; i++){
		*pArr = *(volatile uint16_t*)(address + i*2);
		pArr++;
	}
}
float Flash_ReadFloat(uint32_t address){
	uint32_t data = *(volatile uint32_t*)address;
	return *(float*)&data;
}