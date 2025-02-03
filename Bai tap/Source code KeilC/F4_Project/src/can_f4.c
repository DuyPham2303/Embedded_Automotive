#include "stm32f4_spl_can.h"
#include <stdint.h>

#define CAN_RCC				RCC_APB1Periph_CAN1
#define CAN_PORT_RCC	RCC_AHB1Periph_GPIOB
#define CAN_TX				GPIO_Pin_9		
#define CAN_RX				GPIO_Pin_8
#define CAN_PORT			GPIOB
#define CAN_SOURCE		CAN1

void CAN_GPIO_Config(void);
void CAN_ParamConfig(void);
void CAN_FilterConfig(uint32_t ID);
uint32_t ID = 0x125; //11-bit ID
uint32_t Get_PCLK1_Freq(void);
CAN_Handle can1;
uint8_t flag = 0;
uint8_t buff_tx[] = {12,32,16,61,66,37};
uint8_t buff_rx[7];
uint8_t len = sizeof(buff_tx) / sizeof(buff_tx[0]);
int main(){
	CAN_GPIO_Config();
	CAN_ParamConfig();
	CAN_FilterConfig(ID);
	CAN_TransmitData(&can1,ID,buff_tx,len);
	flag++;
	return 0;
}
void CAN_GPIO_Config(void){
 RCC_APB1PeriphClockCmd(CAN_RCC, ENABLE);
 RCC_AHB1PeriphClockCmd(CAN_PORT_RCC, ENABLE);
	
 GPIO_InitTypeDef can_gpioInit;
 //config CAN_RX
 can_gpioInit.GPIO_Pin = CAN_RX;
 can_gpioInit.GPIO_Mode = GPIO_Mode_AF;
 can_gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_Init(CAN_PORT, &can_gpioInit);
 //config CAN_TX
 can_gpioInit.GPIO_Pin = CAN_TX;
 can_gpioInit.GPIO_Mode = GPIO_Mode_AF;
 can_gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
 can_gpioInit.GPIO_OType = GPIO_OType_PP;
 can_gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(CAN_PORT, &can_gpioInit);
	
 GPIO_PinAFConfig(CAN_PORT,GPIO_PinSource9,GPIO_AF_CAN1);
}
void CAN_ParamConfig(void) {
 can1.CANx = CAN_SOURCE;
 //config general parameter 
 can1.Init.CAN_Mode = CAN_Mode_Normal; 
 can1.Init.CAN_TTCM = DISABLE; // Time Triggered Communication Mode
 can1.Init.CAN_ABOM = ENABLE; // Automatic Bus-Off Management
 can1.Init.CAN_AWUM = ENABLE; // Automatic Wake-Up Mode
 can1.Init.CAN_NART = DISABLE; // NoAutomatic Retransmission
 can1.Init.CAN_RFLM = DISABLE; // Receive FIFO Locked Mode
 can1.Init.CAN_TXFP = ENABLE; // Transmit FIFO Priority

 // Config synchronization parameter - (bit timing)
 can1.Init.CAN_SJW = CAN_SJW_1tq; // Synchronization Jump Width = 1 time quantum
 can1.Init.CAN_BS1 = CAN_BS2_6tq; // Bit Segment 1 = 6 time quanta
 can1.Init.CAN_BS2 = CAN_BS2_7tq; // Bit Segment 2 = 8 time quanta
 can1.Init.CAN_Prescaler = 6; 
 CAN_Init(CAN_SOURCE,&can1.Init);
	//Calculation of bit-timing
  /*
	1 bit = SJW_1tq + BS1_6tq + BS2_8tq = 14 tq
	APBCLK = 42MHZ , prescaler  = 6 -> baudrate = 7 Mhz
	=> baudrate = clock can / bit-timing = 7 / 15 = 0.5 mbps = 500 kbps 
	*/
}
void CAN_FilterConfig(uint32_t ID) {
 CAN_FilterInitTypeDef can_filterInit;
 can_filterInit.CAN_FilterNumber = 0; //use filter 0
 can_filterInit.CAN_FilterMode = CAN_FilterMode_IdMask; 
 can_filterInit.CAN_FilterScale = CAN_FilterScale_32bit; 
 can_filterInit.CAN_FilterIdHigh = ID << 5;// Standard ID 0x321
 can_filterInit.CAN_FilterIdLow = 0x0000;
 can_filterInit.CAN_FilterMaskIdHigh = 0xFFE0; //11 bit-mask out of 16 bit high
 can_filterInit.CAN_FilterMaskIdLow = 0x0000; // no bit-mask out of 16 bit low
 can_filterInit.CAN_FilterFIFOAssignment = CAN_FIFO0; //Ðua khung vào FIFO0
 can_filterInit.CAN_FilterActivation = ENABLE;
 CAN_FilterInit(&can_filterInit);
}