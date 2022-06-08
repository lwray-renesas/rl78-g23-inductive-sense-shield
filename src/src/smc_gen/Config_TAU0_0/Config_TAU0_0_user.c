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
* File Name    : Config_TAU0_0_user.c
* Version      : 1.0.0
* Device(s)    : R7F100GLGxFB
* Description  : This file implements device driver for Config_TAU0_0.
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
#include "Config_TAU0_0.h"
/* Start user code for include. Do not edit comment generated here */
#include "APP.h"
#include "ELC.h"
#include "TAU.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t u8ActualPosition;
volatile uint8_t u8LastPositionRotation;
volatile uint8_t u8LastPositionPlanarSensor1 = 0x80;
volatile uint8_t u8LastPositionPlanarSensor2 = 0x08;
volatile uint8_t u8LastPositionFerriteSensor1 = 0x80;
volatile uint8_t u8LastPositionFerriteSensor2 = 0x08;

volatile uint8_t u8Quarter = 4;
volatile uint16_t u16ErrorCount = 0;
volatile int16_t i16RotationCount = 0;
volatile uint16_t u16PlanarSensor1Count = 0;
volatile uint16_t u16PlanarSensor2Count = 0;
volatile uint16_t u16FerriteSensor1Count = 0;
volatile uint16_t u16FerriteSensor2Count = 0;

uint8_t u8DiscCheckInterval = 0;
uint8_t u8DiscCheckFlag = 0;
tSensorsSelector enSensorLastState;
uint8_t u8PlanarSensor1State;
uint8_t u8PlanarSensor2State;
uint8_t u8RotationState;
uint8_t u8FerriteSensor1State;
uint8_t u8FerriteSensor2State;

