/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
FlagStatus Flag_PWR_LED , Flag_Rec;
extern FlagStatus Flag_Test;
uint16_t test_cnt = 0, USB_LED_Fre = 2;
/* USER CODE END TD */
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ETH_HandleTypeDef heth;
extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
/* USER CODE BEGIN EV */
uint8_t Test_Fre = 5;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
  Serial_PutString("NMI Handeler");
   Display_SCREEN(DISPLAY_HARDFAULT);
   SYSTEM_SW_RESET;
  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
  // Display_SCREEN(DISPLAY_HARDFAULT);
  Serial_PutString("HardFault Handeler");
  SYSTEM_SW_RESET;
  /* USER CODE END HardFault_IRQn 0 */
  
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */
  Serial_PutString("MemManage Handeler");
   Display_SCREEN(DISPLAY_HARDFAULT);
   SYSTEM_SW_RESET;
  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */
  Serial_PutString("BusFault Handeler");
  SYSTEM_SW_RESET;
  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */
  Serial_PutString("UsageFault Handeler");
  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */
    Serial_PutString("SVG Handeler");
  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  Flag_mSec   = Flag_mDelay = SET;
  Sec_Tick++;
  /* If Sec Tick reaches 1000 == 1 sec, set the Flag_Second*/
  if(Sec_Tick > 999)
  {
    Flag_Second = SET;
    Sec_Tick = 0;
    
    //USB Conn LED Status
    if(Flag_USB_Conn_LED_Status)
    { 
      if(USB_LED_Fre)USB_LED_Fre--;
      if(USB_LED_Fre == 1){ USB_LED_Fre = 2; LED_CON_ON;}
      else LED_CON_OFF;
    }
    // SOFT RESET
    if(SoftRst_Time == 1){ SoftRst_Time = 0; Flag_SOFTRESET = SET;}
    else if(SoftRst_Time) SoftRst_Time--;
    
    if(Flag_PWR_LED) {Flag_PWR_LED = RESET; LED_PWR_ON;}
    else if(!Flag_PWR_LED) {Flag_PWR_LED = SET;  LED_PWR_OFF;}

    if(Flag_LCTU_TOUT)
    {
      if(LCTU_Disc_Freq)LCTU_Disc_Freq--;
      
      if(LCTU_Disc_Freq <= 1)
      {
        LCTU_Disc_Freq = 5;
        Flag_LCTU_TOUT          = RESET;
        Flag_Scanner_Dis_Update = SET;
        Flag_Display_Update     = SET;
      }
    }
  }
  
  if(Sec_Tick % 5 == 0){Flag_check = SET;}
  
  if(Flag_Test_Failed && !Flag_LCTU_TOUT)
  {
    if(Show_Failed_Fre)Show_Failed_Fre--;
  }
 /*------------------ Check Scanner for Test Start --------------------*/
  if(Flag_RecScanner && !Flag_LCTU_TOUT) 
  {
    /* Start Testing */
    if(!Flag_StartTime)
    {
      Flag_StartTime            = SET;
      Flag_Display_Update       = SET;
      Flag_Test_Failed          = RESET;
      Flag_LCD_TEST_PROGRESS    = SET;
      Start_TimeStruct.Hours    = sTime.Hours;               // END TIME - Hours (int)
      Start_TimeStruct.Minutes  = sTime.Minutes;             // END TIME - Minutes (int)
      Start_TimeStruct.Seconds  = sTime.Seconds;             // END TIME - Seconds (int)
      Slave_Req_Time            = 0;
      Not_Conn_Slave_Cnt        = 0;
      Conn_Slave_Cnt            = 0;
      MODBUS_RESP.SlaveId       = 0;
      Total_Test_Time           = 0;
      Req_SlaveID               = 0;
      memset(Conn_SlaveId,0, sizeof(Conn_SlaveId));
      //memset(Not_Conn_SlaveId,0, sizeof(Not_Conn_SlaveId));

      for(int i=0; i <= MAX_SLAVE_DEVICES; i++)
        All_SlaveIDs[i][1] = 0;
    }
    
    if(Slave_Req_Time > (Default_Test_TimeOut * MAX_SLAVE_DEVICES))
    {
      Slave_Req_Time   = 0;
      Flag_Failed_SRID = RESET;
      Flag_RecScanner = Flag_Req_Slave  = RESET;
      Flag_StartTime  = RESET;
      Flag_RAM_BKP    = SET;
      Req_SlaveID     = 0;
    }

    /* Total Test Time */
    Total_Test_Time++;

    /* Send Request in 49 mSec */ //(Sec_Tick % 49 == 0) && 
//    if((Sec_Tick % 999 == 0) && Flag_RFM_Request && !Flag_Req_Slave)
//    {
//      Flag_Req_Slave = Flag_Failed_SRID = SET; 
//      if(Resp_Wait_Time == 0 )Resp_Wait_Time = 500;
//    }
//    else 
    if((Sec_Tick % Default_SReq_Time == 0) && !Flag_Req_Slave) //&& Flag_Modbus_Request 
    {
      Flag_Req_Slave = Flag_Failed_SRID = SET;
      if(Resp_Wait_Time == 0 )Resp_Wait_Time = (Default_SReq_Time * 4) + 5;
    }
    
    /* wait for response time */
    if(Resp_Wait_Time)Resp_Wait_Time--;
    if(Resp_Wait_Time == 1)
    {
      Resp_Wait_Time = 0; // Reset Response Wait Time
      
      if(Req_SlaveID >= MAX_SLAVE_DEVICES) 
      {
        Req_SlaveID        = 0;
        Not_Conn_Slave_Cnt = 0;
        Conn_Slave_Cnt     = 0;
        memset(Conn_SlaveId,0, sizeof(Conn_SlaveId));
      } 
      else 
      {
        //Not_Conn_SlaveId[Not_Conn_Slave_Cnt] =  All_SlaveIDs[Req_SlaveID][0]; // Not Connected
       if(!Flag_RSET_DISCONECT) All_SlaveIDs[Req_SlaveID][1] = 0; // Not Connected
        Req_SlaveID++;
        Not_Conn_Slave_Cnt++; 
      }
    }
    
    /* Wait for 50 mSec for Slave Response and then send new request*/
    if(Flag_Failed_SRID == SET) Slave_Req_Time++; //else Slave_Req_Time = 0;
  }

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  unsigned char RcvIn;
  
  if(LL_USART_IsActiveFlag_RXNE(USART1))
  {
    RcvIn = LL_USART_ReceiveData8(USART1);
   
    if(!Flag_RecScanner && !Flag_LCTU_TOUT)
    {
      if(RcvIn == 10)
      {
        Flag_RecScanner = SET;
        Flag_Req_Slave  = SET;
       // Flag_LCD_TEST_START = SET;
        Flag_LCD_TEST_PROGRESS = SET;
        Flag_Scanner_Dis_Update = SET;
        Data_RcvCnt     = 0;
      }
      else 
      {
        if(RcvIn != 0x0D)
        {
          Scanner_Buf[Data_RcvCnt] = RcvIn;
          Data_RcvCnt++;
        }
      }
    }
  }
  else
  {
    LL_USART_ClearFlag_TC(USART1);
    LL_USART_ClearFlag_ORE(USART1);
    LL_USART_ClearFlag_FE(USART1);
    LL_USART_ClearFlag_NE(USART1);
    LL_USART_ClearFlag_PE(USART1);
    LL_USART_ClearFlag_IDLE(USART1);
    RcvIn = LL_USART_ReceiveData8(USART1);
  };
  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
  uint8_t RespData;
  
  if(LL_USART_IsActiveFlag_RXNE(USART3))
  {
    RespData = LL_USART_ReceiveData8(USART3);
    switch (MODBUS_STATE) 
  {
    case INIT_RCV:
        /* Initialize the state, check if received RespData matches */
        MODBUS_RESP.SlaveId = RespData;
        if(Flag_Wait_Set_Response){Flag_Wait_Set_Response = RESET; Flag_SSId_RespOk = SET; MODBUS_STATE = (MODBUS_RESP.SlaveId == Req_SlaveID) ? GET_FCODE : INIT_RCV;}
        else MODBUS_STATE = (MODBUS_RESP.SlaveId == All_SlaveIDs[Req_SlaveID][0]) ? GET_FCODE : INIT_RCV; // == All_SlaveIDs[Req_SlaveID][0]
        break;

    case GET_FCODE:
        /* Retrieve the function code and reset the receive counter */
        MODBUS_RESP.FunctionCode = RespData;
        RcvCnt = 0;

        /* Set appropriate flags based on function code */
        if (MODBUS_RESP.FunctionCode == 0x02){MODBUS_STATE = GET_STRT_ADDR; Flag_BitRead       = SET;}     // Bit Read operation
        else if (MODBUS_RESP.FunctionCode == 0x03){MODBUS_STATE = GET_STRT_ADDR; Flag_RegRead  = SET;} // Register Read operation
        else if ((MODBUS_RESP.FunctionCode == 0x04) || (MODBUS_RESP.FunctionCode == 0x10)){MODBUS_STATE = GET_STRT_ADDR; Flag_RegWrite = SET;} // Register Write operation
        else MODBUS_STATE = INIT_RCV;  // Reset to initialization state for invalid function code

        break;

    case GET_STRT_ADDR:
        /* Retrieve the start address (two bytes: MSB and LSB) */
        (RcvCnt++ == 0) ? (ByteMSB = RespData) : (ByteLSB = RespData);

        /* If both bytes are received, compute the start address and move to GET_LENGTH */
        if (RcvCnt >= 2) 
        {
            MODBUS_RESP.StartAddr = (ByteMSB << 8) | ByteLSB; // Combine MSB and LSB into start address
            MODBUS_STATE = GET_LENGTH;        // Move to length retrieval state
            RcvCnt = 0;                       // Reset receive counter
        }
        break;

    case GET_LENGTH:
        /* Retrieve the length of the data (two bytes: MSB and LSB) */
       (RcvCnt++ == 0) ? (ByteMSB = RespData) : (ByteLSB = RespData);
        /* If both bytes are received, compute the length and determine next state */
        if (RcvCnt >= 2) 
        {
            MODBUS_RESP.NumBytes = (ByteLSB << 8) | ByteMSB;  // Combine MSB and LSB into length
            MODBUS_STATE = (Flag_RegWrite == SET) ? GET_DATA : GET_CRC; // Determine next state
            RcvCnt = 0;                     // Reset receive counter
        }
        break;

    case GET_DATA:
        /* Receive data bytes and store in Holding Register */
        MODBUS_RESP.HRegister[RcvCnt++] = RespData;

        /* If all bytes are received, move to CRC check state */
        if (RcvCnt >= MODBUS_RESP.NumBytes) 
        {
            MODBUS_STATE = GET_CRC;  // Move to CRC state
            RcvCnt = 0;              // Reset receive counter
        }
        break;

    case GET_CRC:
        /* Retrieve the CRC value (two bytes: MSB and LSB) */
        (RcvCnt++ == 0) ? (ByteMSB = RespData) : (ByteLSB = RespData);

        /* If both bytes are received, compute CRC and finalize */
        if (RcvCnt >= 2) 
        {
            MODBUS_RESP.RSP_CRC = (ByteMSB << 8) | ByteLSB; // Combine MSB and LSB into CRC
            RcvCnt = 0;                             // Reset receive counter
            
           if((MODBUS_RESP.HRegister[1] == 0x00) || (MODBUS_RESP.HRegister[2] == 0x01) || (MODBUS_RESP.HRegister[2] == 0x01))
           {
              Conn_SlaveId[Conn_Slave_Cnt] = MODBUS_RESP.SlaveId;
              All_SlaveIDs[Req_SlaveID][1] = 1;
              Flag_DataRdy                 = SET;     // Set data ready flag
              Flag_Req_Slave               = SET;
              RespData                     = 0;
              Resp_Wait_Time               = 0;
              
              Conn_Slave_Cnt++;
              Flag_Display_Update          = SET; // Display Update

              if(Conn_Slave_Cnt >= MAX_SLAVE_DEVICES)
              {
                Flag_RecScanner  = Flag_Req_Slave  = RESET;
                Flag_StartTime   = RESET;
                Flag_RAM_BKP     = SET;
                Req_SlaveID      = 0;
              }
              else
              {
                Req_SlaveID++;
              }
           }
            MODBUS_STATE              = INIT_RCV;                // Reset to initialization state
        }
        break;

    default:
        /* Handle invalid state by resetting to initialization */
        MODBUS_STATE = INIT_RCV;
        break;
  }
   
   
//       Test_Buff[RcvCnt] = RespData;
//        
//        if(RcvCnt >= 8)
//        {
//          RcvCnt = 0;
//          if(Test_Buff[0] == All_SlaveIDs[Req_SlaveID][0])Flag_Test = SET; else memset(Test_Buff,0,sizeof(Test_Buff));
//        }
//        else RcvCnt++;
    }
  else
  {
    LL_USART_ClearFlag_TC(USART3);
    LL_USART_ClearFlag_ORE(USART3);
    LL_USART_ClearFlag_FE(USART3);
    LL_USART_ClearFlag_NE(USART3);
    LL_USART_ClearFlag_PE(USART3);
    LL_USART_ClearFlag_IDLE(USART3);
    RespData = LL_USART_ReceiveData8(USART3);
  };
  /* USER CODE END USART3_IRQn 0 */
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles SPI3 global interrupt.
  */
