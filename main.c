/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC 4 - UART Transmit
*              and Receive Application for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2020-2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
******************************************************************************/

/******************************************************************************
 * Include header files
 *****************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"

/******************************************************************************
* Global Variables
******************************************************************************/
cy_stc_scb_uart_context_t CYBSP_UART_context;

/******************************************************************************
* Function Name: handle_error
*******************************************************************************
* Summary:
* User defined error handling function.
*
******************************************************************************/
void handle_error(void)
{
    /* Disable all interrupts */
    __disable_irq();

    /* Switch On error LED */
    Cy_GPIO_Write(CYBSP_LED1_PORT, CYBSP_LED1_NUM, CYBSP_LED_STATE_ON);
    while(1){}
}

/******************************************************************************
* Function Name: main
*******************************************************************************
* Summary:
* This is the main function for CM4 CPU.
* Reads one byte from the serial terminal and echoes back the read byte.
*
* Parameters:
*  void
*
* Return:
*  int
*
******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    uint32_t read_data;
    cy_en_scb_uart_status_t initstatus;

    /* Turn off the USER LED */
    Cy_GPIO_Write(CYBSP_LED1_PORT, CYBSP_LED1_NUM, CYBSP_LED_STATE_OFF);

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if(result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize the UART */
    initstatus = Cy_SCB_UART_Init(CYBSP_UART_HW, &CYBSP_UART_config, &CYBSP_UART_context);

    /* Initialization failed. Handle error */
    if(initstatus != CY_SCB_UART_SUCCESS)
    {
        handle_error();
    }

    /* Enable global interrupts */
    __enable_irq();

    Cy_SCB_UART_Enable(CYBSP_UART_HW);

    /* Transmit Header to the Terminal */
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "\x1b[2J\x1b[;H");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "***********************************************************\r\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "PSOC 4 MCU UART transmit and receive\r\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "***********************************************************\r\n\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, ">> Start typing to see the echo on the screen \r\n\n");

    for(;;)
    {
        /* Check if there is a received character from user console */
        if(0UL != Cy_SCB_UART_GetNumInRxFifo(CYBSP_UART_HW))
        {
            /* Re-transmit whatever the user types on the console */
            read_data = Cy_SCB_UART_Get(CYBSP_UART_HW);
            while (0UL == Cy_SCB_UART_Put(CYBSP_UART_HW,read_data))
            {
            }
        }
    }
}

/* [] END OF FILE */
