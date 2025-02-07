/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "fatfs.h"
#include "usb_host.h"
#include "File_Handling.h"
#include "lwip.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern ApplicationTypeDef Appli_state;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_IWDG_Init(void);
static void MX_RTC_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART1_UART_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

    int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_IWDG_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init(); // Scanner
  MX_USART3_UART_Init(); // MODBUS
  SPI_FLASH_Init();
  MX_USB_HOST_Init();
  MX_FATFS_Init();
  MX_LWIP_Init();
  /* USER CODE BEGIN 2 */
  App_Init();
  /* USER CODE END 2 */
 /* Initialize the RFM */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_LWIP_Process();
    MX_USB_HOST_Process();
    /* USER CODE BEGIN 3 */
     
    if(Flag_check){Flag_check = RESET; KEY_FUNCTION();}
    
    /* Check for the USB Connection */
    if(!Flag_USB_Connected && !Flag_LCTU_TOUT)
    { 
      /* Send Request for Clamp Status Data to Slave Device*/
      if(Flag_Req_Slave)
        Request_To_Slave(All_SlaveIDs[Req_SlaveID][0]);
      
      /* RFM75 Receive Data */
      if(Flag_RFM_Request) RFM75_Rx_Data();
      
      /* Set slave ID in Slave Device */
      __SET_SlaveID();
      
      /* Display Scanner data */
      if(Flag_Scanner_Dis_Update) {Flag_Scanner_Dis_Update = RESET; Display_SCREEN(FITTED_CLAMPS);}
      
      /* Test Process */
      if(Flag_LCD_TEST_START) {Flag_LCD_TEST_START = RESET; Display_SCREEN(TEST_STARTED);}
      else if(Flag_LCD_TEST_PROGRESS) {Flag_LCD_TEST_PROGRESS = RESET; Display_SCREEN(TEST_PROGRESS);}
      
      /* Check the connected devices */
      if(Flag_Display_Update) 
      {
        Flag_Display_Update = RESET;
        Display_SCREEN(CONN_DEVICES);
      }
      
      /* Check for the data received from Scanner then make log*/
      if(Flag_RAM_BKP) GetBkp_Data();
      
    }
    /* Second Timer */
    if (Flag_Second) Sec_Timer();
    
    /* Write Internal Flash */
    if(Flag_Write_Flash){Flag_Write_Flash = RESET; SET_Config();}
    
    /* SYSTEM Software Reset */
    if(Flag_SOFTRESET){Flag_SOFTRESET = RESET; SYSTEM_SW_RESET;}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
 RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

 /** Configure the main internal regulator output voltage
 */
 __HAL_RCC_PWR_CLK_ENABLE();
 __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

 /** Initializes the RCC Oscillators according to the specified parameters
 * in the RCC_OscInitTypeDef structure.
 */
 RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
 RCC_OscInitStruct.HSEState = RCC_HSE_ON;
 RCC_OscInitStruct.LSIState = RCC_LSI_ON;
 RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
 RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
 RCC_OscInitStruct.PLL.PLLM = 15;
 RCC_OscInitStruct.PLL.PLLN = 144;
 RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
 RCC_OscInitStruct.PLL.PLLQ = 5;
 if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
 {
   Error_Handler();
 }
 
 /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

 /** Initializes the CPU, AHB and APB buses clocks
 */
 RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
 RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
 RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
 RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
 RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

 if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
 {
   Error_Handler();
 }

 HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_128);
  LL_IWDG_SetReloadCounter(IWDG, 4095);
  while (LL_IWDG_IsReady(IWDG) != 1)
  {
  }

  LL_IWDG_ReloadCounter(IWDG);
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 3;
  sTime.Minutes = 10;
  sTime.Seconds = 40;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_SATURDAY;
  sDate.Month = RTC_MONTH_DECEMBER;
  sDate.Date = 14;
  sDate.Year = 0x18;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}


