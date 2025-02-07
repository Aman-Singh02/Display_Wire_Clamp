/******************** (C) COPYRIGHT 2024 EDS-INDIA *****************************
* File Name     : MODBUS.c					               *
* Author        : Santosh Kumar                                                *
* Date          : 28-11-2024                                                   *
* Description   : This file contains funcn related to Sampling and transmitting*
*                 data pkts                                                    *  
********************************************************************************/

#include "main.h"
/* -------------------------------------------------------------------------- */
MODBUS_RcvState_TypeDef MODBUS_STATE;
MODBUS_Request_TypeDef  MODBUS_REQ;
MODBUS_Response_TypeDef MODBUS_RESP;

uint8_t ByteMSB, ByteLSB ;

FlagStatus Flag_DataRdy,
           Flag_BitRead, 
           Flag_RegRead, 
           Flag_RegWrite;

uint16_t len;
 
volatile uint8_t  RcvCnt,RcvState;
/* -------------------------------------------------------------------------- */

/*******************************************************************************
* Function Name  : MODBUS_Transmit
* Description    : Transmits a MODBUS response over UART.
* Input          : Tx_Header, length of buffer, UART
* Output         : None
* Return         : None
*******************************************************************************/
void MODBUS_SendMsg(uint8_t *Tx_Header, uint8_t Len)
{   
  /* Enable RS485 Transmitter */
  MODBUS_Tx_EN;
  mSec_Delay(5);
  for(uint8_t n = 0; n < Len; n++) 
  {
    while (!LL_USART_IsActiveFlag_TXE(MODBUS_UART));   // Wait for TXE flag to be set
    LL_USART_TransmitData8(MODBUS_UART, Tx_Header[n]); // Transmit data
    nSec_Delay(20);
  }
  mSec_Delay(5);
  /* Wait for transmission to complete */
  MODBUS_Tx_DIS;
}


/*******************************************************************************
 * Function Name  : MODBUS_Process_ReadDiscreteInputs
 * Description    : Handles MODBUS read request (Function Code 0x02)
 * Input          : deviceID, Tx_Header, datavalues[], length
 * Output         : None
 * Return         : None
 * *****************************************************************************/

