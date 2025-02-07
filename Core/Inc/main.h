/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_crc.h"
#include "stm32f4xx_ll_iwdg.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_rtc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
  
#include "APP_RAM.h"
#include "LCD_Driver.h"
#include "RFM_Driver.h"
#include "MODBUS_Driver.h"
#include "Epoch_Time.h"
#include "SPI_Flash_Driver.h"
#include "Misc_Function.h"
#include "APP_Init.h"
#include "Scanner.h"
#include "Slave_Read_Write.h"
#include "TCPServerRAW.h"
#include "RTC.h"
#include "Flash.h"
#include "CheckConfig.h"
#include "LCD_Text_Scroll.h"
#include "DISPLAY_SCREEN.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
//#define DEBOUNCE_THRESHOLD 3  // Adjust as needed
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ETH_CRS_Pin LL_GPIO_PIN_0
#define ETH_CRS_GPIO_Port GPIOA
#define ETH_COL_Pin LL_GPIO_PIN_3
#define ETH_COL_GPIO_Port GPIOA



#define SW1_Pin LL_GPIO_PIN_4
#define SW1_GPIO_Port GPIOA
#define SW2_Pin LL_GPIO_PIN_5
#define SW2_GPIO_Port GPIOA
#define SW3_Pin LL_GPIO_PIN_6
#define SW3_GPIO_Port GPIOA
#define SW4_Pin LL_GPIO_PIN_15
#define SW4_GPIO_Port GPIOB


#define LCD_VCC_ON_Pin           LL_GPIO_PIN_0
#define LCD_VCC_ON_GPIO_Port     GPIOD
#define LCD_ON                   LCD_VCC_ON_GPIO_Port->BSRR     = (uint32_t)LCD_VCC_ON_Pin << 16U
#define LCD_OFF                   LCD_VCC_ON_GPIO_Port->BSRR    = (uint32_t)LCD_VCC_ON_Pin


#define LCD_WR_Pin               LL_GPIO_PIN_3
#define LCD_WR_GPIO_Port         GPIOD
#define LCD_RES_Pin              LL_GPIO_PIN_15
#define LCD_RES_GPIO_Port        GPIOE
#define LCD_CS_Pin               LL_GPIO_PIN_1
#define LCD_CS_GPIO_Port         GPIOD
#define LCD_RD_Pin               LL_GPIO_PIN_2
#define LCD_RD_GPIO_Port         GPIOD
#define LCD_PS_Pin               LL_GPIO_PIN_4
#define LCD_PS_GPIO_Port         GPIOD
#define LCD_RS_Pin               LL_GPIO_PIN_2
#define LCD_RS_GPIO_Port         GPIOB

#define LCD_DB7_Pin              LL_GPIO_PIN_7
#define LCD_DB7_GPIO_Port        GPIOE
#define LCD_DB6_Pin              LL_GPIO_PIN_8
#define LCD_DB6_GPIO_Port        GPIOE
#define LCD_DB5_Pin              LL_GPIO_PIN_9
#define LCD_DB5_GPIO_Port        GPIOE
#define LCD_DB4_Pin              LL_GPIO_PIN_10
#define LCD_DB4_GPIO_Port        GPIOE
#define LCD_DB3_Pin              LL_GPIO_PIN_11
#define LCD_DB3_GPIO_Port        GPIOE
#define LCD_DB2_Pin              LL_GPIO_PIN_12
#define LCD_DB2_GPIO_Port        GPIOE
#define LCD_DB1_Pin              LL_GPIO_PIN_13
#define LCD_DB1_GPIO_Port        GPIOE
#define LCD_DB0_Pin              LL_GPIO_PIN_14
#define LCD_DB0_GPIO_Port        GPIOE


#define RS485_Toggle_Pin LL_GPIO_PIN_10
#define RS485_Toggle_GPIO_Port GPIOD

#define DIP_SW0_Pin LL_GPIO_PIN_11
#define DIP_SW0_GPIO_Port GPIOD

#define DIP_SW1_Pin LL_GPIO_PIN_12
#define DIP_SW1_GPIO_Port GPIOD

