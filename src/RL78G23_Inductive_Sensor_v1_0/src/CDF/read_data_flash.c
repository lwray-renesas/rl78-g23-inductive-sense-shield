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
 * File Name    : read_data_flash.c
 * Version      : 1.0.0
 * Device(s)    : R7F100GLGxFB
 * Description  :
 * Creation Date: 2021-11-03
 ***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
 ***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "APP.h"
#include "TAU.h"
#include "DIAG.h"
#include "read_data_flash.h"
#include "string.h"

#include <control_data_flash.h>
#include <control_defines.h>
#include "control_config.h"
#include "r_smc_entry.h"
#include "r_rfd_common_api.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/***********************************************************************************************************************
 Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Global variables and functions
 ***********************************************************************************************************************/
/*variables for read Data Flash process*/
extern TAU_stTimingValue TAU_tstTimingValue;
uint16_t       l_u16_count;
uint8_t * pu8ReadData = (uint8_t *)&TAU_tstTimingValue;
uint32_t       l_u32_check_write_data_addr = WRITE_ADDRESS;
uint8_t* DATAFLASH_START_ADDRESS =  (uint8_t*)0xF1000;
uint8_t u8EmptyCounter = 0;



void vReadDataFlash()
{
	/*read Data Flash - direct copy from ROM to RAM memory*/
	for (l_u16_count = 0u; l_u16_count < sizeof(TAU_tstTimingValue); l_u16_count++)
	{
		pu8ReadData[l_u16_count] = (*(uint8_t  *)l_u32_check_write_data_addr);
		l_u32_check_write_data_addr++;
	}

	/*check if Data Flash is empty*/
	for (l_u16_count = 0u; l_u16_count < sizeof(TAU_tstTimingValue); l_u16_count++)
	{
		if(0xFF == pu8ReadData[l_u16_count])
		{
			u8EmptyCounter++;
		}
	}

	if(u8EmptyCounter >= sizeof(TAU_tstTimingValue))
	{
		/*replace with RAM default values and save to data flash*/
		APP_SetRamDefaultValues();
	}
}


