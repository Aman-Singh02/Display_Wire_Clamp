/*
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************

  File:		  tcpServerRAW.h
  Author:     ControllersTech.com
  Updated:    26-Jul-2021

  ***************************************************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ***************************************************************************************************************
*/


#ifndef INC_TCPSERVERRAW_H_
#define INC_TCPSERVERRAW_H_
#include "main.h"
#define ETH_RX_BUFFER    256


//void TCP_Send_Data(char *pBuff);
//void Send_TCPData(int length);
//

extern 
FlagStatus
Flag_TCP_Connect;   
//err_t TCP_Server_SendData(struct tcp_pcb *tpcb, const char *data, uint16_t len);
void Send_Data_To_Client(const char *message);
void some_function_to_send_data(void);
void send_data(const char *data);

















void TCP_server_init(void);

#endif /* INC_TCPSERVERRAW_H_ */
