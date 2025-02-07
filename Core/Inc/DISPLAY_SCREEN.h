#ifndef __DISPLAY_SCREEN_H
#define __DISPLAY_SCREEN_H

#define DEBOUNCE_THRESHOLD 1  // Adjust as needed
#define ROWS               4
#define COL                7

typedef enum 
{
   COMPANY_NAME_TOTAL_CH,     // "MSSL XCITE-CH96 V0.0"
   OPERATION_MODE,            // ":SEL.OPERATION MODE:"
   SELECT_OPERATION_MODE,     // "PRESS UP/DWN BTN"
   UP_DWN,                    // "PRESS UP/DWN TO CHANGE"
   PRESS_UP_SWITCH,           // "PRESS UP SWITCH"
   PRESS_DWN_SWITCH,          // "PRESS DWN SWITCH"
   PRESS_ENTER_SELECT,        // "PRESS ENTER TO SELECT"
   CLAMP_TEST_MODE,           // "->CLAMP TEST MODE"
   DEVICE_TEST_MODE,               // "<-NORMAL MODE->"
   CALIBRATION_MODE,          // "->CALIBRATION MODE"
   PANNEL_SWITCH_CHECK,       // ":PANEL SWITCH CHECK:"
   PANNEL_SWITCH_WORKING,     // "PANEL SWITCH WORKING"
   PANNEL_SWITCH_FAILED,      // "PANEL SWITCH FAILED"
   BUZZER_TEST,               // "BUZZER TEST MODE"
   OPEN_CLAMP,                // "OPEN CLAMP TEST"
   SHORT_CLAMP,               // "SHORT CLAMP TEST"
   MOUNT_CLAMP,               // "MOUNT CLAMP NO: 1"
   SINGLE_CLAMP,              // "SINGLE CLAMP TEST"
   STATUS_PASS,               // "STATUS: PASS"
   STATUS_FAIL,               // "STATUS: FAIL"
   SERIAL1_TEST,              // "SERIAL1 TEST MODE"
   LED_CHECK,                 // "a. LED CHECK"
   SCANNER_CHECK,             // "b. SCANNER CHECK"  
   ETH_CHECK,                 // "c. ETH CHECK"
   USB_CHECK,                 // "d. USB CHECK" 
   BUTTON_CHECK,              // "e. BUTTON CHECK" 
   MODBUS,                    // "f. MODBUS" 
   DIP,                       // "g. DIP " 
   RFM_CHECK,                 // "h. RFM CHECK" 
   EX_FLASH_CHECK,            // "i. EX-FLASH CHECK"  
   Config_MX_SID,             // "a. Config MX SID"  
   SLV_RQT_FREQ,              // "b. SLV RQT FREQ" 
   TEST_TIMEOUT,              // "c. TEST TIMEOUT"
   SLV_PTL_TYPE_MR,           // "d. SLV PTL TYPE M/R" 
   SET_TIMESTAMP,             // "e. SET TIMESTAMP"   
   SELECT_TESTING,            // "<----SELECT TESTING---->"
   WH_CODE,                   // "WH CODE :"
   LED_CHECKING,              // "a. LED CHECKING"
   ETH_CHECKING,              // "c. ETH CHECKING :-" 
   USB_CHECKING,              // "d. USB CHECKING :-"  
   RFM_CHECKING,              // "h. RFM CHECKING :-"   
   EX_FLASH_CHECKING,         // "i. EX-FLASH CHECKING :-"  
   BCK_PRSS,                  // "PRESS MENU TO BACK"
   REQT_FREQ,                 // "SETTING REQT FRWQ"
   TIME_FREQ,                 // "SETTING TIME FRWQ"
   LEDS_STATUS_PASS,          // "ALL_LEDS_STATUS: PASS"
   LEDS_STATUS_FAIL,          // "ALL_LEDS_STATUS: FAIL"
   UNKNOWN_SCREEN             // "UNKNOWN SCREEN"
} Screen_List_TypeDef;  

typedef enum 
{
   MAIN_SCREEN,
   SELECT_MODE,
   ClampTestMode,
   DeviceMode,
   CalibrationMode,
   openclamp
} Main_Screen_Disp_TypeDef;


void processRow(uint8_t row); 
void Display_SCREEN(Display_Screen_TypeDef Disply_Sreen);
void SELECT_SCREEN(Screen_List_TypeDef ScreenList, uint8_t X, uint8_t Y);
Main_Screen_Disp_TypeDef MAIN_DISPLAY_SCREEN(Main_Screen_Disp_TypeDef Main_Screen_Disp);
void KEY_FUNCTION(void);
uint8_t count_deduct(int value);
void Display_Main_Menu();
void Clamp_test_mode();


#endif // __DISPLAY_SCREEN_H