void SPI3_IRQHandler(void)
{
  /* USER CODE BEGIN SPI3_IRQn 0 */

  /* USER CODE END SPI3_IRQn 0 */
  /* USER CODE BEGIN SPI3_IRQn 1 */

  /* USER CODE END SPI3_IRQn 1 */
}

/**
  * @brief This function handles Ethernet global interrupt.
  */
void ETH_IRQHandler(void)
{
  /* USER CODE BEGIN ETH_IRQn 0 */

  /* USER CODE END ETH_IRQn 0 */
  HAL_ETH_IRQHandler(&heth);
  /* USER CODE BEGIN ETH_IRQn 1 */

  /* USER CODE END ETH_IRQn 1 */
}

/**
  * @brief This function handles Ethernet wake-up interrupt through EXTI line 19.
  */
void ETH_WKUP_IRQHandler(void)
{
  /* USER CODE BEGIN ETH_WKUP_IRQn 0 */

  /* USER CODE END ETH_WKUP_IRQn 0 */
  HAL_ETH_IRQHandler(&heth);
  /* USER CODE BEGIN ETH_WKUP_IRQn 1 */

  /* USER CODE END ETH_WKUP_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */
//   if (hhcd_USB_OTG_FS.State == HAL_HCD_STATE_READY)
//    {
//        if (USBH_MSC_IsReady(&hUsbHostFS))
//        {
//            // USB is connected and ready; do nothing.
//        }
//        else
//        {
//            // USB device disconnected or not ready
//            Appli_state = APPLICATION_DISCONNECT;
//        }
//    }
//    else
//    {
//        // USB is not ready (disconnected, error, etc.)
//        Appli_state = APPLICATION_DISCONNECT;
//    }
  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
