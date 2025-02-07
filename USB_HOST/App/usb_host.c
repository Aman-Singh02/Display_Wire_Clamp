/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
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

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "fatfs.h"


/* USER CODE BEGIN Includes */
#include "File_Handling.h"
extern FILINFO USBHfno;
extern char USB_Read_Data[256];
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */

  /* USER CODE END USB_HOST_Init_PreTreatment */

  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostFS);
  
//    Flag_Cehck_USB = RESET;// Read_File //!Flag_Wait_USB && 
//    if((Read_File("/UCSTATUS.txt") != FR_OK) && ((Appli_state  == APPLICATION_READY) || (Appli_state == APPLICATION_START)))
    if((Appli_state  == APPLICATION_READY) && (Read_File("/UCSTATUS.txt") != FR_OK))
    {
      MX_USB_HOST_Init();
      Appli_state = APPLICATION_IDLE;
      Flag_Reconnect_USB = SET;
    }
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  char *ResPtr = NULL;
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
   Flag_USB_Conn_LED_Status = RESET;
  Appli_state = APPLICATION_DISCONNECT;
  LCDWrite_Text(5,5,"USB DISCONNECTED ");
  Unmount_USB();
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  Clear_Area(0,0,127,63);
  LCDWrite_Text(30,30,"USB CONNECTED ");
  Mount_USB();

  Check_USB_Details();   // check space details
  //Scan_USB("/");   // scan for files and directories
  
  Read_File("SetCmd.txt");
  
  if (!Flag_FileReadFail)
  {
    ResPtr = CheckConfig(USB_Read_Data);
    Create_File("/Resp.txt"); // Check USB Connect Status 
    Write_File("/Resp.txt",ResPtr);
  }
  else 
    Data_Log_Init(sDate, sTime);
 
  Create_File("/UCSTATUS.txt"); // Check USB Connect Status 
//  Write_File("/UCSTATUS.txt","Connected");
//  Create_File("/ROOTFILE.txt");
//  Write_File("/ROOTFILE.txt", "This data should be in root file\n");
//
//  Create_Dir("/DIR1");
//  Create_File("/DIR1/DIR1FILE.txt");
//  Write_File("/DIR1/DIR1FILE.txt", "This data should be in DIR1 file\n");
//
//  Create_Dir("/DIR2");
//  Create_Dir("/DIR2/SUBDIR1");
//  Create_File("/DIR2/SUBDIR1/DIR2FILE.txt");
//  Write_File("/DIR2/SUBDIR1/DIR2FILE.txt", "This data should be in DIR2/SUBDIR1 file\n as i have nothing better to write/n so i just wrote this\n");
//
//  Update_File("/ROOTFILE.txt", "This updated data must be in second line of Root File\n");
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

