/******************** (C) COPYRIGHT 2023 EDS INDIA *****************************
* File Name     : SPI_Flash.c					               *
* Author        : SANTANU ROY                                                 *
* Date          : 24/01/2023                                                   *
* Description   : This file defines SPI Flash & deals with its functions       *
* Revision	: Rev0			                                       *
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Main.h"


FlagStatus Flag_SFLASHOK;

ExFlash_BKP_TypeDef ExFlash_BKP_Type;
/*----- Low layer function -----*/
//static unsigned char SPI_FLASH_ReadByte(void);
static unsigned char SPI_FLASH_SendByte(unsigned char byte);
//static unsigned short SPI_FLASH_SendHalfWord(unsigned short HalfWord);
static void SPI_FLASH_WriteEnable(void);
static void SPI_FLASH_WaitForWriteEnd(void);


/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_PageSize    0x100

/* Private define ------------------------------------------------------------*/
#define WRITE      0x02  /* Write to Memory instruction */
#define WRSR       0x01  /* Write Status Register instruction */
#define WREN       0x06  /* Write enable instruction */

#define READ       0x03  /* Read from Memory instruction */
#define RDSR       0x05  /* Read Status Register instruction  */
#define RDID       0x9F  /* Read identification */
#define SE         0x20  /* Sector Erase instruction */
#define BE         0xD8  /* Block Erase instruction */ 
#define CE         0xC7  /* Bulk Erase instruction */

