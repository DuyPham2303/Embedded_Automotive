#include "Flash.h"
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