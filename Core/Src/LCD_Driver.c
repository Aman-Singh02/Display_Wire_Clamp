/******************** (C) COPYRIGHT 2017 EDS INDIA **************************************************
* File Name     : LCD_Function.c					                            *
* Author        : DILEEP SINGH                                                                      *
* Date          : 22 JULY 2017                                                                      *
* Description   : This file Define all Variables & functions used for simpex LCD                    *
* Revision	: SIMPEX FLASH LIGHT CONTROLLER REV0                                                *
*                                                                                                   * 
*****************************************************************************************************/
//#include "app_include.h"
#include "Main.h"
#include "Bitmap_ROM.h"


unsigned char LCD_RAM[1025];
FlagStatus Flag_OutLine;// Flag_Inverse;

/*******************************************************************************
* Function Name  : Write_Instruction
* Description    : Write Instrictution to LCD 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Write_Instruction(unsigned char cmd)
{
    SET_LCD_RD;
    RST_LCD_RS;                                                                 // Command
    RST_LCD_WR;                                                                 // Write Enable
    LCD_WriteData(cmd);                                                         // Write Data
    SET_LCD_WR;
    SET_LCD_RS;
 }
/*******************************************************************************
* Function Name  : Write_Data
* Description    : Write Data to LCD 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Write_Data(unsigned char dat) 
{
  
  
    SET_LCD_RD;
    SET_LCD_RS;                                                                 // Command
    RST_LCD_WR;                                                                 // Write Enable
    LCD_WriteData(dat);                                                         // Write Data
    SET_LCD_WR;
    SET_LCD_RS;

}
/*******************************************************************************
* Function Name  : Write_Pixel
* Description    : Write_Pixel  to LCD 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Write_Pixel(unsigned char col,unsigned char row)
{
  unsigned char pixel=1, page;
  unsigned short RAM_LOC;
  
  page = row/8;                                                                 // col height decides page address 
  pixel =  pixel << row%8;
  RAM_LOC = col + (row / 8)*128;
    
  
  LCD_RAM[RAM_LOC] |= pixel;
  Set_Page_Address(page); 
  Set_Column_Address(col + 4);                                                  // 
  
    SET_LCD_RD;
    SET_LCD_RS;                                                                 // Command
    RST_LCD_WR;                                                                 // Write Enable
    if (LCD_RAM[RAM_LOC] & 0x01) SET_LCD_D0; else RST_LCD_D0; 
    if (LCD_RAM[RAM_LOC] & 0x02) SET_LCD_D1; else RST_LCD_D1;
    if (LCD_RAM[RAM_LOC] & 0x04) SET_LCD_D2; else RST_LCD_D2;
    if (LCD_RAM[RAM_LOC] & 0x08) SET_LCD_D3; else RST_LCD_D3;
    if (LCD_RAM[RAM_LOC] & 0x10) SET_LCD_D4; else RST_LCD_D4;
    if (LCD_RAM[RAM_LOC] & 0x20) SET_LCD_D5; else RST_LCD_D5;
    if (LCD_RAM[RAM_LOC] & 0x40) SET_LCD_D6; else RST_LCD_D6;
    if (LCD_RAM[RAM_LOC] & 0x80) SET_LCD_D7; else RST_LCD_D7; 
    SET_LCD_WR;
    SET_LCD_RS;
    

}

/*******************************************************************************
* Function Name  : Clr_Pixel
* Description    : Write Data to LCD 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Clr_Pixel(unsigned char col,unsigned char row)
{
  unsigned char pixel=1, page;
  unsigned short RAM_LOC;
  
  page = row/8;                                                                 // col height decides page address 
  pixel =  pixel << row%8;
  
  pixel ^= 0xFF; 
  RAM_LOC = col + (row / 8)*128;
    
 
  LCD_RAM[RAM_LOC] &= pixel;
  Set_Page_Address(page); 
  Set_Column_Address(col + 4);                                                  // 
  
  SET_LCD_RD;
  SET_LCD_RS;                                                                   // Command
  RST_LCD_WR;                                                                   // Write Enable
    
 
  if (LCD_RAM[RAM_LOC] & 0x01) SET_LCD_D0; else RST_LCD_D0; 
  if (LCD_RAM[RAM_LOC] & 0x02) SET_LCD_D1; else RST_LCD_D1;
  if (LCD_RAM[RAM_LOC] & 0x04) SET_LCD_D2; else RST_LCD_D2;
  if (LCD_RAM[RAM_LOC] & 0x08) SET_LCD_D3; else RST_LCD_D3;
  if (LCD_RAM[RAM_LOC] & 0x10) SET_LCD_D4; else RST_LCD_D4;
  if (LCD_RAM[RAM_LOC] & 0x20) SET_LCD_D5; else RST_LCD_D5;
  if (LCD_RAM[RAM_LOC] & 0x40) SET_LCD_D6; else RST_LCD_D6;
  if (LCD_RAM[RAM_LOC] & 0x80) SET_LCD_D7; else RST_LCD_D7;
  
  SET_LCD_WR;
  SET_LCD_RS;
}
/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : Write Data to LCD 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteData(unsigned char val)
{
     if (val & 0x01) SET_LCD_D0; else RST_LCD_D0; 
     if (val & 0x02) SET_LCD_D1; else RST_LCD_D1;
     if (val & 0x04) SET_LCD_D2; else RST_LCD_D2;
     if (val & 0x08) SET_LCD_D3; else RST_LCD_D3;
     if (val & 0x10) SET_LCD_D4; else RST_LCD_D4;
     if (val & 0x20) SET_LCD_D5; else RST_LCD_D5;
     if (val & 0x40) SET_LCD_D6; else RST_LCD_D6;
     if (val & 0x80) SET_LCD_D7; else RST_LCD_D7;
}

/*******************************************************************************
* Function Name  : Set_Column_Address
* Description    : Display Line
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// Set page address 0~63
void Set_Column_Address(unsigned char add)
{
    Write_Instruction((0x10|(add>>4)));
    Write_Instruction((0x0f&add));
}

/*******************************************************************************
* Function Name  : Set_Page_Address
* Description    : Page Number
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

// Set page address 0~7
void Set_Page_Address(unsigned char add)
{
    add=0xb0|add;
    Write_Instruction(add);
}

/*******************************************************************************
* Function Name  : Set_Contrast
* Description    : Contrast Level
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Set_Contrast(unsigned char Const_Level)
{
    Write_Instruction(0x81);
    Write_Instruction(Const_Level+28);	
}
/*******************************************************************************
* Function Name  : INIT_LCD
* Description    : Initialize LCD , clear Display & Display Logo
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void INIT_LCD(void)
{  
  int i,j;
 
  SET_LCD_PS;                                                                   // PARALLEL DATA 
  Write_Instruction(0xE2);                                                      // WRITE INSTRUCTION - RESET 
  
  SET_LCD_RES;                                                                  // LCD RESET SEQUENCE
  LL_mDelay(100);                                                              // 100 MILI SECOND DELAY
  RST_LCD_RES;                                                                  // 
  LL_mDelay(100);
  SET_LCD_RES;
  LL_mDelay(200);
  RST_LCD_CS;                                                                   // Chip Select
  
  Write_Instruction(0xA2);                                                      // LCD BIAS TO 1/9TH                                                  
  Write_Instruction(0xA1);                                                      // HORIZONTALLY REVERSE L->H
  Write_Instruction(0xC0);                                                      // VERTICALLY REVERSE T->D
  Write_Instruction(0x24);                                                      // REGULATOR SELECT / INTERNAL RESISTER DIVIDER 
  Write_Instruction(0x2F);                                                      // POWER CONTROL ALL INTERNAL BLOCKS ON
  Set_Contrast(10);                                                             // SET CONTRSAT LEVEL 
  Write_Instruction(0x40);                                                      // GOTO TOP LEFT OF DISPLAY
 
 // Euler Logo
 unsigned short Cnt = 0, RNo, Loc;
 unsigned char Disp_Mult[] =  { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
 unsigned char DataVal[8];
 for(int Ln = 7; Ln >= 0; Ln--)
 {
  for (int L = 0; L < 16; L++)
  {
    RNo = 0;
    for (int r = 0; r <= 7; r++)
    {
        Loc = (r * 16) + L + (Ln*128);
        DataVal[RNo] = MOTHERSON[Loc];
        RNo++;
    }

    for (int row = 0; row < 8; row++)
      {
          for (int n = 0; n < 8; n++)
          {
              unsigned char Val = DataVal[n];
              if ((Val & Disp_Mult[row]) == Disp_Mult[row])
                  LCD_RAM[Cnt] = (unsigned char)(LCD_RAM[Cnt] << 1);
              else
                  LCD_RAM[Cnt] = (unsigned char)((unsigned char)(LCD_RAM[Cnt] << 1) | 0x01);
          }
          Cnt++;
      }
    
 }

 }

  Cnt = 0;
  for(i=0;i<=7;i++)
  {
    Set_Page_Address(i);
    Set_Column_Address(0x04);
    for(j=0;j<128;j++, Cnt++)                                                       // AS IT IS WORKING 
    {
      Write_Data(LCD_RAM[Cnt]);
    }
  }
   
  
}
  
/*******************************************************************************
* Function Name  : Display_MainScreen
* Description    : Display The Default Screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Display_MainScreen(void)
{
   // Euler Logo
 memset(LCD_RAM,0,1024);
 unsigned short Cnt = 0, RNo, Loc;
 unsigned char Disp_Mult[] =  { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
 unsigned char DataVal[8];
 for(int Ln = 7; Ln >= 0; Ln--)
 {
    for (int L = 0; L < 16; L++)
    {
      RNo = 0;
      for (int r = 0; r <= 7; r++)
      {
          Loc = (r * 16) + L + (Ln*128);
          DataVal[RNo] = Main[Loc];
          RNo++;
      }

      for (int row = 0; row < 8; row++)
        {
            for (int n = 0; n < 8; n++)
            {
                unsigned char Val = DataVal[n];
                if ((Val & Disp_Mult[row]) == Disp_Mult[row])
                    LCD_RAM[Cnt] = (unsigned char)((unsigned char)(LCD_RAM[Cnt] << 1) | 0x01);
                else
                    LCD_RAM[Cnt] = (unsigned char)(LCD_RAM[Cnt] << 1);
            }
            Cnt++;
        }
      
   }

 }

  Cnt = 0;
  for(int i=0;i<=7;i++)
  {
    Set_Page_Address(i);
    Set_Column_Address(0x04);
    for(int j=0;j<128;j++, Cnt++)                                                       // AS IT IS WORKING 
    {
      Write_Data(LCD_RAM[Cnt]);
    }
  }
  
}

void WriteNum_2x18(uint8_t pageStart, uint8_t columnStart,uint8_t Data)
{
  int Cnt = 0, C = 0; int Col;
  
  for (int R = pageStart; R < (pageStart + 3); R++)
  {
    Set_Page_Address(R);
    Set_Column_Address(columnStart);
    for(Cnt = C; Cnt < (18 + Col); Cnt++)
    {
      Write_Data(Eng_5x16[Data-48][Cnt]);
    }

    C = Cnt+1;
    Col = Cnt;
  }
}
/*******************************************************************************
* Function Name  : LCD_HDrawLine
* Description    : Draw Hporizontal Line on the LCD
* Input          : Location
* Output         : Byte
* Return         : None
*******************************************************************************/
void LCD_HDrawLine(unsigned char col, unsigned char row,unsigned char EndPoint)  // X,Y   -> TOP LEFT 
{
  unsigned char loop;
  for(loop = col; loop < EndPoint; loop++){Write_Pixel(loop,row); if(Flag_OutLine)loop++; }                // COLUMN LIES ON X AXIS 
}

