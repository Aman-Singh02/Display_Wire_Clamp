#include "main.h"

extern char SGCData[ETH_RX_BUFFER];
extern uint8_t IP_ADDRESS[4],
               NETMASK_ADDRESS[4],
               GATEWAY_ADDRESS[4];

/****************************************************************************
 * Function Name  : CheckConfig
 * Description    : Check the configuration data
 * Input          : char *Str
 * Output         : None
 * Return         : None
 * *************************************************************************/
char *CheckConfig(char *SGCData)
{
  char *Str = NULL;
  static char buf[512] = {0};
  
  if (strstr(SGCData, "SET"))
  {
    sprintf(buf, "SET : ");
    Flag_Invalidate_Cmd  = SET;
    if ((Str = strstr(SGCData, "MDID:")))   // Master ID config
    {
      MASTER_ID = (uint8_t)strtol(Str + 5, NULL, 10);

      // Check if the Master ID is valid
      if ((MASTER_ID > 0) && (MASTER_ID < 255))
      {
       strcat(buf, "MASTER ID CONFIG DONE\n");
       Flag_Invalidate_Cmd = RESET;
      }
      else 
      {
        Flag_Invalidate_Cmd = SET;
      }
    }
    if ((Str = strstr(SGCData, "SDID:")))  // Slave ID config
    {
      Str += 5; 
      Req_SlaveID = (uint16_t)strtol(Str, &Str, 10);

      // Check if the Slave ID is valid
      if ((Req_SlaveID > 0) && (Req_SlaveID < 255)) 
      {
        if (*Str == ',') Str++; 
        uint16_t SlaveId = (uint16_t)strtol(Str, NULL, 10);
        
        sprintf(Test_Buff, "%d%d", MASTER_ID, SlaveId);
        ConfSlaveId = (uint16_t)atoi(Test_Buff);
        
        Flag_Set_SlaveID = SET;
        Flag_Invalidate_Cmd = RESET;
        strcat(buf, "SLAVE ID CONFIG DONE\n");
      }
      else 
      {
        Flag_Invalidate_Cmd = SET;
      }
    }
    if ((Str = strstr(SGCData, "MSIDS:")))  // Max Slave IDs  config
    {
      memset(All_SlaveIDs,0,sizeof(All_SlaveIDs));
      Str += 6;
      MAX_SLAVE_DEVICES = (uint8_t)strtol(Str, &Str, 10);

      // Check if the number of Slave Devices is valid
      if (MAX_SLAVE_DEVICES > 0 && MAX_SLAVE_DEVICES <= 255) 
      {
        if (*Str == ',')Str++;
        uint8_t i = 0;
        while (i < MAX_SLAVE_DEVICES && *Str != '*' && *Str != '\0') 
        {
          All_SlaveIDs[i++][0] = (uint8_t)strtol(Str, &Str, 0);
          if (*Str == ',') Str++;
        }
        strcat(buf, "MAX SLAVE ID CONFIG DONE\n");
        Flag_Display_Update = SET;
        Flag_Invalidate_Cmd = RESET;
      }
      else 
      {
        Flag_Invalidate_Cmd = SET;
      }
    }
    if ((Str = strstr(SGCData, "SREQF:"))) // Slave Request Time Frequency
    {
      Default_SReq_Time = atoi(Str + 6);

      // Check if the Request Slave Time is valid
      if (Default_SReq_Time >= 30 && Default_SReq_Time <= 50) 
      {
        strcat(buf, "SLAVE REQUEST TIME CONFIG DONE\n");
        Flag_Invalidate_Cmd = RESET;
      }
      else 
      {
        Flag_Invalidate_Cmd = SET;
      }
    }
    if ((Str = strstr(SGCData, "TTOUTF:")))  // Test TimeOut Frequency
    {
      Default_Test_TimeOut = atoi(Str + 7);

      // Check if the Test TimeOut is valid
      if (Default_Test_TimeOut >= 200 && Default_Test_TimeOut <= 10000) 
      {
        strcat(buf, "TEST TIMEOUT CONFIG DONE\n");
        Flag_Invalidate_Cmd = RESET;
      }
      else 
      {
        Flag_Invalidate_Cmd = SET;
      }
    }
    if (strstr(SGCData, "SPTYPE:"))  // Slave Protocol Type
    {
      Flag_Invalidate_Cmd = RESET;
      if (strstr(SGCData, "MODBUS")) 
      {
        Flag_RFM_Request = RESET;
        Flag_Modbus_Request = SET;
        strcat(buf, "SWITCHED TO MODBUS MODE\n");
      }
      else if(strstr(SGCData, "RFM"))
      {
        Flag_Modbus_Request = RESET;
        Flag_RFM_Request    = SET;
        strcat(buf, "SWITCHED TO RFM MODE\n");
      }
      else if (strstr(SGCData, "MR")) 
      {
        Flag_RFM_Request    = SET;
        Flag_Modbus_Request = SET;
        strcat(buf, "SWITCHED TO BOTH MODBUD and RFM MODE\n");
      } 
      else Flag_Invalidate_Cmd = SET;
    }
    if(strstr(SGCData, "RSTDSID")) // RESET DISCONNECTED SLAVE ID
    {
      Flag_RSET_DISCONECT = SET;
    }
    if ((Str = strstr(SGCData, "TimeStamp:")))  // TimeStamp config
    {
      Str += 10;
      sscanf(Str, "%2hhu/%2hhu/%2hhu %2hhu:%2hhu:%2hhu", &sDate.Date, &sDate.Month, &sDate.Year, &sTime.Hours, &sTime.Minutes, &sTime.Seconds);
      HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      strcat(buf, "TIMESTAMP CONFIG DONE\n");
      Flag_Invalidate_Cmd = RESET;
    }
    if(Flag_Invalidate_Cmd)
    {
      Flag_Invalidate_Cmd = RESET;
      strcat(buf, "INVALID COMMAND\n");
    }
    else Flag_Write_Flash = SET;
  }
  else if (strstr(SGCData, "GET"))
  {
    sprintf(buf, "GET : ");
    Flag_Invalidate_Cmd = SET;
    if (strstr(SGCData, "MDID"))  // Get Master ID
    { 
      sprintf(buf + strlen(buf), "MASTER ID = %u\n", MASTER_ID);
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "MSIDS")) // Get Max Slave IDs config
    {
      sprintf(buf + strlen(buf), "MAX SLAVE DEVICES = %u\n", MAX_SLAVE_DEVICES);
      strcat(buf, "SLAVE IDS: ");
      for (uint8_t i = 0; i < MAX_SLAVE_DEVICES; i++) {
        char id_buf[5];
        sprintf(id_buf, "%u", All_SlaveIDs[i][0]);
        strcat(buf, id_buf);
        if (i < MAX_SLAVE_DEVICES - 1) {
          strcat(buf, ",");
        }
      }
      strcat(buf, "\n");
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "SREQF")) // Get Slave Request Time Frequency
    {
      sprintf(buf + strlen(buf), "REQUEST SLAVE TIME = %u\n", Default_SReq_Time);
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "TTOUTF")) // Get Test TimeOut Frequency
    {
      sprintf(buf + strlen(buf), "TEST TIMEOUT = %u\n", Default_Test_TimeOut);
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "SPTYPE")) // Get Slave Protocol Type
    {
      if (Flag_RFM_Request == SET && Flag_Modbus_Request == RESET) {
        strcat(buf, "TYPE = RFM\n");
      }
      else if (Flag_RFM_Request == SET && Flag_Modbus_Request == SET) {
        strcat(buf, "TYPE = RM\n");
      }
      else if (Flag_Modbus_Request == SET && Flag_RFM_Request == RESET) {
        strcat(buf, "TYPE = MODBUS\n");
      }
      else {
        strcat(buf, "TYPE = UNKNOWN\n");
      }
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "TimeStamp")) // Get TimeStamp config
    {
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      sprintf(buf + strlen(buf), "TIMESTAMP = %02u/%02u/%02u %02u:%02u:%02u\n",
              sDate.Date, sDate.Month, sDate.Year, sTime.Hours, sTime.Minutes, sTime.Seconds);
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "CONFIG")) // Get all config data
    {
       // Copy all get command data in buffer
      sprintf(buf, "FIRMWARE VERSION: %s\nMASTER ID = %d\nMAX SLAVE DEVICES = %u\nTEST TIME OUT = %d\nREQUEST TO SLAVE TIME = %d",
              Software_Version,MASTER_ID, MAX_SLAVE_DEVICES, Default_Test_TimeOut,Default_SReq_Time);
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "LOG")) // Get Log data
    {
      if (BKP_Logs_Cnt > 0) {
        sprintf(buf, "GET : START LOGGING\n");
        Flag_Backup_Data = SET;
      } else {
        sprintf(buf, "GET : No Data Log Found\n");
      }
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "IP,PORT")) // Get IP and PORT
    {
      char temp[16]; // Temporary buffer for IP segments
      // Build the IP address string
      snprintf(temp, sizeof(temp), "%d.%d.%d.%d", IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
      strcat(buf, "IP:");
      strcat(buf, temp);
      strcat(buf, ",  PORT:10\nNETMASK:");
      
      // Build the NETMASK string
      snprintf(temp, sizeof(temp), "%d.%d.%d.%d", NETMASK_ADDRESS[0], NETMASK_ADDRESS[1], NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
      strcat(buf, temp);
      strcat(buf, "\nGATEWAY:");
      
      // Build the GATEWAY string
      snprintf(temp, sizeof(temp), "%d.%d.%d.%d", GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
      strcat(buf, temp);
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "FW")) // Get Firmware version
    {
      strcat(buf, "FIRMWARE VERSION: ");
      strcat(buf, Software_Version);
      
      Flag_Invalidate_Cmd = RESET;
    }
    if(Flag_Invalidate_Cmd) 
    {
      Flag_Invalidate_Cmd = RESET;
      strcat(buf, "INVALID COMMAND\n");
    }
  }
  else if (strstr(SGCData, "CLR"))
  {
    sprintf(buf, "CLR : ");
    Flag_Invalidate_Cmd = SET;
    if (strstr(SGCData, "MDID")) {
      MASTER_ID = 0;
      strcat(buf, "MASTER ID RESET DONE\n");
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "SDID")) {
      Req_SlaveID = 0;
      strcat(buf, "SLAVE ID RESET DONE\n");
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "MSIDS")) {
      MAX_SLAVE_DEVICES = 0;
      memset(All_SlaveIDs, 0, sizeof(All_SlaveIDs));
      strcat(buf, "MAX SLAVE DEVICES AND IDS RESET DONE\n");
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "SREQF")) {
      Default_Test_TimeOut = 0;
      strcat(buf, "REQUEST SLAVE TIME RESET DONE\n");
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "TTOUTF")) {
      Default_Test_TimeOut = 0;
      strcat(buf, "TEST TIMEOUT RESET DONE\n");
      Flag_Invalidate_Cmd = RESET;
    }
      
    if (strstr(SGCData, "SPTYPE")) {
      Flag_RFM_Request = RESET;
      Flag_Modbus_Request = RESET;
      strcat(buf, "SPTYPE FLAGS RESET DONE\n");
      Flag_Invalidate_Cmd = RESET;
    }
    if (strstr(SGCData, "TimeStamp")) {
      sDate.Date = 1;
      sDate.Month = 1;
      sDate.Year = 0;
      sTime.Hours = 0;
      sTime.Minutes = 0;
      sTime.Seconds = 0;
      HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      strcat(buf, "TIMESTAMP RESET DONE\n");
      Flag_Invalidate_Cmd = RESET;
    }
    if(Flag_Invalidate_Cmd){
      Flag_Invalidate_Cmd = RESET;
      strcat(buf, "INVALID COMMAND\n");
    }
    Flag_Write_Flash = SET;
  }
  else if (strstr(SGCData, "SOFTRST"))
  {
    SoftRst_Time = 5;
    strcat(buf, "SYSTEM SOFT RESET\n");
  }
  else
  {
    strcat(buf, "INVALID COMMAND\n");
  }
  return buf;
}