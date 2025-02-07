/*
 * File_Handling_RTOS.c
 *
 *  Created on: 26-June-2020
 *      Author: Controllerstech.com
 */

#include "main.h"
#include "fatfs.h"
#include "usb_host.h"
   
   
extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */
extern ApplicationTypeDef Appli_state;
char USB_Status_Buff[100];
char USB_Read_Data[256] = {0};
FILINFO USBHfno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pUSBHFatFS;
DWORD fre_clust;
uint32_t total, free_space;


FRESULT Scan_USB (char* pat);
FRESULT Format_USB (void);
FRESULT Write_File (char *name, char *data);
FRESULT Read_File (char *name);
FRESULT Create_File (char *name);
FRESULT Remove_File (char *name);
FRESULT Create_Dir (char *name);
FRESULT Update_File (char *name, char *data);

void Mount_USB (void)
{
	fresult = f_mount(&USBHFatFS, USBHPath, 1);
	if (fresult != FR_OK) sprintf(USB_Status_Buff,"ERROR!!! in mounting USB ...");
	else 
        {
          sprintf(USB_Status_Buff,"USB mounted successfully...");
          Flag_USB_Conn_LED_Status = SET;
          Flag_USB_Connected = SET;
          Flag_LCTU_TOUT = SET;
        }
        
        Clear_Area(0,0,127,63);
        LCDWrite_Text(1,30,USB_Status_Buff);
}

void Unmount_USB (void)
{
	fresult = f_mount(NULL, USBHPath, 1);
	if (fresult == FR_OK) sprintf(USB_Status_Buff,"USB UNMOUNTED successfully...");
	else sprintf(USB_Status_Buff,"ERROR!!! in UNMOUNTING USB");
        
        Clear_Area(0,0,127,63);
        LCDWrite_Text(6,30,USB_Status_Buff);
}

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_USB (char* pat)
{
    DIR dir;
    UINT i;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s",pat);

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
            if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", USBHfno.fname))) continue;
            	if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
            	char *buf = malloc(30*sizeof(char));
            	sprintf (buf, "Dir: %s\r\n", USBHfno.fname);
				
            	sprintf(USB_Status_Buff,buf);
            	free(buf);
                i = strlen(path);
                sprintf(&path[i], "/%s", USBHfno.fname);
                fresult = Scan_USB(path);                     /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            }
            else
            {   /* It is a file. */
           	   char *buf = malloc(30*sizeof(char));
               sprintf(buf,"File: %s/%s\n", path, USBHfno.fname);
			   
               sprintf(USB_Status_Buff,buf);
               free(buf);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_USB (void)
{
    DIR dir;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s","/");

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
            if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", USBHfno.fname))) continue;
            	if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
            	fresult = f_unlink(USBHfno.fname);
            	if (fresult == FR_DENIED) continue;
            }
            else
            {   /* It is a file. */
               fresult = f_unlink(USBHfno.fname);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

FRESULT Write_File (char *name, char *data)
{

	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		
		sprintf(USB_Status_Buff,buf);
	    free(buf);
	    return fresult;
	}

	else
	{
	    /* Create a file with read write access and open it */
	    fresult = f_open(&USBHFile, name, FA_OPEN_EXISTING | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
			
	    	sprintf(USB_Status_Buff,buf);
	        free(buf);
	        return fresult;
	    }

	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "Opening file-->  *%s*  To WRITE data in it\n", name);
			
	    	sprintf(USB_Status_Buff,buf);
	        free(buf);

	    	fresult = f_write(&USBHFile, data, strlen(data), &bw);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\n", fresult, name);
				
	    		sprintf(USB_Status_Buff,buf);
	    		free(buf);
	    	}

	    	/* Close file */
	    	fresult = f_close(&USBHFile);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d in closing file *%s* after writing it\n\n", fresult, name);
				
	    		sprintf(USB_Status_Buff,buf);
	    		free(buf);
	    	}
	    	else
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "File *%s* is WRITTEN and CLOSED successfully\n\n", name);
				
	    		sprintf(USB_Status_Buff,buf);
	    		free(buf);
	    	}
	    }
	    return fresult;
	}
}

