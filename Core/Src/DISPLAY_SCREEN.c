#include "main.h"

Main_Screen_Disp_TypeDef Main_Screen_Disp;
Key_TypeDef Key_Pressed;
uint8_t test_count = 0;

bool deviceModeFlags[6] = {0};
uint8_t count,  Check_enter;    //Check_up, Check_down,, Check_menu
uint8_t count_down = 0;
//uint8_t WH[ROWS][COL] = {   // 2D array with 4 rows and 5 columns
//    {5},
//    {10},
//    {15},
//    {20}
//};

char WH[ROWS][COL] = {   // 4 rows (for 4 names), 7 columns (including null terminator)
    "AMAN 1",
    "AMAN 2",
    "AMAN 3",
    "AMAN 4"
};

//int8_t WH[3] = {10, 20, 30};   // Initialize the WH buffer
uint8_t currentIndex = 0;       // Keep track of the current index
char buffer[10];
uint8_t loopVariable = 0;
uint8_t count_clamp = 0;
uint8_t reqt = 50;
FlagStatus Flag_set;

uint8_t debounceCounter = 0;  // Counter for debounce


/*******************************************************************************
 * Function Name  : Display_SCREEN
 * Description    : Display the data on the LCD
 * Input          : LCD_OP_TypeDef
 * Output         : None
 * Return         : None
 * *****************************************************************************/

