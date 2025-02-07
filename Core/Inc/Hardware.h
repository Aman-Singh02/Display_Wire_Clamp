#ifndef __HARDWARE_H
#define __HARDWARE_H

/* Ethernet Pins */
#define ETH_CRS_Pin                 LL_GPIO_PIN_0                 // Carrier Sense for Ethernet
#define ETH_CRS_GPIO_Port           GPIOA                   // Ethernet CRS connected to GPIOA
#define ETH_COL_Pin                 LL_GPIO_PIN_3                 // Collision Detect for Ethernet
#define ETH_COL_GPIO_Port           GPIOA                   // Ethernet COL connected to GPIOA

/* Switch Pins */
#define SW1_Pin                     LL_GPIO_PIN_4                     // Switch 1 Pin
#define SW1_GPIO_Port               GPIOA                       // Switch 1 Port connected to GPIOA
#define SW2_Pin                     LL_GPIO_PIN_5                     // Switch 2 Pin
#define SW2_GPIO_Port               GPIOA                       // Switch 2 Port connected to GPIOA
#define SW3_Pin                     LL_GPIO_PIN_6                     // Switch 3 Pin
#define SW3_GPIO_Port               GPIOA                       // Switch 3 Port connected to GPIOA
#define SW4_Pin                     LL_GPIO_PIN_15                    // Switch 4 Pin
#define SW4_GPIO_Port               GPIOB                       // Switch 4 Port connected to GPIOB

/* LCD Pins */
#define LCD_RS_Pin                  LL_GPIO_PIN_2                  // LCD Register Select Pin
#define LCD_RS_GPIO_Port            GPIOB                    // LCD RS connected to GPIOB
#define LCD_DB7_Pin                 LL_GPIO_PIN_7                 // LCD Data Bit 7
#define LCD_DB7_GPIO_Port           GPIOE                   // LCD DB7 connected to GPIOE
#define LCD_DB6_Pin                 LL_GPIO_PIN_8                 // LCD Data Bit 6
#define LCD_DB6_GPIO_Port           GPIOE                   // LCD DB6 connected to GPIOE
#define LCD_DB5_Pin                 LL_GPIO_PIN_9                 // LCD Data Bit 5
#define LCD_DB5_GPIO_Port           GPIOE                   // LCD DB5 connected to GPIOE
#define LCD_DB4_Pin                 LL_GPIO_PIN_10                // LCD Data Bit 4
#define LCD_DB4_GPIO_Port           GPIOE                   // LCD DB4 connected to GPIOE
#define LCD_DB3_Pin                 LL_GPIO_PIN_11                // LCD Data Bit 3
#define LCD_DB3_GPIO_Port           GPIOE                   // LCD DB3 connected to GPIOE
#define LCD_DB2_Pin                 LL_GPIO_PIN_12                // LCD Data Bit 2
#define LCD_DB2_GPIO_Port           GPIOE                   // LCD DB2 connected to GPIOE
#define LCD_DB1_Pin                 LL_GPIO_PIN_13                // LCD Data Bit 1
#define LCD_DB1_GPIO_Port           GPIOE                   // LCD DB1 connected to GPIOE
#define LCD_DB0_Pin                 LL_GPIO_PIN_14                // LCD Data Bit 0
#define LCD_DB0_GPIO_Port           GPIOE                   // LCD DB0 connected to GPIOE
#define LCD_RES_Pin                 LL_GPIO_PIN_15                // LCD Reset Pin
#define LCD_RES_GPIO_Port           GPIOE                   // LCD RES connected to GPIOE

/* RS485 Transmit Pin */
#define RS485_Toggle_Pin            LL_GPIO_PIN_10           // RS485 Transmit Toggle Pin
#define RS485_Toggle_GPIO_Port      GPIOD              // RS485 Toggle connected to GPIOD

