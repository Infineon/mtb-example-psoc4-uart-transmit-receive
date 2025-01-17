/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC 4 - UART Transmit
*              and Receive Application for ModusToolbox.
*
* Related Document: See README.md 
*
*
*******************************************************************************
* Copyright 2020-2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
 * Include header files
 ******************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define LED_ON       0
#define LED_OFF     (!LED_ON)

/*******************************************************************************
* Global Variables
*******************************************************************************/
cy_stc_scb_uart_context_t CYBSP_UART_context;

/*******************************************************************************
* Function Name: handle_error
********************************************************************************
* Summary:
* User defined error handling function.
*
*******************************************************************************/
void handle_error(void)
{
    /* Disable all interrupts */
    __disable_irq();

    /* Switch On error LED */
    Cy_GPIO_Write(CYBSP_LED1_PORT, CYBSP_LED1_NUM, LED_ON);
    while(1){}
}


/*******************************************************************************
* Function Name: main
********************************************************************************
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
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    uint32_t read_data;
    cy_en_scb_uart_status_t initstatus;

    /* Turn off the USER LED */
    Cy_GPIO_Write(CYBSP_LED1_PORT, CYBSP_LED1_NUM, LED_OFF);

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize the UART */
    initstatus = Cy_SCB_UART_Init(CYBSP_UART_HW, &CYBSP_UART_config, &CYBSP_UART_context);

    /* Initialization failed. Handle error */
    if(initstatus!=CY_SCB_UART_SUCCESS)
    {
        handle_error();
    }

    /* Enable global interrupts */
    __enable_irq();

    Cy_SCB_UART_Enable(CYBSP_UART_HW);

    /* Transmit Header to the Terminal */
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "\x1b[2J\x1b[;H");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "***********************************************************\r\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "PSoC 4 MCU UART transmit and receive\r\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "***********************************************************\r\n\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, ">> Start typing to see the echo on the screen \r\n\n");

   for (;;)
    {
        /* Check if there is a received character from user console */
        if (0UL != Cy_SCB_UART_GetNumInRxFifo(CYBSP_UART_HW))
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
