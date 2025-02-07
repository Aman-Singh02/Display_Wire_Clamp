#include "main.h"

uint8_t op_status;
uint16_t i, j;

unsigned char Loop_Count;
//Bank1 register initialization value
//In the array RegArrFSKAnalog,all the register value is the byte reversed!!!!!!!!!!!!!!!!!!!!!
const unsigned long Bank1_Reg0_13[]={       //latest config txt
0xE2014B40,
0x00004BC0,
0x028CFCD0,
0x21390099,
0xDB8296F9,
0xB60F0624,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00127300,
0x36B48000,
};

const unsigned long Bank1_Reg0_4[]={
0xDB8A96F9,
0x1B8296F9,
0xDB8296F9	
};
const unsigned long Bank1_Reg0_5[]={
0xB60F0624,
0xA60F0624,
0xB60F0624	
};

const uint8_t Bank1_Reg14[]=
{
  0x41,0x20,0x08,0x04,0x81,0x20,0xCF,0xF7,0xFE,0xFF,0xFF // 0xFFFFFEF7CF208104082041
};

//Bank0 register initialization value
const uint8_t Bank0_Reg[][2]={
{0,0x33},//reflect RX_DR\TX_DS\MAX_RT,Enable CRC ,2byte,POWER UP,PRX
{1,0x01},//Enable auto acknowledgement data pipe5\4\3\2\1\0
{2,0x01},//Enable RX Addresses pipe5\4\3\2\1\0
{3,0x01},//RX/TX address field width 3byte
{4,0x00},//auto retransmission dalay (4000us),auto retransmission count(15)
{5,0x17},//23 channel
{6,0x07},//air data rate-1M,out power 5dbm,setup LNA gain.
{7,0x07},//
{8,0x00},//
{9,0x00},
{12,0xc3},//only LSB Receive address data pipe 2, MSB bytes is equal to RX_ADDR_P1[39:8]
{13,0xc4},//only LSB Receive address data pipe 3, MSB bytes is equal to RX_ADDR_P1[39:8]
{14,0xc5},//only LSB Receive address data pipe 4, MSB bytes is equal to RX_ADDR_P1[39:8]
{15,0xc6},//only LSB Receive address data pipe 5, MSB bytes is equal to RX_ADDR_P1[39:8]
{17,0x20},//Number of bytes in RX payload in data pipe0(32 byte) 
{18,0x20},//Number of bytes in RX payload in data pipe1(32 byte)
{19,0x20},//Number of bytes in RX payload in data pipe2(32 byte)
{20,0x20},//Number of bytes in RX payload in data pipe3(32 byte)
{21,0x20},//Number of bytes in RX payload in data pipe4(32 byte)
{22,0x20},//Number of bytes in RX payload in data pipe5(32 byte)
{23,0x00},//fifo status
{28,0x3F},//Enable dynamic payload length data pipe5\4\3\2\1\0
{29,0x07}//Enables Dynamic Payload Length,Enables Payload with ACK,Enables the W_TX_PAYLOAD_NOACK command 
};


const uint8_t RX0_Address[]={0x34,0x43,0x10,0x10,0x01};//Receive address data pipe 0
const uint8_t RX1_Address[]={0x39,0x38,0x37,0x36,0xc2};////Receive address data pipe 1


unsigned char data_rate;
char bite_ctr,Return_val;

