/******************** (C) COPYRIGHT 2017 EDS INDIA ****************************
* File Name     : LCD_Function.h					      *
* Author        : DILEEP SINGH                                                *
* Date          : 22 JULY 2017                                                *
* Description   : This file provides all LCD Details                          *
* Revision	: SIMPEX FLASH LIGHT CONTROLLER REV0			      *
*******************************************************************************/

#ifndef  __LCDDrive_H
#define  __LCDDrive_H

#define DISPLAY_ON           Write_Instruction(0xaf)   //  Display on
#define DISPLAY_OFF          Write_Instruction(0xae)   //  Display off


void INIT_LCD(void);
void Display_MainScreen(void);
void Disp_BV(void);
void Disp_Temp(void);
void Disp_Time(void);
void Disp_Odo_Trip(void);
void Disp_GSMSignal(void);
void Disp_BattSOC(void);
void LCDWrite_Speed(void);
void Display_Diagnostic(void);
void Travel_Summary(void);
void Write_Instruction(unsigned char cmd);
void Write_Data(unsigned char );
void LCD_WriteData(unsigned char);
void Set_Page_Address(unsigned char );
void Set_Column_Address(unsigned char add);
void Connected_Device(void);
void Set_Contrast(unsigned char );

void WriteNum_2x18(uint8_t pageStart, uint8_t columnStart,uint8_t Data);

void LCDWrite_Text(unsigned char , unsigned char , char* );
void Write_Pixel(unsigned char col,unsigned char row);
void Clr_Pixel(unsigned char col,unsigned char row);
void Clear_Area(unsigned char CS, unsigned char RS, unsigned char CE, unsigned char RE);
void LCD_HDrawLine(unsigned char col, unsigned char row,unsigned char EndPoint);
void LCD_VDrawLine(unsigned char col, unsigned char row,unsigned char EndPoint);
extern unsigned char LCD_RAM[1025];

void Draw_Box_With_Gap(unsigned char start_col, unsigned char start_row, unsigned char width, unsigned char height, unsigned char gap_size, unsigned char box_count); 
void Clear_Inside_Area(
    unsigned char start_col, 
    unsigned char start_row, 
    unsigned char width, 
    unsigned char height, 
    unsigned char gap_size, 
    unsigned char box_count
);

// COMANDS
#define SET_LCD_PS        (LCD_PS_GPIO_Port->BSRR = LCD_PS_Pin)
#define RST_LCD_PS        (LCD_PS_GPIO_Port->BSRR = (LCD_PS_Pin << 16U))

#define SET_LCD_RD        (LCD_RD_GPIO_Port->BSRR = LCD_RD_Pin)
#define RST_LCD_RD        (LCD_RD_GPIO_Port->BSRR = (LCD_RD_Pin << 16U))

#define SET_LCD_WR        (LCD_WR_GPIO_Port->BSRR = LCD_WR_Pin)
#define RST_LCD_WR        (LCD_WR_GPIO_Port->BSRR = (LCD_WR_Pin << 16U))

#define SET_LCD_RS        (LCD_RS_GPIO_Port->BSRR = LCD_RS_Pin)
#define RST_LCD_RS        (LCD_RS_GPIO_Port->BSRR = (LCD_RS_Pin << 16U))

#define SET_LCD_RES       (LCD_RES_GPIO_Port->BSRR = LCD_RES_Pin)
#define RST_LCD_RES       (LCD_RES_GPIO_Port->BSRR = (LCD_RES_Pin << 16U))

#define SET_LCD_CS        (LCD_CS_GPIO_Port->BSRR = LCD_CS_Pin)
#define RST_LCD_CS        (LCD_CS_GPIO_Port->BSRR = (LCD_CS_Pin << 16U))

// Data Lines
#define SET_LCD_D7        (LCD_DB0_GPIO_Port->BSRR = LCD_DB0_Pin)
#define RST_LCD_D7        (LCD_DB0_GPIO_Port->BSRR = (LCD_DB0_Pin << 16U))

#define SET_LCD_D6        (LCD_DB1_GPIO_Port->BSRR = LCD_DB1_Pin)
#define RST_LCD_D6        (LCD_DB1_GPIO_Port->BSRR = (LCD_DB1_Pin << 16U))

#define SET_LCD_D5        (LCD_DB2_GPIO_Port->BSRR = LCD_DB2_Pin)
#define RST_LCD_D5        (LCD_DB2_GPIO_Port->BSRR = (LCD_DB2_Pin << 16U))

#define SET_LCD_D4        (LCD_DB3_GPIO_Port->BSRR = LCD_DB3_Pin)
#define RST_LCD_D4        (LCD_DB3_GPIO_Port->BSRR = (LCD_DB3_Pin << 16U))

#define SET_LCD_D3        (LCD_DB4_GPIO_Port->BSRR = LCD_DB4_Pin)
#define RST_LCD_D3        (LCD_DB4_GPIO_Port->BSRR = (LCD_DB4_Pin << 16U))

#define SET_LCD_D2        (LCD_DB5_GPIO_Port->BSRR = LCD_DB5_Pin)
#define RST_LCD_D2        (LCD_DB5_GPIO_Port->BSRR = (LCD_DB5_Pin << 16U))

#define SET_LCD_D1        (LCD_DB6_GPIO_Port->BSRR = LCD_DB6_Pin)
#define RST_LCD_D1        (LCD_DB6_GPIO_Port->BSRR = (LCD_DB6_Pin << 16U))

#define SET_LCD_D0        (LCD_DB7_GPIO_Port->BSRR = LCD_DB7_Pin)
#define RST_LCD_D0        (LCD_DB7_GPIO_Port->BSRR = (LCD_DB7_Pin << 16U))

#define OFF             0
#define ON              1

#endif