void Display_SCREEN(Display_Screen_TypeDef Disply_Sreen)
{
  char Local_Scan_Buf[100] = {0};
  uint8_t Page = 0, Column = 0;
  uint8_t Conn_Devices = 0;
   
  for (int i = 0; i < MAX_SLAVE_DEVICES; i++) 
  {
    if (All_SlaveIDs[i][1] == 1) Conn_Devices++;
  }
      
  switch (Disply_Sreen)
  {
    /* Display the connected devices list */
    case CONN_DEVICE:
      Clear_Area(1,1,126,10);
      LCDWrite_Text(15,5,"CONNECTED DEVICES");
      Clear_Area(1,26,126,40);
      Page = 5; Column = 38;
      
      /* Loop through the connected slave devices and display their IDs */
      for (size_t i = 0; i < strlen(Conn_SlaveId); i++)
      {
        LCDWrite_Text(Page, Column, &Conn_SlaveId[i]);
        if (Column < 128) Column += 10; else Column = 38;
        if (Page < 8) Page++; else Page = 5;
      }
      break;
    
    /* Display the not connected devices list */
    case NOT_CONN_DEVICE:
      Clear_Area(1,1,126,50);
      LCDWrite_Text(6,5,"NOT CONNECTED DEVICES");
      
      /* Loop through the slave devices and display their IDs */
      for (uint8_t i = 0; i < strlen(Not_Conn_SlaveId); i++)
      {
        LCDWrite_Text(Page, Column, &Not_Conn_SlaveId[i]);
        if (Column < 128) Column += 10; else Column = 38;
        if (Page < 8) Page++; else Page = 5;
      }
      break;
     
    case FITTED_CLAMPS:
      Clear_Area(0,0,128,23);
      LCDWrite_Text(32,2,"FITTED CLAMPS");
      sprintf(Local_Scan_Buf,"WH CODE: %s",Scanner_Buf); //6996785261733
      LCDWrite_Text(0,15,Local_Scan_Buf);
      break;

    /* Display the total number of connected devices */
    case CONN_DEVICES:
      
      // Clear the display area for updating connected device count
      Clear_Area(0, 24, 128, 45);
        for (int i = 0; i < 3; i++) {
            uint8_t digit = (Conn_Devices / (int)pow(10, 2 - i)) % 10;          // Extract each digit
            WriteNum_2x18(3, 13 + (i * 15), digit + '0');                       // Display digit with spacing of 12 pixels
        }

        // Display the separator '/'
        WriteNum_2x18(3, (18 + (3 * 15)), (58));

        // Display total device count
        for (int i = 0; i < 3; i++) {
            uint8_t digit = (MAX_SLAVE_DEVICES / (int)pow(10, 2 - i)) % 10;     // Extract each digit
            WriteNum_2x18(3, 33 + (4 * 12) + (i * 15), digit + '0');            // Display digit with proper spacing
        }
      break;
      
    case TOTAL_CONFIGURED_DEVICCES:  // Display total device count
        for (int i = 0; i < 3; i++) 
        {
          uint8_t digit = (MAX_SLAVE_DEVICES / (int)pow(10, 2 - i)) % 10;       // Extract each digit
          WriteNum_2x18(3, 33 + (4 * 12) + (i * 15), digit + '0');              // Display digit with proper spacing
        }
      break;
    case TEST_STARTED:
      Clear_Area(1, 52, 128, 63);
      LCDWrite_Text(30,55,"TEST STARTED");
      break;
    case TEST_PROGRESS:
      Clear_Area(1, 52, 128, 63);
      LCDWrite_Text(20,55,"TEST IN PROGRESS");
      break;
    case RESULT_PASS:
      Clear_Area(1, 52, 128, 63);
      LCDWrite_Text(35,55,"RESULT : PASS");
      break;
  case RESULT_FAILED:
    Clear_Area(1, 52, 128, 63);
    LCDWrite_Text(2,55,"RESULT:FAIL-");
    break;
  case SHOW_FAILED_ID:
    
      // Initialize Show_FailedId_Buff to an empty string
      memset(Show_FailedId_Buff,0,sizeof(Show_FailedId_Buff));
      
      Conn_Slave_Cnt = 0;
      Not_Conn_Slave_Cnt = 0;
      for (int i = 0; i < MAX_SLAVE_DEVICES; i++) 
      {
        if (All_SlaveIDs[i][1] != 1) 
          Not_Conn_SlaveId[Not_Conn_Slave_Cnt++] = All_SlaveIDs[i][0]; // Add to not connected list
      }
      // Loop through the Not_Conn_SlaveId array and append each ID to Show_FailedId_Buff
      for (uint8_t i = 0; i < strlen(Not_Conn_SlaveId); i++) {
        char temp[10]; // Temporary buffer for each ID
        snprintf(temp, sizeof(temp), "%d,", Not_Conn_SlaveId[i]); // Format the ID
        strcat(Show_FailedId_Buff, temp); // Append the formatted ID to Show_FailedId_Buff
      }
      
      // Null-terminate Show_FailedId_Buff explicitly (redundant if snprintf/strcat are used correctly)
      Show_FailedId_Buff[strlen(Show_FailedId_Buff) -1] = '\0';
      
      
      // Call Scroll_Text_RightToLeft with the prepared Show_FailedId_Buff
      Initialize_ScrollState(&ScrollState, Show_FailedId_Buff, 65, 55, 128, 20, 5);
      //memset(Not_Conn_SlaveId,0, sizeof(Not_Conn_SlaveId));
    break;
    
    /* Display USB connectivity check message and its status */
    case CHECK_USB_CONNECTIVITY:
      LCDWrite_Text(5,10,"CHECK USB CONNECTIVITY");
      LCDWrite_Text(10, 20, (Flag_USBOK == SET) ? "USB CONNECTED" : "USB NOT CONNECTED");
      break;

    /* Display Modbus connectivity check message and its status */
    case CHECK_MODBUS_CONNECTIVITY:
      LCDWrite_Text(5,10,"CHECK MODBUS CONNECTIVITY");
      LCDWrite_Text(10, 20, (Flag_ModbusOK == SET) ? "MODBUS CONNECTED" : "MODBUS NOT CONNECTED");
      break;

    /* Display RF connectivity check message and its status */
    case CHECK_RF_CONNECTIVITY:
      LCDWrite_Text(5,10,"CHECK RF CONNECTIVITY");
      LCDWrite_Text(10, 20, (Flag_RF_OK == SET) ? "RF CONNECTED" : "RF NOT CONNECTED");
      break;

    /* Display Ethernet connectivity check message and its status */
    case CHECK_ETH_CONNECTIVITY:
      LCDWrite_Text(5,10,"CHECK ETH CONNECTIVITY");
      LCDWrite_Text(10, 20, (Flag_ETH_OK == SET) ? "ETH CONNECTED" : "ETH NOT CONNECTED");
      break;

    /* Display the log count message */
    case DISPLAY_LOG_COUNT:
      LCDWrite_Text(5,10,"DISPLAY LOG COUNT");
      LCDWrite_Text(10, 20, "LOG COUNT : ");
     // LCDWrite_Text(10, 30, (char *)(Total_Log_Count / 1000));
      break;

    /* Display hard fault message */
    case DISPLAY_HARDFAULT:
      LCDWrite_Text(10,10,"DEVICE GOTO HARDFAULT !");
      break;

    default:
      break;
  }
}


