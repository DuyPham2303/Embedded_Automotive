#include "stm32f1_sw_i2c.h"
#include <timer.h>
static void I2C_Start(){
	delay_ms(1);
	WRITE_SDA_0;	//SDA reset to 0 before SCL.
	delay_ms(1);
	WRITE_SCL_0;
	delay_ms(1);
}
static void I2C_Stop(){
	WRITE_SDA_0;
	delay_ms(1);
	WRITE_SCL_1; 	//SCL set to 1 before SDA.
	delay_ms(1);
	WRITE_SDA_1;
	delay_ms(1);
}
static void I2C_Clock(){
	WRITE_SCL_1;
	delay_ms(2);
	WRITE_SCL_0;
	delay_ms(2);
}
status I2C_Write_1byte(uint8_t TxData){	
	status st_ret;
	for(int i=0; i< 8; i++){		//Write byte data.
		if (TxData & 0x80) {
			WRITE_SDA_1;
		} else {
			WRITE_SDA_0;
		}
		I2C_Clock();
		TxData <<= 1;
	}
	//master release bus to wait for slave's response
	WRITE_SDA_1;
	delay_ms(1);
	
	//generate high clock to receive ACK/NACK from slave
	WRITE_SCL_1;
	delay_ms(1); 
	
	//read SDA at the middle of clock  
	if(READ_SDA_VAL) {	
		st_ret = NOT_OK; //NACK bit		
	} else {
		st_ret = OK;		//ACK bit
	}
	delay_ms(1);
	WRITE_SCL_0;
	delay_ms(2);
	
	return st_ret;
}

static uint8_t I2C_Read_1byte(ACK_Bit _ACK){	
	uint8_t i;						
	uint8_t pRxData = 0x00;
	//master release SDA line to let slave take control the bus 
	WRITE_SDA_1;
	delay_ms(2);	//make sure sda line is pulled to high successfully 
	
	for (i = 0; i < 8; ++i) {
		pRxData <<= 1;
		
		//read data at the middle of CLOCK HIGH
		WRITE_SCL_1;
		delay_ms(1);
		if (READ_SDA_VAL) {
			pRxData |= 0x01;
		}
		delay_ms(1);
		WRITE_SCL_0; //send CLOCH LOW
		delay_ms(2);
	}
	//master send ACK/NACK to slave
		if(_ACK == ACK) {	
			WRITE_SDA_0; 
		}else{
			WRITE_SDA_1;
		}		
		I2C_Clock();//generate clock for 1 bit ACK/NACK 
		return pRxData;
}

static status I2C_SendPhaseAddress_bitRW(uint8_t SlaveAdd,bit_RW rw){
	SlaveAdd <<= 1;
	if(rw == READ_BIT) SlaveAdd |= 1;
	while(I2C_Write_1byte(SlaveAdd) == NOT_OK);
	return OK;
}


status I2C_Sw_Send(uint8_t* pTxData,uint16_t Size,uint8_t SlaveAdd){
	I2C_Start();
	while(I2C_SendPhaseAddress_bitRW(SlaveAdd,WRITE_BIT) == NOT_OK);
	while(Size > 0){
		while(I2C_Write_1byte(*pTxData) == NOT_OK);
		pTxData++;
		Size--;
	}
	I2C_Stop();
	return OK;
}
status I2C_Sw_Receive(uint8_t* pRxData,uint16_t Size,uint8_t SlaveAdd){
	I2C_Start();
	while(I2C_SendPhaseAddress_bitRW(SlaveAdd,READ_BIT) == NOT_OK);
	while(Size > 0){
		if(Size == 1){
				*pRxData = I2C_Read_1byte(NACK);
		}
		else{
				*pRxData = I2C_Read_1byte(ACK);
		}
		pRxData++;
		Size--;
	}
	I2C_Stop();
	return OK;
}

