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
* File Name    : Config_PORT.h
* Version      : 1.0.0
* Device(s)    : R7F100GLGxFB
* Description  : This file implements device driver for Config_PORT.
* Creation Date: 2021-12-06
***********************************************************************************************************************/

#ifndef CFG_Config_PORT_H
#define CFG_Config_PORT_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_port.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _80_PM0_DEFAULT                         (0x80U)    /* PM0 default value */
#define _FC_PM3_DEFAULT                         (0xFCU)    /* PM3 default value */
#define _F0_PM4_DEFAULT                         (0xF0U)    /* PM4 default value */
#define _C0_PM5_DEFAULT                         (0xC0U)    /* PM5 default value */
#define _F0_PM6_DEFAULT                         (0xF0U)    /* PM6 default value */
#define _F8_PM12_DEFAULT                        (0xF8U)    /* PM12 default value */
#define _3C_PM14_DEFAULT                        (0x3CU)    /* PM14 default value */
#define _F3_PMCA0_DEFAULT                       (0xF3U)    /* PMCA0 default value */
#define _F7_PMCA1_DEFAULT                       (0xF7U)    /* PMCA1 default value */
#define _FE_PMCA12_DEFAULT                      (0xFEU)    /* PMCA12 default value */
#define _7F_PMCA14_DEFAULT                      (0x7FU)    /* PMCA14 default value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_PORT_Create(void);
void R_Config_PORT_ReadPmnValues(void);
void R_Config_PORT_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
