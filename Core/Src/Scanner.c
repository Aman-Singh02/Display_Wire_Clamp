#include "main.h"


void Scanner_PutStr(char *Str)
{
    /* Clear the buffer */
    memset(Scanner_Buf, 0, sizeof(Scanner_Buf));

    /* Add a small delay for stabilization (if necessary) */
    LL_mDelay(10);

    /* Transmit the string via USART */
    while (*Str != '\0') /* Transmit until the null character */
    {
        /* Wait until the Transmit Data Register (TDR) is empty */
        while (!LL_USART_IsActiveFlag_TXE(USART1));

        /* Write the character to the USART transmit data register */
        LL_USART_TransmitData8(USART1, *Str);

        /* Increment the string pointer to send the next character */
        Str++;

        /* Small delay to ensure transmission stability */
        LL_mDelay(2);
    }

    /* Ensure the last transmission is complete before exiting */
    while (!LL_USART_IsActiveFlag_TC(USART1));

    /* Additional stabilization delay if necessary */
    LL_mDelay(5);
}