#define PD         0xB9  /* Deep Power DOWN Mode */

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte 0xFF  //0xA5                                                 // 0xFF in winbond

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_Init()
{
  LL_SPI_InitTypeDef SPI_InitStruct   = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  /**SPI3 GPIO Configuration
  PC10   ------> SPI3_SCK
  PC11   ------> SPI3_MISO
  PC12   ------> SPI3_MOSI
  */
  
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Configure SPI3 pins: CS, WP */

  GPIO_InitStruct.Pin = PIN_FLASH_CS;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  
  /* Deselect the FLASH: Chip Select high */
  Flash_OFF; 

  /* SPI3 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV128;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;
  LL_SPI_Init(SPI3, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI3, LL_SPI_PROTOCOL_MOTOROLA);
  
  /* Enable SPI3  */
  LL_SPI_Enable(SPI3);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(unsigned long SectorAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  Flash_ON;
  
  /* Send Sector Erase instruction */
  SPI_FLASH_SendByte(SE);
  /* Send SectorAddr high nibble address byte */
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  Flash_OFF;

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(void)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  Flash_ON;
   
  /* Send Bulk Erase instruction  */
  SPI_FLASH_SendByte(BE);
  /* Deselect the FLASH: Chip Select high */
  Flash_OFF;

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}


/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(char* pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  Flash_ON;
  
  /* Send "Write to Memory " instruction */
  SPI_FLASH_SendByte(WRITE);
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

 /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*pBuffer);
    if(*pBuffer == '\0')break;
    /* Point on the next byte to be written */
    pBuffer++;
  }

  
 /* Deselect the FLASH: Chip Select high */
  Flash_OFF;
  
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
  
 
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferWrite(char* pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite)
{
  unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  
  Flash_ON;
   

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      };
    }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
  
  Flash_OFF;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(char* pBuffer, unsigned long ReadAddr, unsigned short NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  Flash_ON;
   
  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(READ);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  
  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  Flash_OFF;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
unsigned long SPI_FLASH_ReadID(void)
{
  unsigned long Temp3 = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  Flash_ON;
   

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(RDID);

  /* Read a byte from the FLASH */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  Flash_OFF;

  Temp3 = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp3;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_PD
* Description    : Puts the Flash in Power Down Mode
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
void SPI_FLASH_PD(void)
{
  /* Select the FLASH: Chip Select low */
  Flash_ON;
   

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(PD);
  
  Flash_OFF;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(unsigned long ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  Flash_ON;
   

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(READ);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
//static u8 SPI_FLASH_ReadByte(void)
//{
//  return (SPI_FLASH_SendByte(Dummy_Byte));
//}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
static unsigned char SPI_FLASH_SendByte(unsigned char byte)
{
    int TimeOut;

    /* Loop while TXE flag is not set */
    while (!LL_SPI_IsActiveFlag_TXE(SPI3))
    {
        if (Flag_mSec)
        {
            Flag_mSec = RESET;
            TimeOut--;
            if (TimeOut == 0)
                break;
        }
    }

    /* Send byte through the SPI3 peripheral */
    LL_SPI_TransmitData8(SPI3, byte);

    /* Wait to receive a byte */
    while (!LL_SPI_IsActiveFlag_RXNE(SPI3))
    {
        if (Flag_mSec)
        {
            Flag_mSec = RESET;
            TimeOut--;
            if (TimeOut == 0)
                break;
        }
    }

    /* Return the byte read from the SPI bus */
    return LL_SPI_ReceiveData8(SPI3);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
//static unsigned short SPI_FLASH_SendHalfWord(unsigned short HalfWord)
//{
//  /* Loop while DR register in not emplty */
//  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
//
//  /* Send Half Word through the SPI3 peripheral */
//  SPI_I2S_SendData16(SPI3, HalfWord);
//
//  /* Wait to receive a Half Word */
//  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
//
//  /* Return the Half Word read from the SPI bus */
//  return SPI_I2S_ReceiveData16(SPI3);
//}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  Flash_ON;
   

  /* Send "Write Enable" instruction */
  SPI_FLASH_SendByte(WREN);

  /* Deselect the FLASH: Chip Select high */
  Flash_OFF;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SPI_FLASH_WaitForWriteEnd(void)
{
  unsigned char FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  Flash_ON;
   

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(RDSR);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    //FLASH_Status = SPI_FLASH_ReadByte();
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
   // __no_operation();__no_operation();__no_operation();
    __iar_builtin_no_operation();__iar_builtin_no_operation();__iar_builtin_no_operation();

  }
  while ((FLASH_Status && WIP_Flag) == 0x01); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  Flash_OFF;
}


/*******************************************************************************
 * Function Name  : ARAI_BKP
 * Description    : ARAI Backup
 * Input          : SERVER, MQTT_BKP_Type
 * Output         : None
 * Return         : None
 * *****************************************************************************/

void ExFlash_LOG_BKP(ExFlash_BKP_TypeDef ExFlash_BKP_Type)
{
    uint32_t FlashAddr = 0;
    char loc_buffer[50] = {0};
    
   switch (ExFlash_BKP_Type)
   {
    case EXFLASH_WRITE:
    {
      while(Log_Cnt > 0)
      {
        /*-------- SET FLASH ADDRESS FOR TAKING BACKUP INTO FLASH ------*/
        FlashAddr  = SPIFLASH_BAKUP1 + (BKP_Logs_Cnt * 256);                   
          
        /*------------- IF ADDR IS ENDING ADDR ERASE ALL PAGES ---------*/
        if(FlashAddr % 4096 == 0)SPI_FLASH_SectorErase(FlashAddr);  
        
        /*------------- Write RamBackup to Flash -----------------------*/
        SPI_FLASH_PageWrite(Log_Buffer[0],FlashAddr,256);
        
        memmove(Log_Buffer[0],Log_Buffer[1],(19 * 256));             
        memset(Log_Buffer[19],0,256);
        /*------------ Update Flash Backup Count Register --------------*/
        if(BKP_Logs_Cnt >= 80000)BKP_Logs_Cnt = 0;
        else BKP_Logs_Cnt++;

        if(Log_Cnt)Log_Cnt--;
      }
    }
    break;
    case EXFLASH_READ:
    {  
      for (uint8_t NoOf_Pkt_Cnt = 0; (NoOf_Pkt_Cnt < 15) && (BKP_Logs_Cnt > 0); NoOf_Pkt_Cnt++)  // BKP_Logs_Cnt
      {
        FlashAddr      = SPIFLASH_BAKUP1 + ((BKP_Logs_Cnt-1) * 256);      // SET FLASH ADDRESS FOR TAKING BACKUP INTO FLASH             
        SPI_FLASH_BufferRead(Log_Buffer[NoOf_Pkt_Cnt],FlashAddr,256); 
        if(Log_Buffer[NoOf_Pkt_Cnt][0] != '$')
        {
          SPI_FLASH_Init();                                                // SPI FLASH INITIALIZE 
          BKP_Logs_Cnt--;
        }
        else
        {
          BKP_Logs_Cnt--;
          Log_Cnt++;
        }
      }
    }
    break;
    
    default:
      printf("Invalid operation type: %d\n", ExFlash_BKP_Type);
      return;
  }

    /*----- WRITE TO FLASH AFTER EVERY 10 COUNTS ----------------------------*/
    if((BKP_Logs_Cnt % 20) == 0 || BKP_Logs_Cnt == 0)
    {
        sprintf(loc_buffer,"Backup1-%hu",BKP_Logs_Cnt);  
        if(Flag_SFLASHOK == SET)
        {
            FlashAddr = SPIFLASH_CONFIG;  
            SPI_FLASH_SectorErase(FlashAddr);
            SPI_FLASH_PageWrite(loc_buffer,FlashAddr,strlen(loc_buffer));
        };
    } 
}


