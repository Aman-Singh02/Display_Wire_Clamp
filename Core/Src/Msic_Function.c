#include "main.h"
#include "File_Handling.h"
#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"



/*******************************************************************************
 * Function Name  : Sec_Timer
 * Description    : Second timer function
 * Input          : None
 * Output         : None
 * Return         : None
 * *****************************************************************************/
extern ApplicationTypeDef Appli_state;

void Sec_Timer(void)
{
  Flag_Second = RESET;
  /* Relooad counter */
  LL_IWDG_ReloadCounter(IWDG);
  
  /* */
  /* Check for the key press */
   //MASTER_ID = Read_MASTER_ID();
   
  /* Data Log in Pendrive */
  if(!Flag_USB_Connected && !Flag_LCTU_TOUT) 
  {
      /* Show Failed ID */
    if(Flag_Test_Failed && (Show_Failed_Fre <= 1))
    {
      Show_Failed_Fre = 2;
        Scroll_Text_StateMachine(&ScrollState, 1); // Assume this function is called every 100ms
    }
    /* GET Backup with ETH */
    if(Flag_Backup_Data && BKP_Logs_Cnt)
    {
      __Logging(ETH_LOG);
    }
    else Flag_Backup_Data = RESET;
    
    /* Check the connected devices */
    if(Flag_Conn_Device)Display_SCREEN(CONN_DEVICE);

    /* Check the not connected devices */
    else if(Flag_Not_Conn_Device)Display_SCREEN(NOT_CONN_DEVICE);
  }
  else if(Flag_USB_Connected)
  {
    __Logging(PENDRIVE_LOG);
  }
  /* GET RTC DATE TIME */
  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  
  /* Convert Time To Epoch Time */
  //EpoachBytes  =  Time_To_Epoch(RTC_DateStruct, RTC_TimeStruct);
}

/*******************************************************************************
 * Function Name  : KEY_SCAN
 * Description    : Check for the key press
 * Input          : None
 * Output         : None
 * Return         : None
 * *****************************************************************************/

Key_TypeDef KEY_SCAN(void)
{
  static uint8_t debounce_up = 0;
  static uint8_t debounce_down = 0;
  static uint8_t debounce_enter = 0;
  static uint8_t debounce_menu = 0;
  
  if (!LL_GPIO_IsInputPinSet(SW1_GPIO_Port, SW1_Pin))
  {
    if (debounce_up < DEBOUNCE_THRESHOLD) debounce_up++;
    if (debounce_up >= DEBOUNCE_THRESHOLD){debounce_up = 0;return UP; } 
  }
  else if (!LL_GPIO_IsInputPinSet(SW2_GPIO_Port, SW2_Pin))
  {
    if (debounce_down < DEBOUNCE_THRESHOLD) debounce_down++;
    if (debounce_down >= DEBOUNCE_THRESHOLD){ debounce_down = 0;count_down++;return DOWN; } 
  }
  else if (!LL_GPIO_IsInputPinSet(SW3_GPIO_Port, SW3_Pin))
  {
    if (debounce_enter < DEBOUNCE_THRESHOLD) debounce_enter++;
    if (debounce_enter >= DEBOUNCE_THRESHOLD){debounce_enter = 0;return ENTER; } 
  }
  else if (!LL_GPIO_IsInputPinSet(SW4_GPIO_Port, SW4_Pin))
  {
    if (debounce_menu < DEBOUNCE_THRESHOLD) debounce_menu++;
    if (debounce_menu >= DEBOUNCE_THRESHOLD){debounce_menu = 0; return MENU;} 
  }
  else return NONE;
}

/*******************************************************************************
 * Function Name  : Read_MASTER_ID
 * Description    : Read the DIP switch status
 * Input          : None
 * Output         : None
 * Return         : None
 * *****************************************************************************/