/*******************************************************************************
 * Function Name  : Display_SCREEN
 * Input          : ScreenList_TypeDef
 * Output         : None
 * Return         : None
 * Description    : Display screen 
 * *****************************************************************************/
void SELECT_SCREEN(Screen_List_TypeDef ScreenList, uint8_t X, uint8_t Y) 
{
    // Define the messages for each screen
    const char *screenMessages[] = 
    {
        "MSSL XCITE-CH96 V0.0",                          // COMPANY_NAME_TOTAL_CH
        "SELECT OPERATION MODE",                         // OPERATION_MODE
        "PRESS UP/DWN BTN",                              // SELECT_OPERATION_MODE
        "PRESS UP/DWN TO CHANGE",                        // UP_DWN
        "PRESS UP SWITCH",                               // PRESS_UP_SWITCH
        "PRESS DWN SWITCH",                              // PRESS_DWN_SWITCH
        "PRESS ENTER TO SELECT",                         // PRESS_ENTER_SELECT
        "1) CLAMP TEST MODE",                            // CLAMP_TEST_MODE
        "2) DEVICE TEST MODE",                           //DEVICE_MODE
        "3) CALIBRATION MODE",                           // CALIBRATION_MODE
        ":PANEL SWITCH CHECK:",                          // PANNEL_SWITCH_CHECK
        "PANEL SWITCH WORKING",                          // PANNEL_SWITCH_WORKING
        "PANEL SWITCH FAILED",                           // PANNEL_SWITCH_FAILED
        "BUZZER TEST MODE",                              // BUZZER_TEST
    	"-> OPEN CLAMP TEST",                            // OPEN_CLAMP
    	"-> SHORT CLAMP TEST",                           // SHORT_CLAMP
        "MOUNT CLAMP NO: 1",                             // MOUNT_CLAMP
        "SINGLE CLAMP TEST",                            // SINGLE_CLAMP
        "STATUS: PASS",                                 // STATUS_PASS
        "STATUS: FAIL",                                 // STATUS_FAIL
        "SERIAL1 TEST MODE",                            // SERIAL1_TEST
	"a. LED CHECK",                                 // LED_CHECK                         
        "b. SCANNER CHECK",                             // SCANNER_CHECK
        "c. ETH CHECK",                                 // ETH_CHECK
        "d. USB CHECK",                                 // USB_CHECK
        "e. BUTTON CHECK",                              // BUTTON_CHECK
        "f. MODBUS",                                    // MODBUS
        "g. DIP ",                                      // DIP
        "h. RFM CHECK",                                 // RFM_CHECK
        "i. EX-FLASH CHECK",                            // EX_FLASH_CHECK
        "a. Config MX SID",                             // Config_MX_SID 
        "b. SLV RQT FREQ",                              // SLV_RQT_FREQ
        "c. TEST TIMEOUT",                              // TEST_TIMEOUT
        "d. SLV PTL TYPE M/R",                          // SLV_PTL_TYPE_MR 
        "e. SET TIMESTAMP",                             // SET_TIMESTAMP 
        "<----SELECT TESTING---->",                     // SELECT_TESTING
        "WH CODE :",                                    // WH_CODE
        "a. LED CHECKING :-",                           // LED_CHECKING   
        "c. ETH CHECKING :-",                           // ETH_CHECKING
        "d. USB CHECKING :-",                           // USB_CHECKING
        "h. RFM CHECKING :-",                           // RFM_CHECKING
        "i. EX-FLASH CHECKING :-",                      // EX_FLASH_CHECKING
        "PRESS MENU TO BACK",                           // BCK_PRSS
        "b. SETTING REQT FRWQ",                         // REQT_FREQ
        "c. SETTING TIME FRWQ",                         // TIME_FREQ
        "LEDS STATUS: PASS",                            //LEDS_STATUS_PASS
        "LEDS STATUS: FAIL",                            //LEDS_STATUS_FAIL
        "UNKNOWN SCREEN"                                // UNKNOWN_SCREEN
    };
    
    // Ensure the index is valid and write the corresponding message
    if (ScreenList < (sizeof(screenMessages) / sizeof(screenMessages[0]))) 
    {  Clear_Area(4, (Y-2), 123, (Y+7)); LCDWrite_Text(X, Y, (char *)screenMessages[ScreenList]);}
    else 
    {  Clear_Area(4, (Y-2), 123, (Y+7)); LCDWrite_Text(X, Y, (char *)screenMessages[UNKNOWN_SCREEN]); }
}