FRESULT Read_File (char *name)
{
  /**** check whether the file exists or not ****/
  fresult = f_stat (name, &USBHfno);
  if (fresult != FR_OK)
  {
    char *buf = malloc(100*sizeof(char));
    sprintf (buf, "ERRROR!!! *%s* does not exists\n\n", name);
    
    sprintf(USB_Status_Buff,buf);
    free(buf);
    return fresult;
  }
  
  else
  {
    /* Open file to read */
    fresult = f_open(&USBHFile, name, FA_READ);
    
    if (fresult != FR_OK)
    {
      char *buf = malloc(100*sizeof(char));
      sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
      
      sprintf(USB_Status_Buff,buf);
      free(buf);
      return fresult;
    }
    /* Read data from the file
    * see the function details for the arguments */
    
    char *buf = malloc(100*sizeof(char));
    sprintf (buf, "Opening file-->  *%s*  To READ data from it\n", name);
    sprintf(USB_Status_Buff,buf);
    free(buf);
    
    char *buffer = malloc(sizeof(f_size(&USBHFile)));
    fresult = f_read (&USBHFile, buffer, f_size(&USBHFile), &br);
    if (fresult != FR_OK)
    {
      char *buf = malloc(100*sizeof(char));
      free(buffer);
      sprintf (buf, "ERROR!!! No. %d in reading file *%s*\n\n", fresult, name);
      
      sprintf(USB_Status_Buff,buffer);
      free(buf);
    }
    else
    {   
      sprintf(USB_Read_Data,buffer);
      free(buffer);
      
      /* Close file */
      fresult = f_close(&USBHFile);
      if (fresult != FR_OK)
      {
        char *buf = malloc(100*sizeof(char));
        sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
        sprintf(USB_Status_Buff,buf);
        Flag_FileReadFail = SET;
        free(buf);
      }
      else
      {
        char *buf = malloc(100*sizeof(char));
        sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
        sprintf(USB_Status_Buff,buf);
        Flag_FileReadFail = RESET;
        free(buf);
      }
    }
    return fresult;
  }
}

FRESULT Create_File (char *name)
{
	fresult = f_stat (name, &USBHfno);
	if (fresult == FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* already exists!!!!\n use Update_File \n\n",name);
		
		sprintf(USB_Status_Buff,buf);
		free(buf);
	    return fresult;
	}
	else
	{
		fresult = f_open(&USBHFile, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in creating file *%s*\n\n", fresult, name);
			
			sprintf(USB_Status_Buff,buf);
			free(buf);
		    return fresult;
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* created successfully\n Now use Write_File to write data\n",name);
			
			sprintf(USB_Status_Buff,buf);
			free(buf);
		}

		fresult = f_close(&USBHFile);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in closing file *%s*\n\n", fresult, name);
			
			sprintf(USB_Status_Buff,buf);
			free(buf);
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
			
			sprintf(USB_Status_Buff,buf);
			free(buf);
		}
	}
    return fresult;
}

FRESULT Update_File (char *name, char *data)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		
		sprintf(USB_Status_Buff,buf);
		free(buf);
	    return fresult;
	}

	else
	{
		 /* Create a file with read write access and open it */
	    fresult = f_open(&USBHFile, name, FA_OPEN_APPEND | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
			
	    	sprintf(USB_Status_Buff,buf);
	        free(buf);
	        return fresult;
	    }

    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "Opening file-->  *%s*  To UPDATE data in it\n", name);
		
    	sprintf(USB_Status_Buff,buf);
        free(buf);

	    /* Writing text */
	    fresult = f_write(&USBHFile, data, strlen (data), &bw);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in writing file *%s*\n\n", fresult, name);
			
	    	sprintf(USB_Status_Buff,buf);
	    	free(buf);
	    }

	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "*%s* UPDATED successfully\n", name);
			
	    	sprintf(USB_Status_Buff,buf);
	    	free(buf);
	    }

	    /* Close file */
	    fresult = f_close(&USBHFile);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
			
	    	sprintf(USB_Status_Buff,buf);
	    	free(buf);
	    }
	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
			
	    	sprintf(USB_Status_Buff,buf);
	    	free(buf);
	     }
	}
    return fresult;
}

