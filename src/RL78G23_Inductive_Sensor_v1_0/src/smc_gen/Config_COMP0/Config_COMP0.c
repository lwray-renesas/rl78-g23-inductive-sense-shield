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
* File Name    : Config_COMP0.c
* Version      : 1.0.0
* Device(s)    : R7F100GLGxFB
* Description  : This file implements device driver for Config_COMP0.
* Creation Date: 2021-12-06
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
#include "Config_COMP0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_COMP0_Create
* Description  : This function initializes the comparator 0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_COMP0_Create(void)
{
    C0ENB = 0U;    /* disable comparator channel 0 operation */
    /* Set IVCMP0 pin */
    PMCA14 |= 0x80U;
    PM14 |= 0x80U;
    /* Set IVREF0 pin */
    PMCA2 |= 0x08U;
    PM2 |= 0x08U;
    COMPOCR &= _78_COMP0_COMPOCR_CLEAR;
    COMPOCR |= _80_COMP_SPEED_HIGH;    /* set high speed mode */
    COMPMDR &= _F0_COMP0_COMPMDR_CLEAR;
    COMPMDR |= (_00_COMP0_REFERENCE_VOLTAGE_IVREF0 | _00_COMP0_0_TO_VDD_14);
    COMPFIR &= _F0_COMP0_COMPFIR_CLEAR;
    COMPFIR |= (_00_COMP0_ONE_EDGE_INTERRUPT | _04_COMP0_FALLING_INTERRUPT);

    R_Config_COMP0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_COMP0_Start
* Description  : This function starts the comparator 0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_COMP0_Start(void)
{
    volatile uint16_t w_count;

    C0ENB = 1U;    /* enable comparator 0 operation */

    /* Change the waiting time according to the system */
    for(w_count = 0U; w_count < COMP_WAIT_TIME; w_count++)
    {
        NOP();
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_COMP0_Stop
* Description  : This function stops the comparator 0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_COMP0_Stop(void)
{
    C0ENB = 0U;     /* disable comparator 0 operation */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
