/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"
#include "GameCubeController.h"
uint64 readController(uint8 controller, uint8 rumble);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    char recvChar;
    char resultString[15];
    uint64_t controllerData;
    UART_Start();
    for(;;)
    {
        
        recvChar = UART_UartGetChar();
        if(recvChar == 'a') 
        {
            controllerData = readController(1,0);
            snprintf(resultString, sizeof(resultString), "%"PRIu64, controllerData);
            UART_UartPutString(resultString);
            UART_UartPutString("\r\n");
        }

        
    }
}

/* [] END OF FILE */