uint8_t Read_MASTER_ID(void)
{
    uint8_t masterID = 0;

    masterID |= (LL_GPIO_IsInputPinSet(DIP_SW7_GPIO_Port, DIP_SW7_Pin) << 0); // Read DIP Switch 7
    masterID |= (LL_GPIO_IsInputPinSet(DIP_SW6_GPIO_Port, DIP_SW6_Pin) << 1); // Read DIP Switch 6
    masterID |= (LL_GPIO_IsInputPinSet(DIP_SW5_GPIO_Port, DIP_SW5_Pin) << 2); // Read DIP Switch 5
    masterID |= (LL_GPIO_IsInputPinSet(DIP_SW4_GPIO_Port, DIP_SW4_Pin) << 3); // Read DIP Switch 4
    masterID |= (LL_GPIO_IsInputPinSet(DIP_SW3_GPIO_Port, DIP_SW3_Pin) << 4); // Read DIP Switch 3
    masterID |= (LL_GPIO_IsInputPinSet(DIP_SW2_GPIO_Port, DIP_SW2_Pin) << 5); // Read DIP Switch 2
    masterID |= (LL_GPIO_IsInputPinSet(DIP_SW1_GPIO_Port, DIP_SW1_Pin) << 6); // Read DIP Switch 1
    masterID |= (LL_GPIO_IsInputPinSet(DIP_SW0_GPIO_Port, DIP_SW0_Pin) << 7); // Read DIP Switch 0

    return masterID;
}



/*******************************************************************************
 * Function Name  : GetBkp_Data
 * Description    : Get the backup data
 * Input          : None
 * Output         : None
 * Return         : None
 * *****************************************************************************/
void GetBkp_Data(void)
{
  Flag_RAM_BKP = RESET;

  /* Check the connected devices */
   if(Flag_with_All_SlaveID)
   {
      Conn_Slave_Cnt = 0;
      Not_Conn_Slave_Cnt = 0;
      for (int i = 0; i < MAX_SLAVE_DEVICES; i++) 
      {
        if (All_SlaveIDs[i][1] == 0x01) 
          Conn_SlaveId[Conn_Slave_Cnt++] = All_SlaveIDs[i][0]; // Add to connected list
        else 
          Not_Conn_SlaveId[Not_Conn_Slave_Cnt++] = All_SlaveIDs[i][0]; // Add to not connected list
      }
   }
   
   Display_SCREEN(CONN_DEVICES);
   
   if(Conn_Slave_Cnt < MAX_SLAVE_DEVICES)
   {
     sprintf(Result, "%s", "FAILED");
     Display_SCREEN(RESULT_FAILED);
     Display_SCREEN(SHOW_FAILED_ID);
     Flag_Test_Failed = SET;
   }
   else
   { 
    sprintf(Result, "%s", "PASS"); 
    Display_SCREEN(RESULT_PASS);
   }
    
    /* Data Logging */
    char Ptr[4] = {0};
   sprintf(Log_Buffer[Log_Cnt],
             "$%s,%02d/%02d/20%02d,%02d:%02d:%02d,%02d:%02d:%02d,%d,%s",
             Scanner_Buf, sDate.Date, sDate.Month, sDate.Year,
             Start_TimeStruct.Hours, Start_TimeStruct.Minutes, Start_TimeStruct.Seconds,
             sTime.Hours, sTime.Minutes, sTime.Seconds,
             Total_Test_Time, Result);
    
  
    if((strlen(Not_Conn_SlaveId) > 0 ) && (Conn_Slave_Cnt != MAX_SLAVE_DEVICES))
    {  
      for(uint8_t Add_NCSID = 0; Add_NCSID <= (strlen(Not_Conn_SlaveId)-1); Add_NCSID++)
      {  
        sprintf(Ptr,",%d", Not_Conn_SlaveId[Add_NCSID]);
        strcat(Log_Buffer[Log_Cnt], Ptr);
      }
    }
    
    /* Add the end of the log */
    strcat(Log_Buffer[Log_Cnt], "*\n");

    /* Send Data with ETH */
    if(Flag_TCP_Connect)send_data(Log_Buffer[Log_Cnt]);
    
    /* Clear MODBUS_RESP.HRegister buffer */
    memset(Conn_SlaveId, 0, sizeof(Conn_SlaveId));
    memset(Not_Conn_SlaveId, 0, sizeof(Not_Conn_SlaveId));
    memset(MODBUS_RESP.HRegister, 0, sizeof(MODBUS_RESP.HRegister));
    memset(Scanner_Buf,0, sizeof(Scanner_Buf));
    /* Reset the test time */
    Total_Test_Time = 0;
    
    /* Increment log count */
    Log_Cnt++;

  /* If Log_Cnt >= 20, save data and reset */
  if (Log_Cnt >= 20) 
  {
    /* Increment total log count  */
    Total_Log_Count += Log_Cnt; 

    /* Save the logs */
    ExFlash_LOG_BKP(EXFLASH_WRITE);

    /* Reset Log_Buffer without clearing the entire buffer */
    Log_Cnt = 0;
  }
}


