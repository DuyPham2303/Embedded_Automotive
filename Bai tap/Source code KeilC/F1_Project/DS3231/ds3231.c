#include "ds3231.h" 
I2C_HandleTypeDef ds3231_i2cInit;
void DS3231_GPIO_Init(){
	RCC_APB1PeriphClockCmd(I2C_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_GPIO_I2C,ENABLE);

	GPIO_InitTypeDef ds3231_gpio_init;
	ds3231_gpio_init.GPIO_Pin = I2C_SDA_Pin | I2C_SCL_Pin;
	ds3231_gpio_init.GPIO_Mode = GPIO_Mode_Out_OD;
	ds3231_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_PORT,&ds3231_gpio_init);
	
	//Softwate API only 

	WRITE_SDA_1;
	WRITE_SCL_1;
	
}
void DS3231_I2C_Init(){
		ds3231_i2cInit.pI2Cx = I2C_SOURCE;
		ds3231_i2cInit.I2C_Config.I2C_Mode = I2C_Mode_I2C; //fast mode
		ds3231_i2cInit.I2C_Config.I2C_OwnAddress1 = 0x61;
		ds3231_i2cInit.I2C_Config.I2C_Ack = I2C_Ack_Enable;
		ds3231_i2cInit.I2C_Config.I2C_ClockSpeed = 100000;
		ds3231_i2cInit.I2C_Config.I2C_DutyCycle = I2C_DutyCycle_2;
		ds3231_i2cInit.I2C_Config.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_Init(I2C_SOURCE,&ds3231_i2cInit.I2C_Config);
		I2C_Cmd(I2C_SOURCE,ENABLE);
		I2C_AcknowledgeConfig(ds3231_i2cInit.pI2Cx,ENABLE);
}
static void I2C_WriteTime(DS3231_Handle* DS3231){
	uint8_t len = sizeof(DS3231->TxbuffTime) / sizeof(DS3231->TxbuffTime[0]);
	for(int i = 0 ; i < len ; i++){
		uint8_t data[2];
		data[0] = i; // word address
		data[1] = DS3231->TxbuffTime[i]; //data written 
		//send word address followed by written data
		//SPL_I2C_MasterSend(&ds3231_i2cInit,data,2,DS3231_ADD);
		
		/*Software API*/
		while(I2C_Sw_Send(data,2,DS3231_ADD) == NOT_OK);
	}
}
static void I2C_ReadTime(DS3231_Handle* DS3231){
	uint8_t len = sizeof(DS3231->RxbuffTime) / sizeof(DS3231->RxbuffTime[0]);
	for(int i = 0 ; i < len ; i++){
		uint8_t read_reg = i;
		//SPL_I2C_MasterSend(&ds3231_i2cInit,&read_reg,1,DS3231_ADD);
		//SPL_I2C_MasterReceive(&ds3231_i2cInit,&DS3231->RxbuffTime[i],1,DS3231_ADD);
		
		/*Software API*/
		while(I2C_Sw_Send(&read_reg,1,DS3231_ADD) == NOT_OK); //write reg pointer at the word address we want to read from
		while(I2C_Sw_Receive(&DS3231->RxbuffTime[i],1,DS3231_ADD) == NOT_OK); //read data from word address
	}
}

static uint8_t BinaryToBCD(uint8_t value){
	uint8_t BCD = 0;
	if(value >= 10){
		uint8_t high_digit = value / 10;
		uint8_t low_digit = value % 10;
		BCD = (high_digit << 4 )| low_digit;
	}
	else if(BCD < 10){
		BCD = value;
	}
	return BCD;
}
static uint8_t BCDToBinary(uint8_t BCD_value){
	uint8_t binary;
	uint8_t high_digit = (uint8_t)(BCD_value >> 4) * 10;
	uint8_t low_digit = (BCD_value & (uint8_t)0xf);
	binary = high_digit + low_digit;
	return binary;
}
void DS3231_SetTime(DS3231_Handle* DS3231,uint8_t hour,uint8_t min,uint8_t sec){
	DS3231->TxbuffTime[0] = BinaryToBCD(sec);
	DS3231->TxbuffTime[1] = BinaryToBCD(min);
	DS3231->TxbuffTime[2] = BinaryToBCD(hour);
	I2C_WriteTime(DS3231);
}
void DS3231_GetTime(DS3231_Handle* DS3231){
	I2C_ReadTime(DS3231);
	DS3231->second = BCDToBinary(DS3231->RxbuffTime[0]);
	DS3231->min = BCDToBinary(DS3231->RxbuffTime[1]);
	DS3231->hour = BCDToBinary(DS3231->RxbuffTime[2]);
}