////////////////////////////////////////////////////////////////////////////////
//                          RFM75 initialization                              //
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Function Name  : RFM75_INIT
* Description    : Initialize RFM75 with some Config
* Input          : None
* Return         : None
*******************************************************************************/
void RFM75_INIT(RFM_Init_TypeDef RFM_Init_Struct)
{
  uint8_t Temp_Var, Value;
  char WriteArr[12];
  
  HAL_Delay(100);                                                               //delay more than 50ms.
  
  Switch_REG_Bank(0);
  /********************Write Bank_0 register***********************************/
  
  for(i = 0; i < 5; i++)
  {
    RFM_Write_Reg((WRITE_REGISTER | Bank0_Reg[i][0]), Bank0_Reg[i][1]);
  }
  //********************Select channel*************************
  
  RFM_Write_Reg(WRITE_REGISTER|5, 0); //first channel 0  CH_No*2
  Temp_Var = 0x28;                          //data rate 2M
  Temp_Var |= 0x07;
  RFM_Write_Reg(WRITE_REGISTER|6,Temp_Var); //power 0dbm
  
  for(i = 7; i < 21; i++)
  {
    RFM_Write_Reg((WRITE_REGISTER | Bank0_Reg[i][0]), Bank0_Reg[i][1]);
  }
  /****************************************************************************/
  
  //reg 10 - Rx0 addr
  for(i = 0; i < 5; i++){WriteArr[i]=RX0_Address[i];}
  RFM_Write_Buf((WRITE_REGISTER | 10), &WriteArr[0],5);
  
  //REG 11 - Rx1 addr
  for(i = 0; i < 5; i++){WriteArr[i]=RX1_Address[i];}
  RFM_Write_Buf((WRITE_REGISTER | 11), &(WriteArr[0]),5);
  
  //REG 16 - TX addr
  for(i = 0; i < 5; i++){WriteArr[i]=RX0_Address[i];}
  RFM_Write_Buf((WRITE_REGISTER | 16), &(WriteArr[0]),5);
  
  
  Value = RFM_Read_Reg(29);                                                     //read Feature Register  Payload With ACK (REG28,REG29).
  if(Value == 0){RFM_Write_Reg(ACTIVATE_CMD,0x73);}                             // Value!=0 showed that chip has been actived.so do not active again.
  
  for(i=22;i>=21;i--)
  {
    RFM_Write_Reg((WRITE_REGISTER | Bank0_Reg[i][0]), Bank0_Reg[i][1]);          //Enable Dynamic Payload length ,Enables the W_TX_PAYLOAD_NOACK command
  }
  

  
  /********************Write Bank_1 register***********************************/
  Switch_REG_Bank(1);
  
  for(i = 0; i <= 3; i++)//reverse
  {
    for(j = 0; j < 4; j++){WriteArr[j] = (Bank1_Reg0_13[i] >> (8*(j))) & 0xff;}
    
    RFM_Write_Buf((WRITE_REGISTER | i), &(WriteArr[0]),4);
  }
  
  for(j = 0; j < 4; j++){WriteArr[j] = (Bank1_Reg0_4[data_rate + 1] >> (8*(j))) & 0xff;}
  
  RFM_Write_Buf((WRITE_REGISTER|i),&(WriteArr[0]),4);
  
  for(j = 0; j < 4; j++){WriteArr[j] = (Bank1_Reg0_5[data_rate+1] >> (8*(j))) & 0xff;}
  
  RFM_Write_Buf((WRITE_REGISTER|i),&(WriteArr[0]),4);
  
  for(i = 6; i <= 8; i++)//reverse
  {
    for(j = 0; j < 4; j++){WriteArr[j] = (Bank1_Reg0_13[i] >> (8*(j))) & 0xff;}
    
    RFM_Write_Buf((WRITE_REGISTER|i), &(WriteArr[0]),4);
  }
  
  for(i = 9; i <= 13; i++)
  {
    for(j=0;j<4;j++){WriteArr[j] = (Bank1_Reg0_13[i] >> (8*(3-j))) & 0xff;}
    
    RFM_Write_Buf((WRITE_REGISTER|i),&(WriteArr[0]),4);
  }
  
  for(i = 0; i < 11; i++){WriteArr[i] = Bank1_Reg14[i];}
  RFM_Write_Buf((WRITE_REGISTER|14),&(WriteArr[0]),11);
  
  //toggle REG4<25,26>
  for(j=0;j<4;j++){WriteArr[j] = (Bank1_Reg0_13[4] >> (8*(j))) & 0xff;}
  
  WriteArr[0] = WriteArr[0] | 0x06;
  RFM_Write_Buf((WRITE_REGISTER | 4), &(WriteArr[0]), 4);
  
  WriteArr[0]=WriteArr[0] & 0xf9;
  RFM_Write_Buf((WRITE_REGISTER | 4),&(WriteArr[0]),4);
  
  HAL_Delay(100);
  
  //********************switch back to Bank0 register access******************
  Switch_REG_Bank(0);
  
  switch (RFM_Init_Struct)
  {
    case RFM_Mode_Tx:  Switch_To_Tx_Mode();break; // Init Tx Mode
    case RFM_Mode_Rx:  Switch_To_Rx_Mode();break; // Init Rx Mode
  }
}

/*******************************************************************************
* Function Name  : RFM_READ_WRITE
* Description    : Read/Writes value 'value' from/to register
* Input          : Value 
* Return         : Value
*******************************************************************************/
char RFM_READ_WRITE(char Value)
{
  char Bit_Cnt = 0;
  for(Bit_Cnt = 0; Bit_Cnt < 8; Bit_Cnt++)
  {
    if(Value & 0x80){SET_MOSI;}
    else{RESET_MOSI;}
    
    Value = (Value << 1);                                                       // shift next bit into MSB..
    SET_CLK;                					                // Set SCK high..
    if(LL_GPIO_IsInputPinSet(RFM_MISO_GPIO_Port,RFM_MISO_Pin) == SET)Value |= 1;
    else Value |= 0;       		                                        // capture current MISO bit
    RESET_CLK;          		                                        // ..then set SCK low again
  }
  return Value;
}

/*******************************************************************************
* Function Name  : RFM_Write_Reg
* Description    : Writes value to register 'reg'
* Input          : Addr, Value 
* Return         : NONE
*******************************************************************************/
void RFM_Write_Reg(char Addr, char Value)
{
  RFM75_ON;
  
  RFM_READ_WRITE(Addr);                                                       // select register
  RFM_READ_WRITE(Value);                                                      // ..and write value to it..
  
  RFM75_OFF;
}

