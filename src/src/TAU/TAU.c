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
* File Name    : TAU.c
* Version      : 1.0.0
* Device(s)    : R7F100GLGxFB
* Description  :
* Creation Date: 2021-07-02
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "APP.h"
#include "ELC.h"
#include "TAU.h"
#include "Config_TAU0_4.h"
#include "Config_TAU0_2.h"
#include "Config_TAU0_0.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/***********************************************************************************************************************
 Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
 Global variables and functions
***********************************************************************************************************************/
TAU_stTimingValue TAU_tstTimingValue;

/*update the timer registers with structure values*/
void TAU_UpdateTimerValues(tSensorsSelector SensorGroup)
{
	/*excitation TAU02-TAU03 and TAU04-TAU05*/
	TDR02 = TAU_tstTimingValue.u16SensorsDelay[SensorGroup];
	TDR03 = TAU_tstTimingValue.u16SensorsExcitationWidth[SensorGroup];
	TDR04 = TAU_tstTimingValue.u16ReferenceDelay[SensorGroup];
	TDR05 = TAU_tstTimingValue.u16ReferenceExcitationWidth[SensorGroup];

	/*measurement point TAU0*/
	TDR00 = TAU_tstTimingValue.u16MeasurementPoint[SensorGroup];
}

/*convert from us to counts available for measurement point*/
uint16_t TAU_u16ConvertUsToCounts(uint16_t u16us)
{
	uint16_t pclkd_freq_hz = R_BSP_GetFclkFreqHz();

	/*apply formula from RL78/G23 user manual chapter 7.8.1 available for interval timer*/
	uint16_t period_counts = ((u16us * 32u) - 1);

	return period_counts;
}

/*convert from master delay period to counts*/
uint16_t TAU_u16ConvertMasterNsToCounts(uint16_t u16ns)
{
	uint16_t pclkd_freq_hz = R_BSP_GetFclkFreqHz();

	/*apply formula from RL78/G23 user manual chapter 7.9.1 available for one-shot pulse output*/
	uint16_t period_counts = ((u16ns * 32u)/1000 - 2);

	return period_counts;
}

/*convert from slave pulse width period to counts*/
uint16_t TAU_u16ConvertSlaveNsToCounts(uint16_t u16ns)
{
	uint16_t pclkd_freq_hz = R_BSP_GetFclkFreqHz();

	/*apply formula from RL78/G23 user manual chapter 7.9.1 available for one-shot pulse output*/
	uint16_t period_counts = (u16ns * 32u)/1000;

	return period_counts;
}

/*convert from ms period to IT counts*/
uint16_t TAU_u16ConvertItMsToCounts(uint16_t u16ms)
{
	uint32_t uLowPowerFreqHz = 32768; //low power clock
	uint16_t period_counts = (uint16_t) (((uint64_t) uLowPowerFreqHz * u16ms) / 1000);

	return period_counts;
}


