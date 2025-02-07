#include "main.h"

char RF_Buffer[100]; // RF BUFFER

/* FlagStatus Type Variable Defien */
FlagStatus 
Flag_mSec,
Flag_Second,
Flag_USBOK,
Flag_ModbusOK,
Flag_RF_OK,
Flag_ETH_OK,
Flag_pass,
Flag_Inverse,
Flag_Req_Slave,
Flag_Log_Save,
Flag_Modbus_Request,
Flag_RFM_Request,
Flag_Conn_Device,
Flag_Not_Conn_Device,
Flag_RecScanner,
Flag_Invalidate_Cmd,
Flag_USB_Connected,
Flag_Backup_Data,
Flag_RAM_BKP,
Flag_StartTime,
Flag_mDelay,
Flag_Failed_SRID,
Flag_SOFTRESET,
Flag_Display_Update,
Flag_Write_Flash,
Flag_Copy_Logs,
Flag_Test,
Flag_Inverse,
Flag_All_Id_Done,
Flag_LCD_TEST_START,
Flag_Set_SlaveID,
Flag_Test_Failed,
Flag_check,
Flag_LCD_TEST_PROGRESS,
Flag_Scanner_Dis_Update,
Flag_Invalidate_Cmd,
Flag_Wait_Set_Response,
Flag_SSId_RespOk,
Flag_Scanner_Dis_Update,
Flag_with_All_SlaveID,
Flag_LCTU_TOUT,
Flag_USB_Conn_LED_Status,
Flag_Reconnect_USB,
Flag_Wait_USB,
Flag_RSET_DISCONECT,
Flag_FileReadFail,
Flag_MainScreen;

/* uint8_t Type Variable Defien */
uint8_t  Conn_Slave_Cnt         = 0,
         Not_Conn_Slave_Cnt     = 0,
         MASTER_ID              = 0,
         Req_SlaveID            = 0,
         Test_TimeOut           = 0,
         SoftRst_Time           = 0,
         LCTU_Disc_Freq         = 5,
         Show_Failed_Fre        = 2;

/* char Type Variable Defien */
char    *Ptr                    = NULL,
        Conn_SlaveId[200]       = {0},
        Not_Conn_SlaveId[200]   = {0},
        Log_File_Name[100]      = {0},
        Log_Dir_Name[100]       = {0},
        Log_Buffer[20][256]     = {0},
        RF_Buffer[100]          = {0},
        Scanner_Buf[256]        = {0},
        RTC_Time[10]            = {0},
        RTC_Date[12]            = {0},
        Result[10]              = {0},
        All_SlaveIDs[200][3]    = {0},
        Show_FailedId_Buff[256] = {0},
        Test_Buff[256]          = {0};

uint8_t Req_Buff[9]             = {0};



/* uint16_t Type Variable Defien */
uint16_t Sec_Tick               = 0, 
         count_up               = 0,
         //count_down             = 0,
         count_enter            = 0,
         count_menu             = 0,
        // Main_Screen_Disp       = 0,
         Total_Log_Count        = 0,
         Slave_Device_ID        = 0,
         Log_Cnt                = 0,
         Data_RcvCnt            = 0,
         Total_Test_Time        = 0,
         Slave_Req_Time         = 0,
         ConfSlaveId            = 0,
         Default_Test_TimeOut  = 0,
         Default_SReq_Time      = 0,
         Resp_Wait_Time         = 0,
         MAX_SLAVE_DEVICES      = 0;

/* uint32_t Type Variable Define */
uint32_t EpoachBytes            = 0,
         BKP_Logs_Cnt           = 0;



End_RTC_TimeTypeDef  Start_TimeStruct = {0};

LL_RTC_InitTypeDef RTC_InitStruct     = {0};
LL_RTC_TimeTypeDef RTC_TimeStruct     = {0};
LL_RTC_DateTypeDef RTC_DateStruct     = {0};
// 
 RTC_HandleTypeDef hrtc= {0};
 RTC_TimeTypeDef sTime = {0};
 RTC_DateTypeDef sDate = {0};

LL_USART_InitTypeDef USART_InitStruct = {0};