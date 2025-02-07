#include "main.h"


/****************************
*Get SDCARD Status
*****************************/
void sdCard_getdat()
{
        // Wait for Micro SD module initialize 
        fres=f_mount(&fs,"1:", 1);  
        if( fres == FR_OK)                                                      //micro sd card mounted sucessfully  
        {                               
            fres=f_getfree("1:", &fre_clust, &pfs);            // Get total and free clust
            blk_no=(uint64_t)(pfs->n_fatent - 2);              //973'584 unit numbers
            blksize= (uint64_t)(pfs->csize * 0.5 * 1024 );     //32 KBYTE(32*1024=32768 byte)
            totalSpace = (uint64_t)(blk_no*blksize);         // 973'584 * 32kb(32*1024=32768 byte) = 32GBYTE
            freeSpace = (uint64_t)(fre_clust * pfs->csize * 0.5 * 1024);    //data size in byte
            SD_DATSz=totalSpace-freeSpace;                     //data size in byte
           
            //Getting Sector Size
            fres=f_getfree("1:", &free_clusters, &getFreeFs);
            total_sectors = (uint32_t)((getFreeFs->n_fatent - 2) * getFreeFs->csize);
            free_sectors = (uint32_t)(free_clusters * getFreeFs->csize); 
            dat_sectors=total_sectors-free_sectors;                                                                       // SD_disk_ioctl(0,GET_SECTOR_COUNT,&Settori);
            
          }
          else if ( fres != FR_OK) {  //micro sd card mount error    //_Error_Handler(__FILE__, __LINE__); 
                                   }
        
          // Close file 
          fres=f_close(&fil);
          if(fres == FR_OK) {  //File Closed Sucessfully
                            }
          else  if( fres != FR_OK) {//file was not closed!     //_Error_Handler(__FILE__, __LINE__);
                                   }

	   // Unmount SDCARD 
           fres=f_mount(NULL, "1:", 1);
           if(fres == FR_OK) {//Micro SD cARD IS UNMOUNTEFD
                             }
           else  if( fres != FR_OK) {//Micro SD cARD WAS NOT UNMOUNTEFD
                                    }
           
}

