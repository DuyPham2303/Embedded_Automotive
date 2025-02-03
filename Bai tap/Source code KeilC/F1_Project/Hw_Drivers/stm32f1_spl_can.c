#include "stm32f1_spl_can.h"
void CAN_TransmitData(CAN_Handle* pCAN_handle,uint32_t ID,uint8_t* data, uint8_t length) {
 pCAN_handle->CAN_MsgType.Tx_msg.StdId = ID; //ID 11-bit
 pCAN_handle->CAN_MsgType.Tx_msg.RTR = CAN_RTR_DATA; // Data Frame
 pCAN_handle->CAN_MsgType.Tx_msg.IDE = CAN_ID_STD; // Standard ID
 pCAN_handle->CAN_MsgType.Tx_msg.DLC = length; // data length
 // copy data to data_frame
 for (int i = 0; i < length; i++) {
		pCAN_handle->CAN_MsgType.Tx_msg.Data[i] = data[i];
 }
 uint8_t mailbox = CAN_Transmit(pCAN_handle->CANx,&pCAN_handle->CAN_MsgType.Tx_msg); // send data frame
 //wait until data is sent successfully
 while (CAN_TransmitStatus(pCAN_handle->CANx,mailbox) != CAN_TxStatus_Ok);
}

void CAN_ReceiveData(CAN_Handle* pCAN_handle,uint32_t ID,uint8_t* data) {
	//wait until FIFO receive data
	while(CAN_MessagePending(pCAN_handle->CANx,CAN_FIFO0) < 1);
	// read from FIFO
	CAN_Receive(pCAN_handle->CANx,CAN_FIFO0,&pCAN_handle->CAN_MsgType.Rx_msg); 
	
	 //copy received data to buffer
	 for (int i = 0; i < pCAN_handle->CAN_MsgType.Rx_msg.DLC; i++) {
			data[i] = pCAN_handle->CAN_MsgType.Rx_msg.Data[i];
	 }
	 
	 //release data in FIFO after read
	 CAN_FIFORelease(pCAN_handle->CANx,CAN_FIFO0);
 }