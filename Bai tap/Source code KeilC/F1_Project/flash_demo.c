#include "Flash.h"

uint8_t writeInt = 234;
uint8_t ArrWrite[] = {90,221,141,11,111,92,12};
float writeFloat = 652.221;
uint8_t ArrRead[7];

#define FLASH_SIZE_BASE 0x1FFFF7E0 //address of flash register
void demo_flash(){
	Flash_Erase(PAGE_ADDRESS_61);
	Flash_WriteInt(PAGE_ADDRESS_61,writeInt);
	uint16_t readInt = Flash_ReadInt(PAGE_ADDRESS_62);
	
	Flash_Erase(PAGE_ADDRESS_62);
	Flash_WriteFloat(PAGE_ADDRESS_62,writeFloat);
	float readFloat = Flash_ReadFloat(PAGE_ADDRESS_63);
	
	Flash_Erase(PAGE_ADDRESS_63);
	Flash_WriteArr(PAGE_ADDRESS_63,ArrWrite,sizeof(ArrWrite));
	Flash_ReadArr(PAGE_ADDRESS_63,ArrRead,sizeof(ArrRead));
}
int main(){

}