/*******************************************************************************
* Function Name  : RFM_Read_Reg
* Description    : Writes value to register 'reg'
* Input          : Addr 
* Return         : Return_val
*******************************************************************************/
char RFM_Read_Reg(char Addr)
{
  RFM75_ON;
  
  RFM_READ_WRITE(Addr);                                                         // Select register to read from..
  Return_val = RFM_READ_WRITE(0);                                               // ..then read register value
  
  RFM75_OFF;
  
  return Return_val;  
}

/*******************************************************************************
* Function Name  : RFM_Read_Buf
* Description    : Read buffer from the register
* Input          : Addr, pBuf, length
* Return         : None
*******************************************************************************/
void RFM_Read_Buf(char Addr, char *pBuf, char length)
{
  unsigned char Loop_Count = 0;
  RFM75_ON;
  
  RFM_READ_WRITE(Addr);       		                                        // Select register to write, and read status uint8_t
  
  for(Loop_Count = 0; Loop_Count < length; Loop_Count++)
  {
    pBuf[Loop_Count] = RFM_READ_WRITE(0);                                       // Perform RFM_READ_WRITE to read uint8_t from RFM70
  }
  
  RFM75_OFF;
}

/*******************************************************************************
* Function Name  : RFM_Write_Buf
* Description    : Read buffer from the register
* Input          : Addr, pBuf, length
* Return         : None
*******************************************************************************/
void RFM_Write_Buf(char Addr, char *pBuf, char length)
{
  unsigned short Loop_Count = 0;
  RFM75_ON;
  
  RFM_READ_WRITE(Addr);                                                         // Select register to write to and read status uint8_t
  for(Loop_Count = 0; Loop_Count < length; Loop_Count++)                        // then write all uint8_t in buffer(*pBuf)
  {
    RFM_READ_WRITE(*pBuf++);
   // for(int i = 0; i < 1000; i++) __NOP();
  }
  
  RFM75_OFF;
}

/*******************************************************************************
* Function Name  : RFM_Send_Packet
* Description    : Send Packet to RFM
* Input          : type, pBuf, length
* Return         : None
*******************************************************************************/
void RFM_Send_Packet(uint8_t type, char *pBuf, uint8_t len)
{
  uint8_t FIFO_Status;
  
  FIFO_Status = RFM_Read_Reg(FIFO_STATUS);                                      // read register FIFO_STATUS's value
  if((FIFO_Status & FIFO_STATUS_TX_FULL) == 0)                                  //if not full, send data (write buff)
  {
    RFM_Write_Buf(type, pBuf, len);                                             // Writes data to buffer
  }      
}

/*******************************************************************************
* Function Name  : Switch_To_Rx_Mode
* Description    : Switch to Rx mode
* Input          : None
* Return         : None
*******************************************************************************/
void Switch_To_Rx_Mode(void)
{
  char value;
  
  RFM_Write_Reg(FLUSH_RX,0);                                                    //flush Rx
  
  value = RFM_Read_Reg(STATUS);	                                                // read register STATUS's value
  RFM_Write_Reg(WRITE_REGISTER | STATUS, value);                                 // clear RX_DR or TX_DS or MAX_RT interrupt flag
  
  //CE=0;
  CHIP_DISABLE;
  
  value = RFM_Read_Reg(CONFIG);	                                                // read register CONFIG's value
  
  //PRX
  value = value|0x01;//set bit 1
  RFM_Write_Reg(WRITE_REGISTER | CONFIG, value);                                 // Set PWR_UP bit, enable CRC(2 length) & Prim:RX. RX_DR enabled..
  //CE=1;
  CHIP_ENABLE;
}

/*******************************************************************************
* Function Name  : Switch_To_Tx_Mode
* Description    : Switch to Tx mode
* Input          : None
* Return         : None
*******************************************************************************/
void Switch_To_Tx_Mode(void)
{
  char value;
  RFM_Write_Reg(FLUSH_TX, 0);                                                   //flush Tx
  
  //CE=0;
  CHIP_DISABLE;
  value = RFM_Read_Reg(CONFIG);	                                                // read register CONFIG's value
  
  value = value&0xfe;//set bit 0
  RFM_Write_Reg(WRITE_REGISTER | CONFIG, value); // Set PWR_UP bit, enable CRC(2 length) & Prim:RX. RX_DR enabled.
  
  //CE=1;
  CHIP_ENABLE;
}

/*******************************************************************************
* Function Name  : Switch_REG_Bank
* Description    : Switch Between Bank 0 or 1
* Input          : Bank - 1:Bank1 0:Bank0
* Return         : None
*******************************************************************************/
void Switch_REG_Bank(char Bank)
{
  char Temp;
  
  Temp = RFM_Read_Reg(STATUS);
  Temp = Temp & 0x80;
  
  if(((Temp) && (Bank == 0)) || (((Temp) == 0) && (Bank)))
  {
    RFM_Write_Reg(ACTIVATE_CMD, 0x53);
  } 
}

/*******************************************************************************
* Function Name  : Set_Channel_No
* Description    : Set channel number
* Input          : channel
* Return         : None
*******************************************************************************/
void Set_Channel_No(unsigned char channel)
{
  RFM_Write_Reg((WRITE_REGISTER|5),(channel));
}