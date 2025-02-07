/******************** (C) COPYRIGHT 2023 EDS INDIA *****************************
* File Name     : SPI_Flash.h	                         	               *
* Author        : SANTANU ROY                                                 *
* Date          : 24/01/2023                                                   *
* Description   : This File Defines functions of SPI Flash                     *
* Revision	    : Rev0						               *
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#define SPI_FLASH_ID          0xEF4018 //0x2C25


/* Exported macro ------------------------------------------------------------*/
#define PORT_FLASH_CS           GPIOA
#define PIN_FLASH_CS            LL_GPIO_PIN_15 //LL_GPIO_Pin_15                                     // SPI1 NSS CHIP SELECT PIN
 

#define PORT_FLASH              GPIOB
#define PIN_FLASH_SCK           GPIO_Pin_3                                     // SPI1 NSS CHIP SELECT PIN
#define PIN_FLASH_MISO          GPIO_Pin_4                                     // SPI1 NSS CHIP SELECT PIN
#define PIN_FLASH_MOSI          GPIO_Pin_5                                     // SPI1 NSS CHIP SELECT PIN
  


#define Flash_ON                SPI3_CS_GPIO_Port->BSRR = (SPI3_CS_Pin << 16)
#define Flash_OFF                 SPI3_CS_GPIO_Port->BSRR  = SPI3_CS_Pin

// Just for Verification
#define  FLASH_WriteAddress     0xF00000                                        // Address above 15 MB 
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  FLASH_ID             


#define SPIFLASH_CONFIG          0x0000                                         // SPI FLASH CONFIGURATION TABLE FOR BACKUP PACKETS FOR ALL (SERVER 1 ,SERVER 2 & EMERGENCY)  
#define SPIFLASH_BAKUP1          0x1000                                         // SPI FLASH PRIMARY SERVER BACKUP 
#define SPIFLASH_BAKUP2          0x9C5000                                       // SPI FLASH SECONDARY SERVER BACKUP
#define SPIFLASH_BAKUP3          0xFB0000                                       // SPI FLASH EMERGENCY 1 PACKET 
#define SPIFLASH_FOTA            0xF00000                                       // FOR STORE PROGRAM DURING FOTA // address above 15 MB of flash
#define SPIFLASH_CONFIG_ODO      0xFFF000
#define SampleCnt                1
#define SPIFLASH_ERROR_LOG       0xF00000                                       // Error Log 


extern FlagStatus Flag_SFLASHOK;

/*--------------------- MQTT BKT Types ----------------------------------------------*/
typedef enum {
    EXFLASH_WRITE,
    EXFLASH_READ
} ExFlash_BKP_TypeDef;


/*-------- Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(unsigned long SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(char* pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite);
void SPI_FLASH_BufferWrite(char* pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite);
void SPI_FLASH_BufferRead(char* pBuffer, unsigned long ReadAddr, unsigned short NumByteToRead);
unsigned long SPI_FLASH_ReadID(void);
void SPI_FLASH_PD(void);
void SPI_FLASH_StartReadSequence(unsigned long ReadAddr);

void ExFlash_LOG_BKP(ExFlash_BKP_TypeDef ExFlash_BKP_Type);
#endif /* __SPI_FLASH_H */