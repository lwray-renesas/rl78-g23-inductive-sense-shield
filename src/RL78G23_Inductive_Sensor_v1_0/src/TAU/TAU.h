/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : TAU.h
 * Description  : This file implements device driver for custom timer settings
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes
 *********************************************************************************************************************/
#ifndef TAU
#define TAU

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/


/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
/*arrays for timer register values
	 * 0 - rotation sensors
	 * 1 - planar sensors
	 * 2 - ferrite sensors
	 * 3 - external sensors
	 * 4 - rotation s3 */
typedef struct
{
	 uint16_t u16SensorsDelay[5];
	 uint16_t u16SensorsExcitationWidth[5];
	 uint16_t u16ReferenceDelay[5];
	 uint16_t u16ReferenceExcitationWidth[5];
	 uint16_t u16MeasurementPoint[5];
	 uint16_t u16IntervalTimer;
	 uint8_t u8CheckDiscState;
}TAU_stTimingValue;

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

void TAU_UpdateTimerValues(tSensorsSelector SensorGroup);
uint16_t TAU_u16ConvertUsToCounts(uint16_t u16us);
uint16_t TAU_u16ConvertMasterNsToCounts(uint16_t u16ns);
uint16_t TAU_u16ConvertSlaveNsToCounts(uint16_t u16ns);
uint16_t TAU_u16ConvertItMsToCounts(uint16_t u16ms);

#endif /* TAU */
