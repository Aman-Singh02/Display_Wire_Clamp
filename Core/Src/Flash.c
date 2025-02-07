#include "main.h"

#define FLASH_EMPTY_VALUE 0xFFFFFFFF
#define FLASH_USER_START_ADDR ADDR_FLASH_SECTOR_23                                          /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR ADDR_FLASH_SECTOR_23 + GetSectorSize(ADDR_FLASH_SECTOR_23) - 1 /* End @ of user Flash area : sector start address + sector size -1 */

uint32_t FirstSector = 0, NbOfSectors = 0;
uint32_t Address = 0, SECTORError = 0;
__IO uint32_t data32 = 0, MemoryProgramStatus = 0;

// Variable used for Erase procedure/
static FLASH_EraseInitTypeDef EraseInitStruct;

static uint32_t GetSector(uint32_t Address);
//static uint32_t GetSectorSize(uint32_t Sector);

void HAL_FLASH_Read(uint8_t *Buff, uint32_t Addr, uint32_t Buff_Len)
{
    // Ensure the input buffer is valid
    if (Buff == NULL || Buff_Len == 0)
        return;

    uint32_t Prog_Loc = 0;

    // Read data until buffer length or flash page size is reached
    while (Prog_Loc < Buff_Len)
    {
        // Read a word from flash memory
        uint32_t flash_data = *(__IO uint32_t *)(Addr + Prog_Loc);

        // Check if flash data is empty
        if (flash_data == FLASH_EMPTY_VALUE)
            break;

        // Copy the flash data into the buffer
        *(uint32_t *)(Buff + Prog_Loc) = flash_data;

        // Increment the progress location by word size (4 bytes)
        Prog_Loc += sizeof(uint32_t);
    }
}


void FLASH_Erase(uint32_t FlashEraseAddress)
{
    FirstSector = GetSector(FlashEraseAddress);
    NbOfSectors = GetSector(FlashEraseAddress) - FirstSector + 1;
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
        Error_Handler();
}

void HAL_FLASH_Write_Buff(uint32_t FlashAddress, char *Data, uint32_t DataSize)
{
    for (uint32_t Prog_Loc = 0; Prog_Loc < DataSize; Prog_Loc += 4)
    {
        // Combine 4 bytes into a 32-bit word
        uint32_t Word =
            ((uint32_t)Data[Prog_Loc + 3] << 24) |
            ((uint32_t)Data[Prog_Loc + 2] << 16) |
            ((uint32_t)Data[Prog_Loc + 1] << 8) |
            (uint32_t)Data[Prog_Loc];

        // Program the 32-bit word into flash memory
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FlashAddress, Word);

        // Increment flash address by 4 bytes (word size)
        FlashAddress += 4;
    }
}

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_SECTOR_7;
    }
    else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_SECTOR_8;
    }
    else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_SECTOR_9;
    }
    else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_SECTOR_10;
    }
    else if ((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
    {
        sector = FLASH_SECTOR_11;
    }
    else if ((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
    {
        sector = FLASH_SECTOR_12;
    }
    else if ((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
    {
        sector = FLASH_SECTOR_13;
    }
    else if ((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
    {
        sector = FLASH_SECTOR_14;
    }
    else if ((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
    {
        sector = FLASH_SECTOR_15;
    }
    else if ((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
    {
        sector = FLASH_SECTOR_16;
    }
    else if ((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
    {
        sector = FLASH_SECTOR_17;
    }
    else if ((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
    {
        sector = FLASH_SECTOR_18;
    }
    else if ((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
    {
        sector = FLASH_SECTOR_19;
    }
    else if ((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
    {
        sector = FLASH_SECTOR_20;
    }
    else if ((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
    {
        sector = FLASH_SECTOR_21;
    }
    else if ((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
    {
        sector = FLASH_SECTOR_22;
    }
    else /*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
    {
        sector = FLASH_SECTOR_23;
    }

    return sector;
}

/*----------------------------------------------------------------------------------------------
* Function Name : GET_Config
* Description   : This function is used to get the configuration from the flash memory
* Input         : None
* Output        : None
* Return        : None
* -------------------------------------------------------------------------------------------*/

void GET_Config(void)
{
    uint32_t Address = FLASH_USER_START_ADDR;
    short FlashData = NULL;
    FlashData = *((uint8_t*)Address); Address += 16;

    if (FlashData == 0xED)
    {
        /* Read Slave Request Time */
        Default_Test_TimeOut = *((uint16_t*)Address); Address += 4;

        /* Slave Request Time */
        Default_SReq_Time    = *((uint16_t*)Address); Address +=4;
        
        /* Read Master ID */
        MASTER_ID            = *((uint8_t*)Address); Address += 4;

        /* Read Modbus or RFM */
        //finetune tbd santosh , take value from single var
        Flag_Modbus_Request  = *((FlagStatus*)Address); Address++;
        Flag_RFM_Request     = *((FlagStatus*)Address); Address++;

        /* Read Total Slave Count */
        MAX_SLAVE_DEVICES    = *((uint8_t*)Address); Address += 2;
        
        for (uint8_t i = 0; i < MAX_SLAVE_DEVICES; i++)
        {
            All_SlaveIDs[i][0] = *((uint8_t*)Address); Address++;
        }
    }
    else SET_Default_Config();
}


/*----------------------------------------------------------------------------------------------
* Function Name : SET_Config
* Description   : This function is used to set the configuration to the flash memory
* Input         : None
* Output        : None
* Return        : None
* -------------------------------------------------------------------------------------------*/
void SET_Config(void)
{
    uint32_t Address = FLASH_USER_START_ADDR;

    HAL_FLASH_Unlock();  // Unlock flash for writing

    /* Erase the flash sector */
    FLASH_Erase(Address);
    
    /*Write ED Marks for Cehck Flash Data*/
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, 0xED); Address += 16;
    
    /*Write Slave Req Time*/
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, Default_Test_TimeOut); Address += 4;

    /*Write Slave Req Time*/
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, Default_SReq_Time); Address += 4;
    
    /*Write Master ID*/
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, MASTER_ID); Address += 4;

    /*Write Modbus or RFM*/
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, Flag_Modbus_Request); Address++;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, Flag_RFM_Request); Address++;
   
   /*Write Total Slave Cnt*/
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, MAX_SLAVE_DEVICES); Address += 2;

   /*Write Slave IDs*/
   for (uint8_t i = 0; i < MAX_SLAVE_DEVICES; i++)
   {
       // Write each ID to flash (1 byte per ID)
       HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, All_SlaveIDs[i][0]); Address++;
   }

  HAL_FLASH_Lock();  // Lock flash after writing
  
  GET_Config();
}

/*----------------------------------------------------------------------------------------------
* Function Name : SET_Default_Config
* Description   : This function is used to set the default configuration to the flash memory
* Input         : None
* Output        : None
* Return        : None
* -------------------------------------------------------------------------------------------*/

void SET_Default_Config(void)
{
   /* Max Slave Device */
   MAX_SLAVE_DEVICES = 20;
   
   /* Slave Request Time*/
   Default_Test_TimeOut = 200;
   
   /* Slave Request Time*/
    Default_SReq_Time = 50;
   
   /* Master ID*/
    MASTER_ID = 1;

   /* Select MODBUS or RFM*/
    Flag_Modbus_Request = SET;
    Flag_RFM_Request    = RESET;

   /* 10 Device Connected default setting*/
    for (uint8_t i = 0; i < MAX_SLAVE_DEVICES; i++)
        All_SlaveIDs[i][0] = i+1;

  SET_Config();
}