#define DIP_SW2_Pin LL_GPIO_PIN_13
#define DIP_SW2_GPIO_Port GPIOD

#define DIP_SW3_Pin LL_GPIO_PIN_14
#define DIP_SW3_GPIO_Port GPIOD

#define DIP_SW4_Pin LL_GPIO_PIN_15
#define DIP_SW4_GPIO_Port GPIOD

#define DIP_SW5_Pin LL_GPIO_PIN_6
#define DIP_SW5_GPIO_Port GPIOC

#define DIP_SW6_Pin LL_GPIO_PIN_7
#define DIP_SW6_GPIO_Port GPIOC

#define DIP_SW7_Pin LL_GPIO_PIN_8
#define DIP_SW7_GPIO_Port GPIOC

#define ETH_MCO_Pin LL_GPIO_PIN_8
#define ETH_MCO_GPIO_Port GPIOA

#define SPI3_CS_Pin LL_GPIO_PIN_15
#define SPI3_CS_GPIO_Port GPIOA

/*
#define IRQ_Pin                                 LL_GPIO_PIN_2
#define IRQ_GPIO_Port                           GPIOA

#define CSN_Pin                                 LL_GPIO_PIN_12
#define CSN_GPIO_Port                           GPIOA

#define CE_Pin                                  LL_GPIO_PIN_11
#define CE_GPIO_Port                            GPIOA

#define SCK_Pin                                 LL_GPIO_PIN_5
#define SCK_GPIO_Port                           GPIOA

#define MOSI_Pin                                LL_GPIO_PIN_1
#define MOSI_GPIO_Port                          GPIOA

#define MISO_Pin                                LL_GPIO_PIN_3
#define MISO_GPIO_Port                          GPIOA


*/
#define RFM_IRQ_Pin LL_GPIO_PIN_5
#define RFM_IRQ_GPIO_Port GPIOD

#define RFM_CSN_Pin LL_GPIO_PIN_6
#define RFM_CSN_GPIO_Port GPIOD
#define RFM_CE_Pin LL_GPIO_PIN_7
#define RFM_CE_GPIO_Port GPIOD
#define RFM_SCK_Pin LL_GPIO_PIN_3
#define RFM_SCK_GPIO_Port GPIOB
#define RFM_MOSI_Pin LL_GPIO_PIN_5
#define RFM_MOSI_GPIO_Port GPIOB

#define RFM_MISO_Pin LL_GPIO_PIN_4
#define RFM_MISO_GPIO_Port GPIOB


#define LED_PWR_Pin LL_GPIO_PIN_9
#define LED_PWR_GPIO_Port GPIOB
#define LED_SYNC_Pin LL_GPIO_PIN_0
#define LED_SYNC_GPIO_Port GPIOE
#define LED_CON_Pin LL_GPIO_PIN_1
#define LED_CON_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

#define SYSTEM_SW_RESET NVIC_SystemReset()
#define Software_Version "V0.0.0"
#define Hardware_Version "V0.0.0"
/* Turn ON the LED_PWR pin */
#define LED_PWR_OFF         (LED_PWR_GPIO_Port->BSRR = LED_PWR_Pin) 
/* Turn OFF the LED_PWR pin */
#define LED_PWR_ON        (LED_PWR_GPIO_Port->BSRR = (LED_PWR_Pin << 16))


/* Turn ON the LED_SYNC pin */
#define LED_SYNC_OFF        (LED_SYNC_GPIO_Port->BSRR = LED_SYNC_Pin) 
/* Turn OFF the LED_SYNC pin */
#define LED_SYNC_ON       (LED_SYNC_GPIO_Port->BSRR = (LED_SYNC_Pin << 16))


/* Turn ON the LED_CON pin */
#define LED_CON_OFF         (LED_CON_GPIO_Port->BSRR = LED_CON_Pin) 
/* Turn OFF the LED_CON pin */
#define LED_CON_ON       (LED_CON_GPIO_Port->BSRR = (LED_CON_Pin << 16))

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