// Function to calculate the number of digits in a number
int getNumberOfDigits(uint32_t num) {
    if (num == 0) return 1; // Special case for zero
    int digits = 0;
    while (num > 0) {
        digits++;
        num /= 10;
    }
    return digits;
}

// Function to calculate the width of text
int calculateTextWidth(int numDigits, int columnWidth) {
    return numDigits * columnWidth;
}

// void displayCenteredCount(uint32_t Conn_Slave_Cnt, uint32_t MAX_SLAVE_DEVICES, int columnWidth, int displayWidth) {
 
// Function to display numbers and center-align them on a fixed width
void displayDeviceCounts(uint32_t Conn_Slave_Cnt, int columnWidth, int displayWidth) 
{
    // Calculate number of digits for Conn_Slave_Cnt and MAX_SLAVE_DEVICES
    int connDigits = getNumberOfDigits(Conn_Slave_Cnt);
    int maxDigits = getNumberOfDigits(MAX_SLAVE_DEVICES);

    // Calculate total text width for both numbers and the separator
    int connWidth = calculateTextWidth(connDigits, columnWidth);
    int separatorWidth = columnWidth; // Assume separator '/' has the same width as a digit
    int maxWidth = calculateTextWidth(maxDigits, columnWidth);
    int totalWidth = connWidth + separatorWidth + maxWidth;

    // Calculate starting position for centering
    int startX = (displayWidth - totalWidth) / 2;

    // Display connected devices count
    for (int i = 0; i < connDigits; i++) {
        uint8_t digit = (Conn_Slave_Cnt / (int)pow(10, connDigits - 1 - i)) % 10; // Extract each digit
        WriteNum_2x18(3, startX + (i * columnWidth), digit + '0');                // Display digit
    }

    // Display the separator '/'
    WriteNum_2x18(3, startX + connWidth, (58)); // ASCII value for '/'

    // Display total devices count
    for (int i = 0; i < maxDigits; i++) {
        uint8_t digit = (MAX_SLAVE_DEVICES / (int)pow(10, maxDigits - 1 - i)) % 10; // Extract each digit
        WriteNum_2x18(3, startX + connWidth + separatorWidth + (i * columnWidth), digit + '0'); // Display digit
    }
}


/*******************************************************************************
 * Function Name  : __Logging
 * Description    : Display the main screen with company logo
 * Input          : None
 * Output         : None
 * Return         : None
 * *****************************************************************************/
