#ifndef __RFM_DRIVER_H__
#define __RFM_DRIVER_H__

/******************** (C) COPYRIGHT 2017 EDS INDIA *****************************
* File Name     : RFM75.h					               *
* Author        : DILEEP SINGH                                                 *
* Date          : 08-NOV-2017                                                  *
* Description   : This file provides all FUNCTION & VAR Details OF RFM75       *
* Revision	: Rev 0						       	       *
*******************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

#define MAX_PACKET_LEN                  32// max value is 32


//************************ FSK COMMAND and REGISTER ****************************************//
// SPI(RFM75) commands
#define READ_REGISTER        		0x00  // REGISTER READ CMD
#define WRITE_REGISTER       		0x20  // REGISTER WRITE CMD
#define READ_RX_PAYLOAD     		0x61  // REGISTER RX PAYLOAD ADDRESS
#define WRITE_TX_PAYLOAD     		0xA0  // TX PAYLOAD REGISTER ADDRESS
#define FLUSH_TX        		0xE1  // Flush TX REGISTER CMD
#define FLUSH_RX        		0xE2  // Flush RX REGISTER CMD
#define REUSE_TX_PL     		0xE3  // Reuse TX PAYLOAD REGISTER CMD
#define ACTIVATE_CMD			0x50  // THIS CMD FOLLOWED BY 0x73 ACTIVATES BELOW 3 REGISTER
#define READ_RX_PL_WID			0x60  // Read RX-PAYLOAD width for the top R_RX_PAYLOAD in the RX FIFO.
#define W_ACK_PAYLOAD_CMD		0xA8  // Write Payload to be transmitted together with ACK packet on PIPE PPP(000-101)
#define W_TX_PAYLOAD_NOACK_CMD	        0xB0  // Used in TX mode. Disables AUTOACK on this specific packet.
#define NOP_NOP            		0xFF  // No Operation, might be used to read status REGISTER

// SPI(RFM75) REGISTER ADDRESSES 
/*--------------------------- REGISTER_BANK-0 ----------------------------------------*/
#define CONFIG                          0x00  // Config
#define EN_AA                           0x01  // Enable Auto Acknowledgment
#define EN_RXADDR                       0x02  // Enabled RX addresses
#define SETUP_AW                        0x03  // Setup ADDRESS width
#define SETUP_RETR                      0x04  // Setup Auto. Retransmission
#define RF_CH                           0x05  // RF channel
#define RF_SETUP                        0x06  // RF setup
#define STATUS                          0x07  // Status
#define OBSERVE_TX                      0x08  // Observe TX
#define CD                              0x09  // Carrier Detect
#define RX_ADDR_P0                      0x0A  // RX ADDRESS Data Pipe 0
#define RX_ADDR_P1                      0x0B  // RX ADDRESS Data Pipe 1
#define RX_ADDR_P2                      0x0C  // RX ADDRESS Data Pipe 2
#define RX_ADDR_P3                      0x0D  // RX ADDRESS Data Pipe 3
#define RX_ADDR_P4                      0x0E  // RX ADDRESS Data Pipe 4
#define RX_ADDR_P5                      0x0F  // RX ADDRESS Data Pipe 5
#define TX_ADDR                         0x10  // TX ADDRESS
#define RX_PW_P0                        0x11  // RX PAYLOAD width, Pipe 0
#define RX_PW_P1                        0x12  // RX PAYLOAD width, Pipe 1
#define RX_PW_P2                        0x13  // RX PAYLOAD width, Pipe 2
#define RX_PW_P3                        0x14  // RX PAYLOAD width, Pipe 3
#define RX_PW_P4                        0x15  // RX PAYLOAD width, Pipe 4
#define RX_PW_P5                        0x16  // RX PAYLOAD width, Pipe 5
#define FIFO_STATUS                     0x17  // FIFO Status Register
#define DYNPD			        0x1C  // Enable Dynamic PAYLOAD Length
#define FEATURE			        0x1D  // Feature Register
#define PAYLOAD_WIDTH                   0x1f  // PAYLOAD length of 256 bytes modes REGISTER ADDRESS

//interrupt status
#define STATUS_RX_DR 	                0x40
#define STATUS_TX_DS 	                0x20
#define STATUS_MAX_RT 	                0x10

#define STATUS_TX_FULL 	                0x01

//FIFO_STATUS
#define FIFO_STATUS_TX_REUSE 	        0x40
#define FIFO_STATUS_TX_FULL 	        0x20
#define FIFO_STATUS_TX_EMPTY 	        0x10

#define FIFO_STATUS_RX_FULL 	        0x02
#define FIFO_STATUS_RX_EMPTY 	        0x01

/*
#define RFM75_ON               (CSN_GPIO_Port->BSRR  = (CSN_Pin << 16))
#define RFM75_OFF               CSN_GPIO_Port->BSRR  = CSN_Pin

#define CHIP_ENABLE             CE_GPIO_Port->BSRR  = CE_Pin
#define CHIP_DISABLE            (CE_GPIO_Port->BSRR  = (CE_Pin << 16))

#define SET_MOSI                MOSI_GPIO_Port->BSRR = MOSI_Pin
#define RESET_MOSI              (MOSI_GPIO_Port->BSRR = (MOSI_Pin << 16))	

#define SET_CLK                 SCK_GPIO_Port->BSRR = SCK_Pin
#define RESET_CLK               (SCK_GPIO_Port->BSRR = (SCK_Pin << 16))
*/

#define RFM75_ON               (RFM_CSN_GPIO_Port->BSRR   = (RFM_CSN_Pin << 16))
#define RFM75_OFF               RFM_CSN_GPIO_Port->BSRR   = RFM_CSN_Pin

#define CHIP_ENABLE             RFM_CE_GPIO_Port->BSRR    = RFM_CE_Pin
#define CHIP_DISABLE            (RFM_CE_GPIO_Port->BSRR   = (RFM_CE_Pin << 16))

#define SET_MOSI                RFM_MOSI_GPIO_Port->BSRR  = RFM_MOSI_Pin
#define RESET_MOSI              (RFM_MOSI_GPIO_Port->BSRR = (RFM_MOSI_Pin << 16))	

#define SET_CLK                 RFM_SCK_GPIO_Port->BSRR   = RFM_SCK_Pin
#define RESET_CLK               (RFM_SCK_GPIO_Port->BSRR  = (RFM_SCK_Pin << 16))

typedef enum
{
    RFM_Mode_Tx,
    RFM_Mode_Rx
}RFM_Init_TypeDef;



void RFM75_INIT(RFM_Init_TypeDef RFM_Init_Struct);
char RFM_READ_WRITE(char Value);
void RFM_Write_Reg(char Addr, char Value);
char RFM_Read_Reg(char Addr);
void RFM_Read_Buf(char Addr, char *pBuf, char length);
void RFM_Write_Buf(char Addr, char *pBuf, char length);
void RFM_Send_Packet(uint8_t type, char *pBuf, uint8_t len);
void Switch_To_Rx_Mode(void);
void Switch_To_Tx_Mode(void);
void Switch_REG_Bank(char Bank);
void Set_Channel_No(unsigned char channel);
void RFM_Remote_Read(void);

#endif // __RFM_DRIVER_H__