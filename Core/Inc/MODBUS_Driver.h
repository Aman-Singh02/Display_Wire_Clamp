/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODBUS_H
#define __MODBUS_H


#define MODBUS_UART                 USART3
#define MODBUS_Tx_EN                RS485_Toggle_GPIO_Port->BSRR = RS485_Toggle_Pin
#define MODBUS_Tx_DIS               RS485_Toggle_GPIO_Port->BSRR = (RS485_Toggle_Pin << 16)


extern uint8_t ByteMSB,ByteLSB ;

extern FlagStatus Flag_DataRdy,
                  Flag_BitRead, 
                  Flag_RegRead, 
                  Flag_RegWrite;

extern uint16_t len;
 
extern  volatile uint8_t  RcvCnt,
                          RcvState;
/* -------------------------------------------------------------------------- */

/* Buffers and flags */
extern uint8_t Modbus_RxBuffer[36], Modbus_TxBuffer[17] ;
extern volatile uint8_t RxIndex, TxIndex;
extern uint16_t HoldingRegisters[100], NumBytes; 
extern FlagStatus Flag_Transmit;

//extern FlagStatus Flag_Sample;
typedef enum 
{
    INIT_RCV,
    GET_FCODE,
    GET_STRT_ADDR,
    GET_LENGTH,
    GET_NBYTES,
    GET_DATA,
    GET_CRC
} MODBUS_RcvState_TypeDef;

typedef struct
{
    uint16_t SlaveId;
    uint8_t FunctionCode;
    uint16_t StartAddr;
    uint16_t NumBytes;
    char HRegister[9];
    uint16_t REQ_CRC;
} MODBUS_Request_TypeDef;

typedef struct
{
    uint16_t SlaveId;
    uint8_t FunctionCode;
    uint16_t StartAddr;
    uint16_t NumBytes;
    char HRegister[100];
    uint16_t RSP_CRC;
} MODBUS_Response_TypeDef;

extern MODBUS_RcvState_TypeDef MODBUS_STATE;
extern MODBUS_Request_TypeDef MODBUS_REQ;
extern MODBUS_Response_TypeDef MODBUS_RESP;

/* Function Prototypes */
void MODBUS_SendMsg(uint8_t *Tx_Header, uint8_t Len);
uint8_t *MODBUS_Request(uint16_t SlaveId, uint8_t FunctionCode, uint16_t StartAddr, uint16_t Req_Data_Size);
MODBUS_RcvState_TypeDef MODBUS_Respones(MODBUS_RcvState_TypeDef MODBUS_STATE, char RespData);
uint8_t *MODBUS_Reply(uint16_t SlaveId, uint8_t FunctionCode, uint16_t Data, uint16_t DataSize);
uint16_t MODBUS_CRC(uint8_t* data, uint16_t length);

#endif /* __MODBUS_H */