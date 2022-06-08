/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
 * LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
 * ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
 * of this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : Config_UART0_user.c
 * Version      : 1.0.0
 * Device(s)    : R7F100GLGxFB
 * Description  : This file implements device driver for Config_UART0.
 * Creation Date: 2021-11-26
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
 ***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
#define DIAG_nServiceDataMaxLength 32
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
 ***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_UART0.h"
/* Start user code for include. Do not edit comment generated here */
#include "DIAG.h"
#include "r_cg_sau.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
 ***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;    /* uart0 transmit buffer address */
extern volatile uint16_t g_uart0_tx_count;        /* uart0 transmit data number */
extern volatile uint8_t * gp_uart0_rx_address;    /* uart0 receive buffer address */
extern volatile uint16_t g_uart0_rx_count;        /* uart0 receive data number */
extern volatile uint16_t g_uart0_rx_length;       /* uart0 receive data length */
/* Start user code for global. Do not edit comment generated here */

uint8_t receiveSwOverRunCounter =0;
uint8_t receiveEndCounter = 0;
uint8_t au8DataReceived[8];

extern volatile uint8_t DIAG_EventState;
volatile uint8_t UART_u8State = UART_Disconnected;

extern void R_SAU0_Set_SnoozeOff(void);

/** @brief Called inside interrupt*/
void Exit_stop_snooze_uart_err(void)
{
	R_Config_UART0_Stop();
	R_SAU0_Set_SnoozeOff();
	R_Config_UART0_Start();
}
/* END OF FUCNTION*/
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
 * Function Name: R_Config_UART0_Create_UserInit
 * Description  : This function adds user code after initializing UART0.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_Config_UART0_Create_UserInit(void)
{
	/* Start user code for user init. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: r_Config_UART0_callback_receiveend
 * Description  : This function is a callback function when UART0 finishes reception.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
static void r_Config_UART0_callback_receiveend(void)
{
	/* Start user code for r_Config_UART0_callback_receiveend. Do not edit comment generated here */
	uint16_t u16DID = 0xFFFF;
	uint16_t u16Data = 0;

	/*receive the 4 bytes of data
	 * byte 0->1 - command ID
	 * byte 2->3 - data message with LSB first */

	/*Extract DID*/
	u16DID = au8DataReceived[0];
	u16DID = u16DID << 8;
	u16DID |= au8DataReceived[1];

	if(DIAG_EventState == DIAG_EventWriteProcess)
	{
		DIAG_vReadServiceDispatcher(u16DID,&au8DataReceived[0]);
	}
	else if (DIAG_EventState == DIAG_EventReadProcess)
	{
		DIAG_vWriteServiceDispatcher(u16DID);
	}

	/*check the UART status - enable the UART communication and disable the STOP mode*/
	if(UART_u8State == UART_WaitToChangeState)
	{
		UART_u8State = UART_Connected;
		/*Change UART to normal mode*/
		Exit_stop_snooze_uart_err();
	}
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: r_Config_UART0_callback_error
 * Description  : This function is a callback function when UART0 reception error occurs.
 * Arguments    : err_type -
 *                    error type info
 * Return Value : None
 ***********************************************************************************************************************/
static void r_Config_UART0_callback_error(uint8_t err_type)
{
	/* Start user code for r_Config_UART0_callback_error. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: r_Config_UART0_callback_softwareoverrun
 * Description  : This function is a callback when UART0 receives an overflow data.
 * Arguments    : rx_data -
 *                    receive data
 * Return Value : None
 ***********************************************************************************************************************/
static void r_Config_UART0_callback_softwareoverrun(uint16_t rx_data)
{
	/* Start user code for r_Config_UART0_callback_softwareoverrun. Do not edit comment generated here */
	/*check the UART status - enable the UART communication and disable the STOP mode*/
	if(UART_u8State == UART_Disconnected)
	{
		UART_u8State = UART_WaitToChangeState;
	}
	/* check the service*/
	switch(rx_data)
	{
	/*write service*/
	case DIAG_WriteDataSID:
	{
		DIAG_EventState = DIAG_EventWriteProcess;

		/* receive the other 3 parameters */
		R_Config_UART0_Receive(au8DataReceived, 4U);
		break;
	}
	/*read service*/
	case DIAG_ReadDataSID:
	{

		DIAG_EventState = DIAG_EventReadProcess;

		/* receive the other 3 parameters */
		R_Config_UART0_Receive(au8DataReceived, 2U);
		break;
	}
	/*close communication service*/
	case DIAG_CloseCommunication:
	{
		UART_u8State = UART_Disconnected;
		break;
	}
	/*save to data flash*/
	case DIAG_SaveToDataFlash:
	{
		DIAG_vSaveToDataFlash();
		break;
	}

	default:
	{
		break;
	}
	}
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: r_Config_UART0_interrupt_send
 * Description  : This function is UART0 send interrupt service routine.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void r_Config_UART0_interrupt_send(void)
{
	if (g_uart0_tx_count > 0U)
	{
		TXD0 = *gp_uart0_tx_address;
		gp_uart0_tx_address++;
		g_uart0_tx_count--;
	}
}

/***********************************************************************************************************************
 * Function Name: r_Config_UART0_interrupt_receive
 * Description  : This function is UART0 receive interrupt service routine.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void r_Config_UART0_interrupt_receive(void)
{
	uint8_t rx_data;

	rx_data = RXD0;

	if (g_uart0_rx_length > g_uart0_rx_count)
	{
		*gp_uart0_rx_address = rx_data;
		gp_uart0_rx_address++;
		g_uart0_rx_count++;

		if (g_uart0_rx_length == g_uart0_rx_count)
		{
			r_Config_UART0_callback_receiveend();
		}
	}
	else
	{
		r_Config_UART0_callback_softwareoverrun(rx_data);
	}
}

/***********************************************************************************************************************
 * Function Name: r_Config_UART0_interrupt_error
 * Description  : This function is UART0 error interrupt service routine.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void r_Config_UART0_interrupt_error(void)
{
	uint8_t err_type;

	*gp_uart0_rx_address = RXD0;
	err_type = (uint8_t)(SSR01 & 0x0007U);
	SIR01 = (uint16_t)err_type;
	r_Config_UART0_callback_error(err_type);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */