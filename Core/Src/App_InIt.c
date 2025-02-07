#include "main.h"

uint8_t CH_No = 0x01;

void App_Init(void)
{
  /*------------ LED STATUS Check -------------*/
  LED_PWR_ON;
  LED_SYNC_ON;
  LED_CON_ON;
  
  /*TCP INIT */
  TCP_server_init();

  /*---------- CHECK SPI FLASH MEMORY ------------------------------------------*/
  uint32_t FlashAddress = SPI_FLASH_ReadID();                                            
  Flag_SFLASHOK = (FlashAddress == 0xEF4018 || FlashAddress == 0xEF4017 || FlashAddress == 0x852018) ? SET : RESET; // GET FLASH STATUS WORKING 
  
  /* Check Backup Logs */
  FlashAddress = SPIFLASH_CONFIG; 
  char ConfigPtr[20] = {0};
  SPI_FLASH_BufferRead(ConfigPtr, FlashAddress, 20);

  char *Ptr = strstr(ConfigPtr, "Backup1-");
  if (Ptr != NULL) 
  {
    Ptr += 8; BKP_Logs_Cnt = atoi(Ptr);
  }
  
    /* Check Master ID */
  MASTER_ID    = Read_MASTER_ID();
  
  /* Get Config*/
  GET_Config();
  
  /* Initialize the LCD */
  LCD_ON;
  INIT_LCD();
  DISPLAY_ON;
  mSec_Delay(2000);
  /* LCD INIT FUNCTION */
  Clear_Area(0,0,127,63);
  
  Flag_MainScreen = SET;
//  Flag_Display_Update     = SET;
//  Flag_Scanner_Dis_Update = SET;
//  Flag_with_All_SlaveID   = SET;
  
  if(Flag_RFM_Request)
  {
    RFM75_INIT(RFM_Mode_Rx);
    Set_Channel_No(0x03); // Select channel
    char Status = RFM_Read_Reg(STATUS);
  }
}