/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct   = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin                 = LL_GPIO_PIN_9|LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode                = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed               = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType          = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate           = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate            = 9600;
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */
   /* Enable RXNE interrupt */
    LL_USART_EnableIT_RXNE(USART1);
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  /**USART3 GPIO Configuration
  PD8   ------> USART3_TX
  PD9   ------> USART3_RX
  */
  GPIO_InitStruct.Pin        = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate  = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USART3 interrupt Init */
  NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART3_IRQn);

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  USART_InitStruct.BaudRate            = 115200;
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART3);
  LL_USART_Enable(USART3);
  /* USER CODE BEGIN USART3_Init 2 */
/* Enable RXNE interrupt */
  LL_USART_EnableIT_RXNE(USART3);
  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, ETH_CRS_Pin|ETH_COL_Pin|ETH_MCO_Pin|SPI3_CS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, SW4_Pin|RFM_SCK_Pin|RFM_MOSI_Pin|LED_PWR_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOE,LED_SYNC_Pin|LED_CON_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, RFM_CSN_Pin|RFM_CE_Pin);
  
  
  

  /**/
  GPIO_InitStruct.Pin        = ETH_CRS_Pin|ETH_COL_Pin|SPI3_CS_Pin;
  GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
   
  
  /*---------*/
  GPIO_InitStruct.Pin =   SW4_Pin|LED_PWR_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  
  /* LCD PIN CONFIGURATION */
  GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;

  GPIO_InitStruct.Pin        = LCD_VCC_ON_Pin; LL_GPIO_Init(LCD_VCC_ON_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_WR_Pin;     LL_GPIO_Init(LCD_WR_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_RES_Pin;    LL_GPIO_Init(LCD_RES_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_CS_Pin;     LL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_RD_Pin;     LL_GPIO_Init(LCD_RD_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_RS_Pin;     LL_GPIO_Init(LCD_RS_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_PS_Pin;     LL_GPIO_Init(LCD_PS_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin        = LCD_DB7_Pin;     LL_GPIO_Init(LCD_DB7_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_DB6_Pin;     LL_GPIO_Init(LCD_DB6_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_DB5_Pin;     LL_GPIO_Init(LCD_DB5_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_DB4_Pin;     LL_GPIO_Init(LCD_DB4_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_DB3_Pin;     LL_GPIO_Init(LCD_DB3_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_DB2_Pin;     LL_GPIO_Init(LCD_DB2_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_DB1_Pin;     LL_GPIO_Init(LCD_DB1_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = LCD_DB0_Pin;     LL_GPIO_Init(LCD_DB0_GPIO_Port, &GPIO_InitStruct);
  
  /*------------------- RFM OUTPUT---------------*/
  GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Pin        = RFM_SCK_Pin|RFM_MOSI_Pin; LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.Pin        = RFM_CSN_Pin|RFM_CE_Pin;   LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  /*--------- RFM MISO INPUT -----------------*/
  GPIO_InitStruct.Pin  = RFM_MISO_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(RFM_MISO_GPIO_Port, &GPIO_InitStruct);

  
  GPIO_InitStruct.Pin = LED_SYNC_Pin|LED_CON_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /**/

  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  
  GPIO_InitStruct.Pin  = DIP_SW0_Pin|DIP_SW1_Pin|DIP_SW2_Pin|DIP_SW3_Pin|DIP_SW4_Pin;LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  GPIO_InitStruct.Pin  = DIP_SW5_Pin|DIP_SW6_Pin|DIP_SW7_Pin; LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /*Key Scan */
  GPIO_InitStruct.Pin  = SW1_Pin|SW2_Pin|SW3_Pin;LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.Pin  = SW4_Pin;LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /**/


  /**/
  GPIO_InitStruct.Pin = RS485_Toggle_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);


/* USER CODE BEGIN MX_GPIO_Init_2 */
  LED_PWR_OFF;
  LED_SYNC_OFF;
  LED_CON_OFF;
  
  // LCD Baklit COntrol
//  LCD_BACKLIGHT_OFF;

/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
 //  send_data("MAIN Error Handeler");
  __disable_irq();
  SYSTEM_SW_RESET;
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */