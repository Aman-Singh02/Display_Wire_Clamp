#include "main.h"


/*******************************************************************************
*FUNCTION NAME   : Rx                                                          *
*DESCRIPTION     : RECEIVE DATA USING RFM75                                    *
*INPUT           : NONE                                                        *
*OUTPUT          : NONE                                                        *
*******************************************************************************/
/*

1. MODBUS Request
Request for Clamp Bit Status
Function Code: 0x02 (Read Discrete Input)
Starting Address: 0x0001 (Clamp Status Address)
Quantity of Inputs: 1
Example Request Frame:


[ Slave ID | Function Code | Start Addr (0x0001) | Quantity (0x0001) | CRC ]


2. MODBUS Response
Clamp Bit Status Response
Function Code: 0x02 (Read Discrete Input)
Byte Count: 1
Input Status: 1 for active, 0 for inactive.
Example Response Frame:

mathematica
Copy code

[ Slave ID | Function Code | Byte Count (1) | Input Status | CRC ]

If the Clamp is Active: 0x01.
If the Clamp is Inactive: 0x00.


Epoch Time Response
Function Code: 0x03 (Read Holding Registers)
Byte Count: 4 (2 Registers × 2 Bytes each)
Register Values: Epoch time as a 32-bit integer split into 16-bit chunks.
Example Response Frame:

mathematica
Copy code
[ Slave ID | Function Code | Byte Count (4) | High Word (0x0002) | Low Word (0x0003) | CRC ]
High Word (0x0002): High 16 bits of the timestamp.
Low Word (0x0003): Low 16 bits of the timestamp.
4. Epoch Time Calculation
To encode epoch time:

Get the current Unix timestamp (seconds since 1970).
Split it into high and low 16-bit words:
High Word: epoch_time >> 16
Low Word: epoch_time & 0xFFFF
Example:

Epoch Time: 1700812291 (Nov 21, 2024, 12:04:51 UTC)
High Word: 0x6563 (25955 in decimal)
Low Word: 0xE713 (59155 in decimal)
To decode epoch time:

*/

FlagStatus Flag_RFData;

/*******************************************************************************
 * Function Name  : Request_Slave
 * Description    : Request data from the slave device
 * Input          : None
 * Output         : None
 * Return         : None
  *******************************************************************************/
void Request_To_Slave(uint8_t SlaveID)
{   
  /* Request for Clamp Status with MODBUS*/
  Flag_Req_Slave = RESET;
  if(SlaveID > 0)
  {
    if(Flag_Modbus_Request)
    {
      MODBUS_Request(SlaveID,0x03,0x0000,0x0002); // Request to Slave with MODBUS  for Clamp Status
      MODBUS_SendMsg(Req_Buff,8);
    }
    /* Request for Epoch Time with RFM */
    if(Flag_RFM_Request)
    {
      /* Initialize RFM75 in Tx mode  */
      RFM75_INIT(RFM_Mode_Tx);
      Set_Channel_No(0x03); // Select channel
      char Status = RFM_Read_Reg(STATUS);
      
      MODBUS_Request(SlaveID,0x03,0x0000,0x0002);
      
      /* Transmit the request to the RFM slave device */
      RFM_Send_Packet(WRITE_TX_PAYLOAD, (char *)Req_Buff,8);
      
      /* Re-Initialize RFM75 in Rx mode */
      RFM75_INIT(RFM_Mode_Rx); 
      Set_Channel_No(0x03);  // Select channel
      Status = RFM_Read_Reg(STATUS);
    }
  }
}




/*******************************************************************************
* Function Name  : RFM75_Rx_Data
* Description    : Read data from RFM75
* Input          : None
* Return         : None
*******************************************************************************/
void RFM75_Rx_Data(void)
{
  unsigned char Lenght,Status,FIFO_Status;                                      // LOCAL VARIABES 
  Status = RFM_Read_Reg(STATUS);                                                // READ REGISTER STATUS's VALUE
 
 if((STATUS_RX_DR & Status) == 0x40)                                            // IF RECEIVE DATA READY (RX_DR) INTERRUPT
 {
   do
   {
     Lenght = RFM_Read_Reg(READ_RX_PL_WID);                                     // READ LENGTH
     
     if(Lenght <= MAX_PACKET_LEN)
     {
       RFM_Read_Buf(READ_RX_PAYLOAD, RF_Buffer, Lenght);                        // READ RECEIVE PAYLOAD FROM RX_FIFO BUFFER

       if(RF_Buffer[0] == All_SlaveIDs[Req_SlaveID][0])Flag_RFData = SET; else memset(RF_Buffer,0,sizeof(RF_Buffer));
       
       /* Process MODBUS Response */
       if(Flag_RFData)
       {
         Flag_RFData = RESET;
         for (uint8_t i = 0; i < Lenght; i++)
         {
            MODBUS_STATE = MODBUS_Respones(MODBUS_STATE, RF_Buffer[i]);         // PROCESS MODBUS RESPONSE
         }
         memset(RF_Buffer,0,sizeof(RF_Buffer));
         Lenght = 0;
       }
     }
     else {RFM_Write_Reg(FLUSH_RX, 0);}				                // FLUSH Rx
     
     FIFO_Status = RFM_Read_Reg(FIFO_STATUS);                                   // READ REGISTER FIFO_STATUS's VALUE 
   }
   while((FIFO_Status & FIFO_STATUS_RX_EMPTY) == 0);                            // WHILE NOT EMPTY

 }
 RFM_Write_Reg(WRITE_REGISTER | STATUS, Status);                                // CLEAR RX_DR or TX_DS or MAX_RT INTERRUPT FLAG  
}