extern tSensorsSelector enSensorsSelector;
extern volatile uint8_t u8MainStopState;
extern TAU_stTimingValue TAU_tstTimingValue;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TAU0_0_Create_UserInit
* Description  : This function adds user code after initializing the TAU0 channel 0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAU0_0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TAU0_0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_Config_TAU0_0_interrupt(void)
{
    /* Start user code for r_Config_TAU0_0_interrupt. Do not edit comment generated here */

	/*read comparators state*/
	u8ActualPosition = COMPMDR & 0x88;

	/*toggle P30 to check the point measurement*/
	P3_bit.no0 = 1U;
	P3_bit.no0 = 0U;

	/*stop interval TAU0*/
	TT0 |= _0001_TAU_CH0_STOP_TRG_ON;

	switch(enSensorsSelector)
	{

	/*algorithm for rotation sensors 1 and 2 */
	case RotationSensors:
		/* reset shield LEDs -> 125ms blink delay */
		APP_DisableLEDs();

		u8RotationState  = u8ActualPosition;
		if(u8LastPositionRotation != u8ActualPosition)
		{
			switch(u8ActualPosition)
			{
			case 0x00:
				if(u8LastPositionRotation == 0x80)
				{
					u8Quarter++;
				}
				else if(u8LastPositionRotation == 0x08)
				{
					u8Quarter--;
				}
				else
				{
					u16ErrorCount++;
				}
				break;

			case 0x08:
				if(u8LastPositionRotation == 0x00)
				{
					u8Quarter++;
				}
				else if(u8LastPositionRotation == 0x88)
				{
					u8Quarter--;
				}
				else
				{
					u16ErrorCount++;
				}
				break;

			case 0x88:
				if(u8LastPositionRotation == 0x08)
				{
					u8Quarter++;
				}
				else if(u8LastPositionRotation == 0x80)
				{
					u8Quarter--;
				}
				else
				{
					u16ErrorCount++;
				}
				break;

			case 0x80:
				if(u8LastPositionRotation == 0x88)
				{
					u8Quarter++;
				}
				else if(u8LastPositionRotation == 0x00)
				{
					u8Quarter--;
				}
				else
				{
					u16ErrorCount++;
				}
				break;

			default:
				break;
			}

			u8LastPositionRotation = u8ActualPosition;

			if(u8Quarter == 0)
			{
				i16RotationCount--;
				u8Quarter = 4;
				APP_LEDCW_ENABLE();
			}
			else if(u8Quarter == 8)
			{
				i16RotationCount++;
				u8Quarter = 4;
				APP_LEDCCW_ENABLE();
			}
		}

		/* if the disc rotation is not detected
		 * check the disc presence at every 5 seconds */
		else if(0x88 == u8RotationState)
		{
			if(TAU_tstTimingValue.u8CheckDiscState)
			{
				u8DiscCheckInterval++;
				if(u8DiscCheckInterval > 39)
				{
					u8DiscCheckInterval  = 0;

					/* enable disc check flag */
					u8DiscCheckFlag = 1U;
				}
			}
		}
		break;

		/*algorithm for planar sensors */
	case PlanarSensors:
		u8PlanarSensor1State  = (u8ActualPosition & 0x80);
		u8PlanarSensor2State  = (u8ActualPosition & 0x08);

		/*detect comparator 1*/
		if(u8LastPositionPlanarSensor1 != u8PlanarSensor1State)
		{
			/*detected planar L2*/
			if(0 != u8LastPositionPlanarSensor1)
			{
				u16PlanarSensor1Count++;
			}
			u8LastPositionPlanarSensor1 = u8PlanarSensor1State;
		}
		if(0 == u8LastPositionPlanarSensor1)
		{
			APP_LED3_ENABLE();
		}


		/*detect comparator 0*/
		if(u8LastPositionPlanarSensor2 != u8PlanarSensor2State)
		{
			/*detected planar L1*/
			if(0 != u8LastPositionPlanarSensor2)
			{
				u16PlanarSensor2Count++;
			}
			u8LastPositionPlanarSensor2 = u8PlanarSensor2State;
		}
		if(0 == u8LastPositionPlanarSensor2)
		{
			APP_LED4_ENABLE();
		}
		break;

		/* algorithm for ferrite sensors */
	case FerriteSensors:
		u8FerriteSensor1State  = (u8ActualPosition & 0x80);
		u8FerriteSensor2State  = (u8ActualPosition & 0x08);

		/*detect comparator 1*/
		if(u8LastPositionFerriteSensor1 != u8FerriteSensor1State)
		{
			/*detected ferrite L5*/
			if(0 != u8LastPositionFerriteSensor1)
			{
				u16FerriteSensor1Count++;
			}
			u8LastPositionFerriteSensor1 = u8FerriteSensor1State;
		}
		if(0 == u8LastPositionFerriteSensor1)
		{
			APP_LED1_ENABLE();
		}


		/*detect comparator 0*/
		if(u8LastPositionFerriteSensor2 != u8FerriteSensor2State)
		{
			/*detected ferrite L4*/
			if(0 != u8LastPositionFerriteSensor2)
			{
				u16FerriteSensor2Count++;
			}
			u8LastPositionFerriteSensor2 = u8FerriteSensor2State;
		}
		if(0 == u8LastPositionFerriteSensor2)
		{
			APP_LED2_ENABLE();
		}
		break;

	case ExternalSensors:
		//do nothing
		break;

	case RotationDiscCheck:
		/* check only comparator 1 */
		u8ActualPosition &= 0x80;

		if(u8ActualPosition)
		{
			/*enable warning blink LED*/
			APP_LEDCW_ENABLE();
			APP_LEDCCW_ENABLE();
		}
		/* restore to the last sensor sequence */
		enSensorsSelector = enSensorLastState;

		break;

	default:
		//do nothing
		break;
	}

	/* enable a disc check sequence if rotation
	 *is not detected at every 5 seconds */

	if(u8DiscCheckFlag)
	{
		/* erase flag */
		u8DiscCheckFlag = 0;

		/* force jump to rotation sensor 3 state*/
		enSensorLastState = enSensorsSelector;
		enSensorsSelector = RotationDiscCheck;

		/* set selection channels for the next sensors group */
		APP_MuxSelectSensorGroup(enSensorsSelector);

		/* update custom timing for rotation sensor group */
		TAU_UpdateTimerValues(enSensorsSelector);

		/* update input 3-pins used as group selector for sensors */
		ELC_SelectSensorGroup();
		ELC_SelectOutputGroup();

		/*start next sequence
		 * enable simultaneous the excitations and point measurement */
		TS0 |= (_0004_TAU_CH2_START_TRG_ON | _0010_TAU_CH4_START_TRG_ON
				| _0001_TAU_CH0_START_TRG_ON);
	}
	else
	{

		/*set next sequence*/
		enSensorsSelector++;

		if(enSensorsSelector >= 3) //available just for first 3 groups
		{
			/* reset to rotation sensor and wait for SMS */
			enSensorsSelector = RotationSensors;

			/* set selection channels for the next sensors group */
			APP_MuxSelectSensorGroup(enSensorsSelector);

			/* update custom timing for rotation sensor group */
			TAU_UpdateTimerValues(enSensorsSelector);

			/* update input 3-pins used as group selector for sensors */
			ELC_SelectSensorGroup();
			ELC_SelectOutputGroup();

			/*enable flag for STOP mode*/
			u8MainStopState = 1U;
		}
		/*first measurement is enabled from IT and this sequence
		 * is available for the next 2*/
		else
		{
			/* set selection channels for the next sensors group */
			APP_MuxSelectSensorGroup(enSensorsSelector);

			/* update custom timing for rotation sensor group */
			TAU_UpdateTimerValues(enSensorsSelector);

			/* update input 3-pins used as group selector for sensors */
			ELC_SelectSensorGroup();
			ELC_SelectOutputGroup();

			/*start next sequence
			 * enable simultaneous the excitations and point measurement */
			TS0 |= (_0004_TAU_CH2_START_TRG_ON | _0010_TAU_CH4_START_TRG_ON
					| _0001_TAU_CH0_START_TRG_ON);
		}
	}
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