void __Logging(Logging_TypeDef Logging)
{
  uint8_t Log_Present = 0;
  uint32_t Coped_Logs = 0;
  uint32_t Total_Logs = 0;
  char Temp_Buf[5] = {0};
  switch (Logging)  
  {
  case ETH_LOG:
    if((Log_Cnt == 0) && (BKP_Logs_Cnt > 0))
    {
      ExFlash_LOG_BKP(EXFLASH_READ);
      
      mSec_Delay(100);
      for (uint8_t i = 0; (i < 15) && (Log_Cnt > 0); i++)
      {
        if(Flag_TCP_Connect)send_data(Log_Buffer[0]);
        mSec_Delay(5);
        memmove(Log_Buffer[0],Log_Buffer[1],(19 * 256));             
        memset(Log_Buffer[19],0,256);
        if(Log_Cnt)Log_Cnt--;
      }
    }
    break;
  case PENDRIVE_LOG:
    if ((Log_Cnt == 0) && (BKP_Logs_Cnt > 0))
    {
      //uint8_t logs_to_process = BKP_Logs_Cnt;
       // Processing copy Log to Pendrive
        Clear_Area(0,0,127,63);
        LCDWrite_Text(5,20,"Log Copying to Pendrive ...");
        LCDWrite_Text(25, 40, "Logs Copied:      %");
        Total_Logs = BKP_Logs_Cnt;
        while (BKP_Logs_Cnt > 0) 
        {
          // Read backup logs from external flash
          ExFlash_LOG_BKP(EXFLASH_READ);
          
          /* Check Log count */
          Log_Present = CALCULATE_PERCENTAGE(Coped_Logs, Total_Logs);
          sprintf(Temp_Buf,"%u",Log_Present);
          Clear_Area(80, 38, 95, 50);
          LCDWrite_Text(85, 40, Temp_Buf);
          
          mSec_Delay(10);
          
          if (Flag_Second){Flag_Second = RESET; LL_IWDG_ReloadCounter(IWDG);}
          while (Log_Cnt > 0)
          {
            // Update Excel file with the first log
            __Data_Update_Excle(Log_File_Name, Log_Buffer[0]);
            
            // Shift remaining logs in the buffer
            memmove(Log_Buffer[0], Log_Buffer[1], 19 * 256); // Move logs up
            memset(Log_Buffer[19], 0, 256);                  // Clear last buffer
            Log_Cnt--;
            Coped_Logs++;
          }
        }
        
      Clear_Area(0,0,127,63);
      LCDWrite_Text(15,30,"Logs Copied Sucessfully");
      Flag_USB_Connected = RESET;
      Flag_LCTU_TOUT = SET;
     // Appli_state = APPLICATION_DISCONNECT;
    }
    else
    {
      Clear_Area(0,0,127,63);
      LCDWrite_Text(35,30,"No Logs Found");
      //Appli_state = APPLICATION_DISCONNECT;
      Flag_USB_Connected = RESET;
      Flag_LCTU_TOUT = SET;
    }
    break;
  }
}

/*******************************************************************************
 * Function Name  : Serial_PutString
 * Description    : Send a string to the serial port
 * Input          : str: string to be sent
 * Output         : None
 * Return         : None
 ******************************************************************************/
void Serial_PutString(char *str)
{
  MODBUS_Tx_EN;
  nSec_Delay(500);
  while (*str != '\0')
  {
    /* Wait for the transmit buffer to be empty */
    while (!LL_USART_IsActiveFlag_TXE(MODBUS_UART));
    LL_USART_TransmitData8(MODBUS_UART, *str);
    str++;
    nSec_Delay(20);
  }
   nSec_Delay(500);
  /* Wait for transmission to complete */
  MODBUS_Tx_DIS;
}


/*******************************************************************************
* Function Name  : mSec_Delay
* Description    : Insert Delay in mili Seconds
* Input          : mili seconds 
* Output         : None
* Return         : None
*******************************************************************************/
void mSec_Delay(unsigned int nTime)
{
 while(nTime != 0)                                                              // MILI SECOND DELAY 
  { 
    if(Flag_mSec == SET){Flag_mSec = RESET;nTime--;};                           // MILI SECOND FLAG IS SET BY SYSTICK TIMER 
  };
}

/*******************************************************************************
* Function Name  : nSec_Delay
* Description    : Insert Delay in mili Seconds
* Input          : mili seconds 
* Output         : None
* Return         : None
*******************************************************************************/
void nSec_Delay(unsigned int nTime)
{
  while(nTime)                                                              // MILI SECOND DELAY 
  { 
    __NOP();
   nTime--;                           // MILI SECOND FLAG IS SET BY SYSTICK TIMER 
  };
}