/* DIP Switch Pins */
#define DIP_SW0_Pin                 LL_GPIO_PIN_11                // DIP Switch 0 Pin
#define DIP_SW0_GPIO_Port           GPIOD                   // DIP Switch 0 Port connected to GPIOD
#define DIP_SW1_Pin                 LL_GPIO_PIN_12                // DIP Switch 1 Pin
#define DIP_SW1_GPIO_Port           GPIOD                   // DIP Switch 1 Port connected to GPIOD
#define DIP_SW2_Pin                 LL_GPIO_PIN_13                // DIP Switch 2 Pin
#define DIP_SW2_GPIO_Port           GPIOD                   // DIP Switch 2 Port connected to GPIOD
#define DIP_SW3_Pin                 LL_GPIO_PIN_14                // DIP Switch 3 Pin
#define DIP_SW3_GPIO_Port           GPIOD                   // DIP Switch 3 Port connected to GPIOD
#define DIP_SW4_Pin                 LL_GPIO_PIN_15                // DIP Switch 4 Pin
#define DIP_SW4_GPIO_Port           GPIOD                   // DIP Switch 4 Port connected to GPIOD
#define DIP_SW5_Pin                 LL_GPIO_PIN_6                 // DIP Switch 5 Pin
#define DIP_SW5_GPIO_Port           GPIOC                   // DIP Switch 5 Port connected to GPIOC
#define DIP_SW6_Pin                 LL_GPIO_PIN_7                 // DIP Switch 6 Pin
#define DIP_SW6_GPIO_Port           GPIOC                   // DIP Switch 6 Port connected to GPIOC
#define DIP_SW7_Pin                 LL_GPIO_PIN_8                 // DIP Switch 7 Pin
#define DIP_SW7_GPIO_Port           GPIOC                   // DIP Switch 7 Port connected to GPIOC

/* Ethernet Management Clock Output Pin */
#define ETH_MCO_Pin                 LL_GPIO_PIN_8                 // Ethernet Management Clock Output Pin
#define ETH_MCO_GPIO_Port           GPIOA                   // Ethernet MCO connected to GPIOA

/* SPI3 Chip Select Pin */
#define SPI3_CS_Pin                 LL_GPIO_PIN_15                // SPI3 Chip Select Pin
#define SPI3_CS_GPIO_Port           GPIOA                   // SPI3 CS connected to GPIOA

/* LCD Control Pins */
#define LCD_VCC_ON_Pin              LL_GPIO_PIN_0              // LCD VCC Power On Pin
#define LCD_VCC_ON_GPIO_Port        GPIOD                // LCD VCC On connected to GPIOD
#define LCD_CS_Pin                  LL_GPIO_PIN_1                  // LCD Chip Select Pin
#define LCD_CS_GPIO_Port            GPIOD                    // LCD CS connected to GPIOD
#define LCD_RD_Pin                  LL_GPIO_PIN_2                  // LCD Read Pin
#define LCD_RD_GPIO_Port            GPIOD                    // LCD RD connected to GPIOD
#define LCD_WR_Pin                  LL_GPIO_PIN_3                  // LCD Write Pin
#define LCD_WR_GPIO_Port            GPIOD                    // LCD WR connected to GPIOD
#define LCD_PS_Pin                  LL_GPIO_PIN_4                  // LCD Power Supply Pin
#define LCD_PS_GPIO_Port            GPIOD                    // LCD PS connected to GPIOD

/* RF Module Pins */
#define RFM_CSN_Pin                 LL_GPIO_PIN_6                 // RF Module Chip Select Pin
#define RFM_CSN_GPIO_Port           GPIOD                   // RF CS connected to GPIOD
#define RFM_CE_Pin                  LL_GPIO_PIN_7                  // RF Module Chip Enable Pin
#define RFM_CE_GPIO_Port            GPIOD                    // RF CE connected to GPIOD
#define RFM_SCK_Pin                 LL_GPIO_PIN_3                 // RF Module SPI Clock Pin
#define RFM_SCK_GPIO_Port           GPIOB                   // RF SCK connected to GPIOB
#define RFM_MISO_Pin                LL_GPIO_PIN_4                // RF Module SPI MISO Pin
#define RFM_MISO_GPIO_Port          GPIOB                  // RF MISO connected to GPIOB
#define RFM_MOSI_Pin                LL_GPIO_PIN_5                // RF Module SPI MOSI Pin
#define RFM_MOSI_GPIO_Port          GPIOB                  // RF MOSI connected to GPIOB

/* LED Pins */
#define LED_PWR_Pin                 LL_GPIO_PIN_9                 // Power LED Pin
#define LED_PWR_GPIO_Port           GPIOB                   // Power LED connected to GPIOB
#define LED_SYNC_Pin                LL_GPIO_PIN_0                // Sync LED Pin
#define LED_SYNC_GPIO_Port          GPIOE                  // Sync LED connected to GPIOE
#define LED_CON_Pin                 LL_GPIO_PIN_1                 // Connection LED Pin
#define LED_CON_GPIO_Port           GPIOE                   // Connection LED connected to GPIOE

#endif /* __HARDWARE_H */