/*******************************************************************************
 * Function Name  : __SET_SlaveID
 * Description
 * Input          : None
 * Output         : None
 * Return         : None
  *******************************************************************************/
void __SET_SlaveID(void)
{
  if(Flag_Set_SlaveID) 
  {
    Flag_Wait_Set_Response = SET;
    Flag_Set_SlaveID = RESET; MODBUS_SendMsg(MODBUS_Reply(Req_SlaveID, 0x04, ConfSlaveId, 0x02),8);
  } 
  else if(Flag_SSId_RespOk)
  {
    Flag_SSId_RespOk = RESET;
    if(Flag_TCP_Connect)send_data("SET : SLAVE DEVICE ID DONE");
  }
}

 /*******************************************************************************
  * Function Name  : Testing_Process
  * Description    : Test Process
  * Input          : None
  * Output         : None
  * Return         : None
  * *****************************************************************************/
  void Testing_Process(void)
  {
    /* Start Testing */
    if(!Flag_StartTime)
    {
      Flag_StartTime            = SET;
      Flag_Display_Update       = SET;
      Flag_Test_Failed          = RESET;
      Flag_LCD_TEST_PROGRESS    = SET;
      Start_TimeStruct.Hours    = sTime.Hours;               // END TIME - Hours (int)
      Start_TimeStruct.Minutes  = sTime.Minutes;             // END TIME - Minutes (int)
      Start_TimeStruct.Seconds  = sTime.Seconds;             // END TIME - Seconds (int)
      Slave_Req_Time            = 0;
      Not_Conn_Slave_Cnt        = 0;
      Conn_Slave_Cnt            = 0;
      Slave_Device_ID           = 0;
      Total_Test_Time           = 0;
      Req_SlaveID               = 0;
      
      for(int i=0; i <= MAX_SLAVE_DEVICES; i++)
      {
        All_SlaveIDs[i][1] = 0;
      }
      memset(Conn_SlaveId,0, sizeof(Conn_SlaveId));
      memset(Not_Conn_SlaveId,0, sizeof(Not_Conn_SlaveId));
    }
    
    /* Test Time Out */
    if(Slave_Req_Time > (Default_Test_TimeOut * MAX_SLAVE_DEVICES))
    {
      Slave_Req_Time           = 0;
      Req_SlaveID              = 0;
      Flag_Failed_SRID         = RESET;
      Flag_RecScanner          = RESET;
      Flag_Req_Slave           = RESET;
      Flag_StartTime           = RESET;
      Flag_RAM_BKP             = SET;
    } 
    
     /* Total Test Time */
    Total_Test_Time++;

    /* Check Slave Request Time */
   if((Sec_Tick % 999 == 0) && Flag_RFM_Request)//RFM Request
    {Flag_Req_Slave = Flag_Failed_SRID = SET; Resp_Wait_Time = 999;} 

    else if((Sec_Tick % Default_SReq_Time == 0) && Flag_Modbus_Request) // Modbus Request
    {
      Flag_Req_Slave = Flag_Failed_SRID = SET;
      Resp_Wait_Time = 70;//tbd g
      //test_cnt++;
    }
    
    /* wait for response time */
    if(Resp_Wait_Time)Resp_Wait_Time--;
    if(Resp_Wait_Time == 1)
    {
      Resp_Wait_Time = 0;
      
      if(Req_SlaveID >= MAX_SLAVE_DEVICES) //&& Conn_Slave_Cnt != MAX_SLAVE_DEVICES)
      {
        for(int i=0; i <= MAX_SLAVE_DEVICES; i++)
          All_SlaveIDs[i][1] = 0;

        Req_SlaveID        = 0;
        Not_Conn_Slave_Cnt = 0;
        Conn_Slave_Cnt     = 0;
        memset(Conn_SlaveId,0, sizeof(Conn_SlaveId));
      } 
      else 
      {
        Not_Conn_SlaveId[Not_Conn_Slave_Cnt] =  All_SlaveIDs[Req_SlaveID][0];
        Req_SlaveID++;
        Not_Conn_Slave_Cnt++; 
      }
    }
    
    /* Wait for 50 mSec for Slave Response and then send new request*/
    if(Flag_Failed_SRID == SET) Slave_Req_Time++; //else Slave_Req_Time = 0;
  }