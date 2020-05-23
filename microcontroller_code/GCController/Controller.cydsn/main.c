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
    char sendString[16];
    uint64_t controllerData;
    uint32_t firstPart; //most right bits
    uint32_t secondPart;//most left bits
    UART_Start();
    UART_UartPutString("Hello\r\n");
    controllerData = readController(1,0);
    for(;;)
    {
        
        recvChar = UART_UartGetChar();
        if(1) 
        {
            controllerData = readController(1,0);
            firstPart = controllerData & 0x00000000FFFFFFFF;
            secondPart = (controllerData >> 32) & 0x00000000FFFFFFFF;
            sprintf(sendString, "%08X", secondPart);
            sprintf(sendString, "%s%08X", sendString, firstPart);
            UART_UartPutString(sendString);
            UART_UartPutString("\r");
        }

        
    }
}

/* [] END OF FILE */
