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
    _Bool rumble = 0;
    uint64_t controllerData;
    uint32_t firstPart; //most right bits
    uint32_t secondPart;//most left bits
    UART_Start();
    for(;;)
    {
        recvChar = UART_UartGetChar();
        if(recvChar != 0)
        {
            if(recvChar == 'g') //just send data
            {
                controllerData = readController(1,0);
                readController(1,rumble);
                
                //send data
                firstPart = controllerData & 0x00000000FFFFFFFF;
                secondPart = (controllerData >> 32) & 0x00000000FFFFFFFF;
                sprintf(sendString, "%08X", secondPart);
                sprintf(sendString, "%s%08X", sendString, firstPart);
                UART_UartPutString(sendString);
                UART_UartPutString("\r");
            }
            if(recvChar == 'r') //rumble
            {
                rumble = 1;
                readController(1,rumble);
            }
            if(recvChar == 'n')
            {
                rumble = 0;
                readController(1,rumble);
            }
            if(recvChar == 't')
            {
                UART_UartPutString("GC_Contr\r\n");
                
            }

        }   
    }
}

/* [] END OF FILE */