/*******************************************************************************
* Function Name  : LCD_VDrawLine
* Description    : Draw Vertical Line on the LCD
* Input          : Location
* Output         : Byte
* Return         : None
*******************************************************************************/

void LCD_VDrawLine(unsigned char col, unsigned char row,unsigned char EndPoint)
{
  
  unsigned char loop;
  for(loop = row; loop < EndPoint; loop++){Write_Pixel(col,loop);if(Flag_OutLine)loop++; } 
}

/*******************************************************************************
* Function Name  : LCD_WriteText Row scanning wise
* Description    : Write TExt on LCD Screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCDWrite_Text(unsigned char col,unsigned char row,char* buffer)
{ 
 unsigned char offset,Data2Disp,Font_Size,bit_count=0,x,y;  
 unsigned char Wr_Char_LOW,Wr_Char_High,loop ;        
  x = col;                                            //COLOMN Value where to print 
  y = row;                                            //ROW VALUE WHERE TO PRINT THE CHAR 
  
  while(*buffer != '\0')                              //RUN LOOP  TILL 0
  {
     Data2Disp = *buffer - 32;                           //SAPRATE VALUE OF THE CHAR FOR THE ARRAY 
    
     offset = Eng_7x5_ChrRom[Data2Disp][0];Font_Size = 6; 

     for(loop = offset;loop < Font_Size; loop++) 
     {   
       Wr_Char_LOW  = (Eng_7x5_ChrRom[Data2Disp][loop] & 0xFF);  //Take LSB from the array
       Wr_Char_High = (Eng_7x5_ChrRom[Data2Disp][loop] >> 8 );   //Take MSB bit the array
     
       bit_count = 16; //bcz we 16 bit character
       while(bit_count)
       {
         if(Flag_Inverse)
         {
            if((Wr_Char_LOW & 0x01)== 0x01)Clr_Pixel(x,y+8);                   //PRINT THE LSB DATA ON THE DISPLAY
            if((Wr_Char_High & 0x01)== 0x01)Clr_Pixel(x,y);                    //PRINT THE MSB DATA THE DISPLAY         
         }
         else
         {
            if((Wr_Char_LOW & 0x01)== 0x01)Write_Pixel(x,y+8);                   //PRINT THE LSB DATA ON THE DISPLAY
            if((Wr_Char_High & 0x01)== 0x01)Write_Pixel(x,y);                    //PRINT THE MSB DATA THE DISPLAY        
         }
        
          Wr_Char_LOW  = Wr_Char_LOW >> 1;                 //SHIFT DATA ONE BY ONE
          Wr_Char_High = Wr_Char_High >>1;                 //SHIFT DATA ONE BY ONE
          y--;//right
          bit_count--;
       }
       x++;y=row;
       
    }//end for loop
    
    x++;
    buffer++;
    
  }//end while loop
}//end function

/*******************************************************************************
* Function Name  : Clear_Area
* Description    : Clear specified area from lcd memory
* Input          : co-ordinates
* Output         : None
* Return         : None
*******************************************************************************/
void Clear_Area(unsigned char CS, unsigned char RS, unsigned char CE, unsigned char RE)
{
  unsigned char x,y;
  
  for(x = CS; x <= CE; x++)
  {
    if(Flag_Inverse)
      for(y=RS; y<=RE; y++)Write_Pixel(x,y);
    else
      for(y=RS; y<=RE; y++)Clr_Pixel(x,y);
  }
}