FRESULT Remove_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		
		sprintf(USB_Status_Buff,buf);
		free(buf);
		return fresult;
	}

	else
	{
		fresult = f_unlink (name);
		if (fresult == FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* has been removed successfully\n\n", name);
			
			sprintf(USB_Status_Buff,buf);
			free(buf);
		}

		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in removing *%s*\n\n",fresult, name);
			
			sprintf(USB_Status_Buff,buf);
			free(buf);
		}
	}
	return fresult;
}

FRESULT Create_Dir (char *name)
{
    fresult = f_mkdir(name);
    if (fresult == FR_OK)
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "*%s* has been created successfully\n\n", name);
		
    	sprintf(USB_Status_Buff,buf);
    	free(buf);
    }
    else
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "ERROR No. %d in creating directory *%s*\n\n", fresult,name);
    	
    	free(buf);
    }
    return fresult;
}

void Check_USB_Details (void)
{
    /* Check free space */
    f_getfree("", &fre_clust, &pUSBHFatFS);

    total = (uint32_t)((pUSBHFatFS->n_fatent - 2) * pUSBHFatFS->csize * 0.5);
    char *buf = malloc(30*sizeof(char));
    sprintf (buf, "USB  Total Size: \t%lu\n",total);
    sprintf(USB_Status_Buff,buf);
    free(buf);
    free_space = (uint32_t)(fre_clust * pUSBHFatFS->csize * 0.5);
    buf = malloc(30*sizeof(char));
    sprintf (buf, "USB Free Space: \t%lu\n",free_space);
    sprintf(USB_Status_Buff,buf);
    free(buf);
}


/*******************************************************************************
 * FUNCTION NAME   : Data_Log
 * DESCRIPTION     : LOG DATA IN SD CARD
 * INPUT           : NONE
 * OUTPUT          : NONE
 * Example         : 
 *          Create_Dir("/DIR1");
 *          Create_File("/DIR1/DIR1FILE.txt");
 *          Write_File("/DIR1/DIR1FILE.txt", "This data should be in DIR1 file\n");
 *          Update_File("/DIR1/DIR1FILE.txt", "This data should be in new line in DIR1 file\n");
 * *******************************************************************************/

void Data_Log_Init(RTC_DateTypeDef RTC_DateStruct, RTC_TimeTypeDef RTC_TimeStruct)
{
    /* Create Directory */
    Create_Dir("/Data_Log");
    
    /* Open the file for writing */
    sprintf(Log_File_Name, "/Data_Log/Data_Log_%d%d%d_%d%d20%d.csv", RTC_DateStruct.Date, RTC_DateStruct.Month, RTC_DateStruct.Year, RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
    
    /* Create the file */
    Create_File(Log_File_Name);

    /* Write data to the file */
    Write_File(Log_File_Name, "SCANNER ID.,DATE, START TIME, END TIME, TOTAL TIME, RESULT, FAILED IDs\n");
}


/*******************************************************************************
 * FUNCTION NAME   : __Data_Update_Excle
 * DESCRIPTION     : UPDATE DATA IN EXCLE FILE
 * INPUT           : char *FileName, char *data
 * OUTPUT          : NONE
 * *******************************************************************************/
void __Data_Update_Excle(char *FileName, char *data)
{  
    /* Update the file */
    Update_File(FileName, data);
}