MODBUS_RcvState_TypeDef MODBUS_Respones(MODBUS_RcvState_TypeDef MODBUS_STATE, char RespData) 
{
  switch (MODBUS_STATE) 
  {
    case INIT_RCV:
        /* Initialize the state, check if received RespData matches */
        Slave_Device_ID = RespData;
        MODBUS_STATE = (Slave_Device_ID  == All_SlaveIDs[Req_SlaveID][0]) ? GET_FCODE : INIT_RCV; // == All_SlaveIDs[Req_SlaveID][0]
        break;

    case GET_FCODE:
        /* Retrieve the function code and reset the receive counter */
        MODBUS_RESP.FunctionCode = RespData;
        RcvCnt = 0;

        /* Set appropriate flags based on function code */
        if (MODBUS_RESP.FunctionCode == 0x02){MODBUS_STATE = GET_STRT_ADDR; Flag_BitRead = SET;}    // Bit Read operation
        else if (MODBUS_RESP.FunctionCode == 0x03) {Flag_RegRead = SET; MODBUS_STATE = GET_STRT_ADDR; } // Register Read operation
        else if ((MODBUS_RESP.FunctionCode == 0x04) || (MODBUS_RESP.FunctionCode == 0x10)){ Flag_RegWrite = SET; MODBUS_STATE = GET_LENGTH; } // Register Write operation
        else
        {
            MODBUS_STATE = INIT_RCV;  // Reset to initialization state for invalid function code
        }
        break;

    case GET_STRT_ADDR:
        /* Retrieve the start address (two bytes: MSB and LSB) */
        (RcvCnt++ == 0) ? (ByteMSB = RespData) : (ByteLSB = RespData);

        /* If both bytes are received, compute the start address and move to GET_LENGTH */
        if (RcvCnt >= 2) 
        {
            MODBUS_RESP.StartAddr = (ByteMSB << 8) | ByteLSB; // Combine MSB and LSB into start address
            MODBUS_STATE = GET_LENGTH;        // Move to length retrieval state
            RcvCnt = 0;         
        }
        break;

    case GET_LENGTH:
        /* Retrieve the length of the data (two bytes: MSB and LSB) */
       (RcvCnt++ == 0) ? (ByteMSB = RespData) : (ByteLSB = RespData);
        if (RcvCnt >= 2) 
        {
            MODBUS_RESP.NumBytes = (ByteLSB << 8) | ByteMSB;  // Combine MSB and LSB into length
            MODBUS_STATE = (Flag_RegWrite == SET) ? GET_DATA : GET_CRC; // Determine next state
            RcvCnt = 0;       
        }
        break;

    case GET_DATA:
        /* Receive data bytes and store in Holding Register */
        MODBUS_RESP.HRegister[RcvCnt++] = RespData;

        /* If all bytes are received, move to CRC check state */
        if (RcvCnt >= MODBUS_RESP.NumBytes) 
        {
            MODBUS_STATE = GET_CRC;  // Move to CRC state
            RcvCnt = 0;
        }
        break;

    case GET_CRC:
        /* Retrieve the CRC value (two bytes: MSB and LSB) */
        (RcvCnt++ == 0) ? (ByteMSB = RespData) : (ByteLSB = RespData);

        /* If both bytes are received, compute CRC and finalize */
        if (RcvCnt >= 2) 
        {
            MODBUS_RESP.RSP_CRC = (ByteMSB << 8) | ByteLSB; // Combine MSB and LSB into CRC
            RcvCnt = 0;               
            
           if((MODBUS_RESP.HRegister[1] == 0x00) || (MODBUS_RESP.HRegister[2] == 0x01) || (MODBUS_RESP.HRegister[2] == 0x01))
           {
            Conn_SlaveId[Conn_Slave_Cnt] = Slave_Device_ID;
            All_SlaveIDs[Req_SlaveID][1] = 1;
            Flag_DataRdy                 = SET;     // Set data ready flag
            Flag_Req_Slave               = SET;
            RespData                     = 0;
            Resp_Wait_Time               = 0;
            
            Conn_Slave_Cnt++;
            
            Flag_Display_Update          = SET; // Display Update

            if(Conn_Slave_Cnt >= MAX_SLAVE_DEVICES)
            {
              Flag_RecScanner  = Flag_Req_Slave  = RESET;
              Flag_StartTime   = RESET;
              Flag_RAM_BKP     = SET;
              Req_SlaveID      = 0;
            }
            else
            {
              Req_SlaveID++;
            }
           }
           MODBUS_STATE = INIT_RCV;                // Reset to initialization state
        }
        break;

    default:
        /* Handle invalid state by resetting to initialization */
        MODBUS_STATE = INIT_RCV;
        break;
  }
  
  return  MODBUS_STATE;
}

/*******************************************************************************
 * Function Name  : Request_Slave
 * Description    : Request data from the slave device
 * Input          : SlaveId, FunctionCode, StartAddr, Req_Data_Size
 * Output         : None
 * Return         : None
 * Note           : None
 * Example        : Request_To_Slave(0x01, 0x02, 0x0001, 0x0001);
  *******************************************************************************/