void Draw_Box_With_Gap(unsigned char start_col, unsigned char start_row, unsigned char width, unsigned char height, unsigned char gap_size, unsigned char box_count) 

{
    unsigned char box, x, y, pixel, page;
    unsigned short RAM_LOC;

    for (box = 0; box < box_count; box++) { // Loop for multiple boxes
        for (y = start_row; y < start_row + height; y++) { // Vertical pixels
            page = y / 8;  // Calculate the page for the row
            pixel = 1 << (y % 8);

            for (x = start_col + box * (width + gap_size); x < start_col + box * (width + gap_size) + width; x++) { // Horizontal pixels
                RAM_LOC = x + (page * 128); // Calculate memory location
                LCD_RAM[RAM_LOC] |= pixel; // Set the pixel in RAM

                // Update LCD
                Set_Page_Address(page);
                Set_Column_Address(x + 4);

                SET_LCD_RD;
                SET_LCD_RS;
                RST_LCD_WR;

                // Write pixel data
                if (LCD_RAM[RAM_LOC] & 0x01) SET_LCD_D0; else RST_LCD_D0;
                if (LCD_RAM[RAM_LOC] & 0x02) SET_LCD_D1; else RST_LCD_D1;
                if (LCD_RAM[RAM_LOC] & 0x04) SET_LCD_D2; else RST_LCD_D2;
                if (LCD_RAM[RAM_LOC] & 0x08) SET_LCD_D3; else RST_LCD_D3;
                if (LCD_RAM[RAM_LOC] & 0x10) SET_LCD_D4; else RST_LCD_D4;
                if (LCD_RAM[RAM_LOC] & 0x20) SET_LCD_D5; else RST_LCD_D5;
                if (LCD_RAM[RAM_LOC] & 0x40) SET_LCD_D6; else RST_LCD_D6;
                if (LCD_RAM[RAM_LOC] & 0x80) SET_LCD_D7; else RST_LCD_D7;

                SET_LCD_WR;
                SET_LCD_RS;
            }
        }
    }
}


void Clear_Inside_Area(
    unsigned char start_col, 
    unsigned char start_row, 
    unsigned char width, 
    unsigned char height, 
    unsigned char gap_size, 
    unsigned char box_count
) {
    unsigned char box, x, y;

    for (box = 0; box < box_count; box++) {
        // Calculate box start and end coordinates
        unsigned char inner_start_col = start_col + box * (width + gap_size) + 1;       // Exclude outermost column
        unsigned char inner_start_row = start_row + 1;                                  // Exclude outermost row
        unsigned char inner_end_col = start_col + box * (width + gap_size) + width - 2; // Exclude outermost column
        unsigned char inner_end_row = start_row + height - 2;                            // Exclude outermost row

        // Clear the inner area
        for (y = inner_start_row; y <= inner_end_row; y++) {
            for (x = inner_start_col; x <= inner_end_col; x++) {
                // Skip clearing the edge pixels
                if (
                    x == inner_start_col || x == inner_end_col || // Left and right edges
                    y == inner_start_row || y == inner_end_row    // Top and bottom edges
                ) {
                    continue;
                }

                // Clear pixel inside the area
               Clr_Pixel(x, y);
            }
        }
    }
}
