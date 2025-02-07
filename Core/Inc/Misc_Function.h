#ifndef __MISC_FUNCTION_H
#define __MISC_FUNCTION_H

#define CALCULATE_PERCENTAGE(BKP_Logs_Cnt, Total_Logs) \
    ((Total_Logs == 0) ? 0 : ((uint8_t)((BKP_Logs_Cnt * 100) / Total_Logs)))
typedef enum
{
    CONN_DEVICE,               // Connected device screen
    NOT_CONN_DEVICE,           // Not connected device screen
    FITTED_CLAMPS,             // Fitted clamps screen
    CONN_DEVICES,              // Connected devices screen
    TOTAL_CONFIGURED_DEVICCES,  // Total configured devices screen
    TEST_STARTED,              // Test started screen
    TEST_PROGRESS,             // Test in progress screen
    RESULT_PASS,               // Test result pass screen
    RESULT_FAILED,               // Test result fail screen
    SHOW_FAILED_ID,            // Show failed ID screen
    CHECK_USB_CONNECTIVITY,   // USB connectivity check screen
    CHECK_MODBUS_CONNECTIVITY, // Modbus connectivity check screen
    CHECK_RF_CONNECTIVITY,    // RF connectivity check screen
    CHECK_ETH_CONNECTIVITY,   // Ethernet connectivity check screen
    DISPLAY_LOG_COUNT,         // Display log count screen
    DISPLAY_HARDFAULT         // Display hardfault screen
} Display_Screen_TypeDef;

/* Data Logging Type ENUM */
typedef enum 
{
  ETH_LOG,
  PENDRIVE_LOG
}Logging_TypeDef;

typedef enum 
{
  UP = 0,
  DOWN,
  ENTER,
  MENU,
  NONE
} Key_TypeDef;

extern Key_TypeDef Key_Pressed;


void Sec_Timer(void);
void GetBkp_Data(void);
void RFM75_Rx_Data(void);
uint8_t Read_MASTER_ID(void);
void displayDeviceCounts(uint32_t Conn_Slave_Cnt, int columnWidth, int displayWidth);
int getNumberOfDigits(uint32_t num);
int calculateTextWidth(int numDigits, int columnWidth);
Key_TypeDef KEY_SCAN(void);
void Serial_PutString(char *str);
void __Logging(Logging_TypeDef Logging);
void mSec_Delay(unsigned int nTime);
void nSec_Delay(unsigned int nTime);
#endif // __MISC_FUNCTION_H