uint8_t *MODBUS_Request(uint16_t SlaveId, uint8_t FunctionCode, uint16_t StartAddr, uint16_t Req_Data_Size)
{
  //static uint8_t Req_Buff[9] = {0};                      // Buffer to hold Modbus request message
  uint8_t index       = 6;                        // Index after filling the required fields
  uint16_t crc;                                   // CRC value to append at the end

  /* Step 1: Fill in the request buffer with the necessary fields */
  Req_Buff[0]       = SlaveId & 0xFF;              // Slave ID (1 byte)
  Req_Buff[1]       = FunctionCode;                // Function code (1 byte)
  Req_Buff[2]       = (StartAddr >> 8) & 0xFF;     // Start address high byte
  Req_Buff[3]       = StartAddr & 0xFF;            // Start address low byte
  Req_Buff[4]       = (Req_Data_Size >> 8) & 0xFF; // Data size high byte
  Req_Buff[5]       = Req_Data_Size & 0xFF;        // Data size low byte

  /* Step 2: Calculate and append CRC */
  crc = MODBUS_CRC(Req_Buff, index);
  Req_Buff[index++] = crc & 0xFF;                  // CRC low byte
  Req_Buff[index++] = (crc >> 8) & 0xFF;           // CRC high byte

  /* Step 3: Calculate the correct message length */
 // uint8_t length    = index;
  
  /* Step 4: Send the request to the slave device */
 // MODBUS_SendMsg(Req_Buff, length);  // Send the message
  
  /* Request buffer Set Zero */
 // memset(Req_Buff, 0, sizeof(Req_Buff));
  return 0; //Req_Buff;
}


/*******************************************************************************
 * Function Name  : MODBUS_REPLY
 * Description    : Reply to the master device
 * Input          : SlaveId, FunctionCode, StartAddr, Req_Data_Size
 * Output         : None
 * Return         : None
 * Note           : None
 * Example        : MODBUS_Reply(0x01, 0x02, 0x0001, 0x0001);
  *******************************************************************************/
uint8_t *MODBUS_Reply(uint16_t SlaveId, uint8_t FunctionCode, uint16_t Data, uint16_t DataSize)
{
    /* Buffer to hold the MODBUS response message */
    static uint8_t response[256]; 
    uint16_t crc;
    
    uint8_t *data = (uint8_t *)malloc(DataSize * sizeof(uint8_t));
    if (*data == NULL) { return 0;}
   // uint16_t dataSize = DataSize * 2;

    /* 1. Prepare the data to be sent in the response */
    for (uint16_t i = 0; i < DataSize; i++) {
        data[i] = Data + i;
    }

    /* 2. Build the MODBUS response message */
    uint16_t index = 0;

    /* Add Slave ID to the response */
    response[index++] = (uint8_t)SlaveId;

    /* Add Function Code to the response */
    response[index++] = FunctionCode;

    /* Add Byte Count (number of data bytes in the response) */
    response[index++] = (uint8_t)(DataSize);

    /* Add Data (register values) to the response */
    for (uint16_t i = 0; i < DataSize; i++) {
        response[index++] = (data[i] >> 8) & 0xFF; // High byte of the register value
        response[index++] = data[i] & 0xFF;        // Low byte of the register value
    }

    /* 3. Calculate the CRC for the response message */
    crc = MODBUS_CRC(response, index);
    response[index++] = (crc & 0xFF);       // Add CRC Low byte
    response[index++] = (crc >> 8) & 0xFF;  // Add CRC High byte

    /* 4. Send the complete MODBUS response message */
    //MODBUS_SendMsg(response, index);

    /* Free dynamically allocated memory */
    if(data != NULL)free(data);
    
    return response;
}

/*******************************************************************************
* Function Name  : MODBUS_CRC
* Description    : Computes the CRC-16 for MODBUS data packets
* Input          : data: Pointer to the data array
*                  length: Length of the data array
* Output         : None
* Return         : 16-bit calculated CRC value
*******************************************************************************/
uint16_t MODBUS_CRC(uint8_t* data, uint16_t length) 
{
  /* CRC-16 MODBUS algorithm */
  uint16_t crc = 0xFFFF;

  /* Process each bit in the byte */
  while (length--) 
  {
    crc ^= *data++; /* Check if the least significant bit (LSB) is set */
    for (uint8_t i = 0; i < 8; i++) 
    { 
      /* If LSB is set, shift right and XOR with the polynomial (0xA001)*/
      crc = (crc >> 1) ^ (0xA001 * (crc & 1)); // 0xA001 is the polynomial
    }
  }
  return crc; // Return the calculated CRC
}
