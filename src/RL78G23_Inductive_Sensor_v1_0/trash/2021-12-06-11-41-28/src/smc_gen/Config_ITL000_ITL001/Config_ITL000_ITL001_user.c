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
* File Name    : Config_ITL000_ITL001_user.c
* Version      : 1.0.0
* Device(s)    : R7F100GLGxFB
* Description  : This file implements device driver for Config_ITL000_ITL001.
* Creation Date: 2021-11-26
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_ITL000_ITL001.h"
/* Start user code for include. Do not edit comment generated here */
#include "APP.h"
#include "ELC.h"
#include "Config_PORT.h"
#include "Config_TAU0_0.h"
#include "Config_TAU0_2.h"
#include "Config_TAU0_4.h"
#include "Config_UART0.h"
#include "r_cg_sau.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_ITL000_ITL001_Create_UserInit
* Description  : This function adds user code after initializing the ITL000_ITL001 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_ITL000_ITL001_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_Config_ITL000_ITL001_Callback_Shared_Interrupt
* Description  : This function handles the ITL000_ITL001 shared interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_ITL000_ITL001_Callback_Shared_Interrupt(void)
{
    /* Start user code for R_Config_ITL000_ITL001_Callback_Shared_Interrupt. Do not edit comment generated here */
	R_Config_InitWithoutSMS();
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void R_Config_InitWithoutSMS(void)
{
	/*test function used to init MUX and timers for configuration without SMS*/
	/*init MUXs pins*/
	APP_EnableAIC1_Enable();
	APP_EnableBIC1_Enable();
	APP_EnableAIC2_Enable();

	/*set ELC configuration for first sensor group - rotation*/
	P2_bit.no0 = 1;
	P2_bit.no1 = 1;
	P2_bit.no2 = 0;

	/*start next sequence
	 * enable simultaneous the excitations and point measurement */
	TS0 |= (_0004_TAU_CH2_START_TRG_ON | _0010_TAU_CH4_START_TRG_ON
			| _0001_TAU_CH0_START_TRG_ON);

}
/* End user code. Do not edit comment generated here */
