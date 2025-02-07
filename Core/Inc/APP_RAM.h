#ifndef __APP_RAM_H
#define __APP_RAM_H

#define DefualtTimeOut    120
/* FlagStatus Type Variable Defien */
extern FlagStatus 
        Flag_mSec,
        Flag_Second,
        Flag_USBOK,
        Flag_ModbusOK,
        Flag_RF_OK,
        Flag_ETH_OK,
        Flag_Req_Slave,
        Flag_Inverse,
        Flag_pass,
        Flag_Log_Save,
        Flag_Modbus_Request,
        Flag_RFM_Request,
        Flag_Conn_Device,
        Flag_Not_Conn_Device,
        Flag_RecScanner,
        Flag_USB_Connected,
        Flag_Backup_Data,
        Flag_RAM_BKP,
        Flag_StartTime,
        Flag_mDelay,
        Flag_check,
        Flag_Failed_SRID,
        Flag_SOFTRESET,
        Flag_Display_Update,
        Flag_Write_Flash,
        Flag_Copy_Logs,
        Flag_Test,
        Flag_All_Id_Done,
        Flag_LCD_TEST_START,
        Flag_Set_SlaveID,
        Flag_Test_Failed,
        Flag_LCD_TEST_PROGRESS,
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
        Flag_MainScreen; //Log Copy Time OUT


/* char [Type Variable Defien */
extern char *Ptr,
        Conn_SlaveId[200],
        Not_Conn_SlaveId[200],
        Log_File_Name[100],
        Log_Dir_Name[100],
        Log_Buffer[20][256],
        RF_Buffer[100],
        Scanner_Buf[256],
        RTC_Time[10],
        RTC_Date[12],
        Result[10],
        Test_Buff[256],
        Show_FailedId_Buff[256],
        All_SlaveIDs[200][3];

/* uint8_t Type Variable Defien */
extern uint8_t  
        Conn_Slave_Cnt,
        Not_Conn_Slave_Cnt,
        MASTER_ID,
        count_down,
        Req_SlaveID,
        Test_TimeOut,
        SoftRst_Time,
        LCTU_Disc_Freq,
        Show_Failed_Fre;

extern uint8_t Req_Buff[9];
//extern uint8_t count_down = 0;
/* uint16_t Type Variable Defien */
extern uint16_t Sec_Tick,
                Total_Log_Count,
                Slave_Device_ID,
                count_up,
                Log_Cnt,
                Data_RcvCnt,
                Total_Test_Time,
                Slave_Req_Time,
                ConfSlaveId,
                Default_Test_TimeOut,
                Default_SReq_Time,
                Resp_Wait_Time,
                MAX_SLAVE_DEVICES;

/* uint32_t Type Variable Defien */
extern uint32_t EpoachBytes,
                BKP_Logs_Cnt;

/* Struct Defined */
typedef struct
{
  uint8_t Hours;     
  uint8_t Minutes;    
  uint8_t Seconds;   
} End_RTC_TimeTypeDef;

extern End_RTC_TimeTypeDef  Start_TimeStruct;

extern   LL_RTC_InitTypeDef RTC_InitStruct;
extern   LL_RTC_TimeTypeDef RTC_TimeStruct;
extern   LL_RTC_DateTypeDef RTC_DateStruct;

extern RTC_HandleTypeDef hrtc;
extern  RTC_TimeTypeDef sTime;
extern  RTC_DateTypeDef sDate;

extern LL_USART_InitTypeDef USART_InitStruct;

#endif /* __APP_RAM_H */