/*******************************************************************************
 * Function Name  : MAIN_DISPLAY_SCREEN
 * Description    : Display the main screen
 * Input          : Main_Screen_Disp_TypeDef
 * Output         : None
 * Return         : None
 * *****************************************************************************/

Main_Screen_Disp_TypeDef MAIN_DISPLAY_SCREEN(Main_Screen_Disp_TypeDef Main_Screen_Disp)
{
   switch (Main_Screen_Disp)
    {
        
      case MAIN_SCREEN:
        Clear_Area(1, 1, 126,63); 
        SELECT_SCREEN(COMPANY_NAME_TOTAL_CH, 10, 5);                            // Display company name and model information
        SELECT_SCREEN(OPERATION_MODE, 6, 28);                                   // Display "SEL.OPERATION MODE" message
        SELECT_SCREEN(SELECT_OPERATION_MODE, 18, 52);                           // Display "PRESS UP/DWN BTN" message
        if((Key_Pressed == UP) || (Key_Pressed == DOWN)){ Flag_MainScreen = SET; Main_Screen_Disp = SELECT_MODE;}  
        else {Flag_set = RESET;Key_Pressed = NONE;}
        break;
        
      case SELECT_MODE:
        Clear_Area(1, 1, 126,60); 
        SELECT_SCREEN(OPERATION_MODE, 3, 5);                                    // Display company name and model information 
        SELECT_SCREEN(CLAMP_TEST_MODE, 4, 22);
        SELECT_SCREEN(DEVICE_TEST_MODE, 4, 38);
        SELECT_SCREEN(UP_DWN, 4, 58);                                           // Display "PRESS UP/DWN BTN" message
        
        if(Key_Pressed == UP){count_deduct(count_down);};
        
        if (Key_Pressed == ENTER)
        {
          Flag_MainScreen = SET; 
          if(count_down == 1){Main_Screen_Disp = ClampTestMode;count_down = 0;Flag_set = RESET;}
          if(count_down == 2){Main_Screen_Disp = DeviceMode;count_down = 1;Flag_set = RESET;}
          if(count_down == 3){Main_Screen_Disp = CalibrationMode;count_down = 1;Flag_set = RESET;}
         }
        
        if(Key_Pressed == DOWN || Key_Pressed == UP)
        {
          if(count_down == 1)
          {   
            Draw_Box_With_Gap(1, 16, 126, 16, 0, 1); 
            Clear_Inside_Area(1, 16, 126, 16, 0, 1);
            SELECT_SCREEN(CLAMP_TEST_MODE, 4, 22);
            SELECT_SCREEN(DEVICE_TEST_MODE, 4, 38);
          }
         if(count_down == 2)
          {  
            Clear_Area(1, 16, 126, 49);
            SELECT_SCREEN(CLAMP_TEST_MODE, 4, 22);
            Draw_Box_With_Gap(1, 31, 126, 17, 0, 1); 
            Clear_Inside_Area(1, 31, 126, 17, 0, 1);
            SELECT_SCREEN(DEVICE_TEST_MODE, 4, 38); 
          }
         if(count_down == 3)
          {
            Clear_Area(1, 16, 126, 49);
            SELECT_SCREEN(DEVICE_TEST_MODE, 4, 22);
            Draw_Box_With_Gap(1, 31, 126, 17, 0, 1); 
            Clear_Inside_Area(1, 31, 126, 17, 0, 1);
            SELECT_SCREEN(CALIBRATION_MODE, 4, 38);
          }
          
        }
        if(count_down > 3) count_down = 0;
        if(Key_Pressed == MENU) {Clear_Area(0,0,127,63); Flag_MainScreen = SET; Main_Screen_Disp = MAIN_SCREEN; Flag_set = RESET;}
       // Key_Pressed = NONE;
        break;
      
      case ClampTestMode:
        /* If the screen has not been updated, initialize the screen */
        if(Flag_set == RESET)
        {
          Clear_Area(1, 1, 126, 63);                                            // Clear the top portion of the screen
          SELECT_SCREEN(CLAMP_TEST_MODE, 3, 5);                                 // Show Clamp Test Mode screen
          SELECT_SCREEN(SELECT_OPERATION_MODE, 18, 58);                         // Show operation mode selection option
          // Key_Pressed = NONE;
          Flag_set = SET;
        }
        
        if(Key_Pressed == ENTER)
        {
          if(count_down >= 1 && count_down <= 4) // Ensure valid range
          {
            Flag_RecScanner = SET;
          }
        }
        
        // If the down button is pressed and count_down is less than ROWS (4)
        if (Key_Pressed == DOWN && count_down < 4) count_down++;
        
        // If the up button is pressed and count_down is greater than 1
        if (Key_Pressed == UP && count_down > 1) count_down--;
        
        /* Clear any previous text and display the current warehouse name on the screen */
        Clear_Area(69, 14, 110, 22);
        LCDWrite_Text(19, 17, "WH CODE :");
        
        // Convert count_down to a zero-based index
        uint8_t currentIndex = count_down - 1; 
        
        // Display the selected warehouse name
        LCDWrite_Text(70, 17,WH[currentIndex]);  
        
         processRow(currentIndex);
        
        // Display main screen if MENU is pressed
        if(Key_Pressed == MENU) 
        { 
          Clear_Area(0, 0, 127, 63); 
          Flag_MainScreen = SET; 
          Main_Screen_Disp = MAIN_SCREEN; 
        }
        Key_Pressed = NONE;
        break;
          
      case DeviceMode:
        /* If the screen has not been updated, update it now */
          if(Flag_set == RESET)
          {
            Clear_Area(1, 1, 126, 63);                                          // Clear a section of the screen
            SELECT_SCREEN(DEVICE_TEST_MODE, 3, 5);                              // Show the device mode screen
            SELECT_SCREEN(UP_DWN, 3, 58);                                       // Show the Up/Down button screen
            SELECT_SCREEN(LED_CHECK, 4, 22);                                    // Show LED check screen
            SELECT_SCREEN(SCANNER_CHECK, 4, 38);                                // Show Scanner check screen
            Flag_set = SET;
            Key_Pressed = NONE;
          }

        /* Check if the down button has been pressed */
        if (Key_Pressed == DOWN || Key_Pressed == ENTER || Key_Pressed == UP) 
        {
            /* If this mode hasn't been selected before */
              Clear_Area(1, 1, 126, 63);                                        // Clear the display
              SELECT_SCREEN(DEVICE_TEST_MODE, 3, 5);                            // Display "Calibration Mode" title
              SELECT_SCREEN(UP_DWN, 3, 58);                                     // Display navigation instructions

              if (Key_Pressed == DOWN || !deviceModeFlags[count_down - 1] || Key_Pressed == UP) 
              {     
                uint8_t odd = (count_down % 2 == 1);                            // Check if the index is odd or even
                Clear_Area(1, 16, 126, odd ? 47 : 32);                          // Adjust area clearing based on index
                if (!odd) Clear_Area(1, 35, 126, 45);                           // Clear additional area if even
                
                /* Highlight the selected option */
                Draw_Box_With_Gap(1, odd ? 16 : 31, 126, odd ? 16 : 17, 0, 1);
                Clear_Inside_Area(1, odd ? 16 : 31, 126, odd ? 16 : 17, 0, 1);
                
                /* Switch through the modes based on the value of count_down*/
                switch (count_down)
                {
                case 1:
                  SELECT_SCREEN(LED_CHECK, 4, 22);
                  SELECT_SCREEN(SCANNER_CHECK, 4, 38);
                  break;
                  
                case 2:
                  SELECT_SCREEN(LED_CHECK, 4, 22);
                  SELECT_SCREEN(SCANNER_CHECK, 4, 38);
                  break;
                  
                case 3:
                  SELECT_SCREEN(ETH_CHECK, 4, 22);
                  SELECT_SCREEN(USB_CHECK, 4, 38);
                  break;
                  
                case 4:
                  SELECT_SCREEN(ETH_CHECK, 4, 22);
                  SELECT_SCREEN(USB_CHECK, 4, 38);
                  break;
                  
                case 5:
                  SELECT_SCREEN(BUTTON_CHECK, 4, 22);
                  SELECT_SCREEN(MODBUS, 4, 38);
                  break;
                  
                case 6:
                  SELECT_SCREEN(BUTTON_CHECK, 4, 22);
                  SELECT_SCREEN(MODBUS, 4, 38);
                  break;
                  
                case 7:
                  SELECT_SCREEN(DIP, 4, 22);
                  SELECT_SCREEN(RFM_CHECK, 4, 38);
                  break;
                  
                case 8:
                  SELECT_SCREEN(DIP, 4, 22);
                  SELECT_SCREEN(RFM_CHECK, 4, 38);
                  break;
                  
                case 9:
                  SELECT_SCREEN(EX_FLASH_CHECK, 4, 22);
                  break;
                }
                
              }
        }
              if(Key_Pressed == UP){count_deduct(count_down);};
              if (Key_Pressed == ENTER)
                {
                  Flag_MainScreen = SET; 
                  Clear_Area(1, 0, 126, 63);
                  SELECT_SCREEN(BCK_PRSS, 13, 58);
                  switch(count_down)
                  {
                  case 1:
                    SELECT_SCREEN(LED_CHECKING, 3, 5);
                    SELECT_SCREEN(Flag_pass == SET ? LEDS_STATUS_PASS : LEDS_STATUS_FAIL, 18, 31);
                    break;
                    
                  case 2:
                    SELECT_SCREEN(SCANNER_CHECK, 3, 5);
                    SELECT_SCREEN(Flag_pass == SET ? STATUS_PASS : STATUS_FAIL, 29, 31);
                    break;
                    
                  case 3:
                    SELECT_SCREEN(ETH_CHECKING, 3, 5);
                    SELECT_SCREEN(Flag_pass == SET ? STATUS_PASS : STATUS_FAIL, 29, 31);
                    break;
                    
                  case 4:
                    SELECT_SCREEN(USB_CHECKING, 3, 5);
                    SELECT_SCREEN(Flag_pass == SET ? STATUS_PASS : STATUS_FAIL, 29, 31);
                    break;
                    
                  case 5:
                    if (test_count == 1) test_count = 0; 
                    SELECT_SCREEN(Flag_pass == SET ? STATUS_PASS : STATUS_FAIL, 29, 31);
                    break;
                    
                  case 6:
                    Clear_Area(1, 1, 126, 49);
                    SELECT_SCREEN(SELECT_TESTING, 1, 3);
                    SELECT_SCREEN(PRESS_ENTER_SELECT, 6, 55);
                    SELECT_SCREEN(OPEN_CLAMP, 4, 22);
                    SELECT_SCREEN(SHORT_CLAMP, 4, 38);
                    if (Key_Pressed == DOWN || Key_Pressed == UP) 
                    {
                      Draw_Box_With_Gap(1, count == 0 ? 16 : 31, 126, count == 0 ? 16 : 17, 0, 1);
                      Clear_Inside_Area(1, count == 0 ? 16 : 31, 126, count == 0 ? 16 : 17, 0, 1);
                      SELECT_SCREEN(OPEN_CLAMP, 4, 22);
                      SELECT_SCREEN(SHORT_CLAMP, 4, 38);
                      if (Key_Pressed == ENTER)
                      {
                        Clear_Area(1, 1, 126, 48);
                        SELECT_SCREEN(count == 0 ? OPEN_CLAMP : SHORT_CLAMP, 3, 5);
                        SELECT_SCREEN(Flag_pass == SET ? STATUS_PASS : STATUS_FAIL, 29, 28);
                      }
                    break;
                    
                  case 7:
                    if (test_count == 1) test_count = 0; 
                    break;
                    
                  case 8:
                    SELECT_SCREEN(RFM_CHECKING, 3, 5);
                    SELECT_SCREEN(Flag_pass == SET ? STATUS_PASS : STATUS_FAIL, 29, 31);
                    break;
                    
                  case 9:
                    SELECT_SCREEN(EX_FLASH_CHECKING, 3, 5);
                    SELECT_SCREEN(Flag_pass == SET ? STATUS_PASS : STATUS_FAIL, 29, 31);
                    break;
                    count_down = 0;
                  }
                }
              }
             if (count_down > 9){count_down = 0;}
                  
            /* If MENU button is pressed, exit device mode*/
            if (Key_Pressed == MENU) 
            {
                Flag_MainScreen = SET;
                Main_Screen_Disp = MAIN_SCREEN;                                 // Exit the device mode screen
                Flag_set = RESET;                                               // Reset initialization flag
            }
        break;
        
      case CalibrationMode:
              // Check if the screen needs to be updated
              if(Flag_set == RESET)
              {
                Clear_Area(1, 1, 126, 62);                                      // Clear the display
                SELECT_SCREEN(CALIBRATION_MODE, 3, 5);                          // Display "Calibration Mode" title
                SELECT_SCREEN(UP_DWN, 2, 58);                                   // Display navigation instructions
                Flag_set = SET;
                Key_Pressed = NONE;                                             //Added for sometime
              }
              
            // Handle DOWN key press if the current option is not already selected
            if (Key_Pressed == DOWN || !deviceModeFlags[count_down - 1] || Key_Pressed == UP) 
            {     
              uint8_t odd = (count_down % 2 == 1);                              // Check if the index is odd or even
              Clear_Area(1, 16, 126, odd ? 47 : 32);                            // Adjust area clearing based on index
              
              /* Highlight the selected option */
              Draw_Box_With_Gap(1, odd ? 16 : 31, 126, odd ? 16 : 17, 0, 1);
              Clear_Inside_Area(1, odd ? 16 : 31, 126, odd ? 16 : 17, 0, 1);
              
              /* Display corresponding options based on the selected index */   
              switch (count_down) 
              {
              case 1:
                SELECT_SCREEN(Config_MX_SID, 4, 22);
                SELECT_SCREEN(SLV_RQT_FREQ, 4, 38);
                break;
                
              case 2:
                SELECT_SCREEN(Config_MX_SID, 4, 22);
                SELECT_SCREEN(SLV_RQT_FREQ, 4, 38);
                
                if (Key_Pressed == ENTER)                                       // Check if enter flag is set
                { 
                  Clear_Area(1, 1, 126, 63);
                  SELECT_SCREEN(REQT_FREQ, 3, 5);
                  SELECT_SCREEN(UP_DWN, 10, 52); 
                  if (Key_Pressed == UP  && reqt < 100) 
                  {reqt++; }
                  else if (Key_Pressed == DOWN && reqt > 50) 
                  {reqt--;} 
                }
                break;
                
              case 3:
                SELECT_SCREEN(TEST_TIMEOUT, 4, 22);
                SELECT_SCREEN(SLV_PTL_TYPE_MR, 4, 38);
                 if (Key_Pressed == ENTER )                                     // Check if enter flag is set
                { 
                  Clear_Area(1, 1, 126, 63);
                  SELECT_SCREEN(TIME_FREQ, 3, 5);
                  SELECT_SCREEN(UP_DWN, 10, 52); 
                  if (Key_Pressed == UP  && reqt < 100) 
                  {reqt++; }
                  else if (Key_Pressed == DOWN && reqt > 50) 
                  {reqt--;} 
                }
                break;
                
              case 4:
                SELECT_SCREEN(TEST_TIMEOUT, 4, 22);
                SELECT_SCREEN(SLV_PTL_TYPE_MR, 4, 38);
                break;
                
              case 5:
                SELECT_SCREEN(SET_TIMESTAMP, 4, 22);
                break;
              }
            }
            if(Key_Pressed == UP){count_deduct(count_down);};
            if(count_down > 5){count_down = 0;} 
            
            /* Handle MENU key press to exit Calibration Mode */
            if (Key_Pressed == MENU)
            {
              Flag_MainScreen = SET;
              Main_Screen_Disp = MAIN_SCREEN;  
            }
        
        break;
        }
  return Main_Screen_Disp;
}
    

/*******************************************************************************
 * Function Name  : KEY_FUNCTION
 * Description    : Key function
 * Input          : None
 * Output         : None
 * Return         : None
 * *****************************************************************************/
void KEY_FUNCTION(void)
{
  Key_Pressed = KEY_SCAN();
  if(Flag_MainScreen || (Key_Pressed != NONE)) { Flag_MainScreen = RESET; Main_Screen_Disp = MAIN_DISPLAY_SCREEN(Main_Screen_Disp); Key_Pressed = NONE;}
}



/*******************************************************************************
 * Function Name  : processRow
 * Description    : it select row
 * Input          : None
 * Output         : None
 * Return         : None
 * *****************************************************************************/
void processRow(uint8_t row) 
{
  
    // Display selected row's value
    //snprintf(buffer, sizeof(buffer), "%d", WH[ROWS][0]);
    //LCDWrite_Text(58, 28, buffer);
    if(count_down > 4){count_down = 0;}
 
}

uint8_t count_deduct(int value)
{
    if (count_down > 1) {
        count_down--;  // Decrease the value bee
    } 
   Key_Pressed = UP;
   return count_down;
}

