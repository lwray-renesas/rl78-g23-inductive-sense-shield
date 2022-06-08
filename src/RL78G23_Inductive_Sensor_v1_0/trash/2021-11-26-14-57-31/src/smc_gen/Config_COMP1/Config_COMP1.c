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
* File Name    : Config_COMP1.c
* Version      : 1.0.0
* Device(s)    : R7F100GLGxFB
* Description  : This file implements device driver for Config_COMP1.
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
#include "Config_COMP1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_COMP1_Create
* Description  : This function initializes the comparator 1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_COMP1_Create(void)
{
    C1ENB = 0U;    /* disable comparator channel 1 operation */
    /* Set IVCMP1 pin */
    PMCA12 |= 0x01U;
    PM12 |= 0x01U;
    /* Set IVREF1 pin */
    PMCA1 |= 0x08U;
    PMCE1 &= 0xF7U;
    PM1 |= 0x08U;
    COMPOCR &= _0F_COMP1_COMPOCR_CLEAR;
    COMPOCR |= _80_COMP_SPEED_HIGH;    /* set high speed mode */
    COMPMDR &= _0F_COMP1_COMPMDR_CLEAR;
    COMPMDR |= (_00_COMP1_REFERENCE_VOLTAGE_IVREF1 | _00_COMP1_0_TO_VDD_14);
    COMPFIR &= _0F_COMP1_COMPFIR_CLEAR;
    COMPFIR |= (_00_COMP1_ONE_EDGE_INTERRUPT | _40_COMP1_FALLING_INTERRUPT);

    R_Config_COMP1_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_COMP1_Start
* Description  : This function starts the comparator 1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_COMP1_Start(void)
{
    volatile uint16_t w_count;

    C1ENB = 1U;    /* enable comparator 1 operation */

    /* Change the waiting time according to the system */
    for(w_count = 0U; w_count < COMP_WAIT_TIME; w_count++)
    {
        NOP();
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_COMP1_Stop
* Description  : This function stops the comparator 1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_COMP1_Stop(void)
{
    C1ENB = 0U;     /* disable comparator 1 operation */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
