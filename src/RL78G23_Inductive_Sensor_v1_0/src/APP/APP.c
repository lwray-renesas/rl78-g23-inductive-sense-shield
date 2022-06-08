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
* File Name    : APP.c
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
#include "string.h"
#include "DIAG.h"
#include "read_data_flash.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
tSensorsSelector enSensorsSelector;

/***********************************************************************************************************************
 Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
 Global variables and functions
***********************************************************************************************************************/
/*RAM parameters*/
uint16_t au16SensorsDelayTemp[5] = { 8, 8, 6, 0, 8 }; // 312ns, 312ns, 250ns, 63ns, 312ns
uint16_t au16SensorsPulseTemp[5] = { 5, 5, 3, 0, 5 }; // 156ns, 156ns, 94ns, 0, 156ns
uint16_t au16ReferenceDelayTemp[5] = { 0, 0, 0, 0, 0 }; // 63ns, 63ns, 63ns, 63ns, 63ns
uint16_t au16ReferenceExcitationWidthTemp[5] = { 3, 3, 3, 0, 3 }; // 94ns, 94ns, 94ns, 0, 94ns
uint16_t au16MeasurementPointTemp[5] = { 0x5F, 0x5F, 0x5F, 0, 0x5F }; //3us, 3us, 3us, 0, 3us
extern TAU_stTimingValue TAU_tstTimingValue;

/*switch the channel pins for current sensor group*/
void APP_MuxSelectSensorGroup(tSensorsSelector SensorGroup)
{
	switch(SensorGroup)
	{
	case RotationSensors:
		/*select output I2A and I2B IC1*/
		APP_S1IC1_High();
		APP_S0IC1_Low();

		/*select output I3A IC2*/
		APP_S1IC2_High();
		APP_S0IC2_High();
		break;

	case PlanarSensors:
		/*select output I1A and I1B IC1*/
		APP_S1IC1_Low();
		APP_S0IC1_High();

		/*select output I1A IC2*/
		APP_S1IC2_Low();
		APP_S0IC2_High();
		break;

	case FerriteSensors:
		/*select output I0A and I0B IC1*/
		APP_S1IC1_Low();
		APP_S0IC1_Low();

		/*select output I2A IC2*/
		APP_S1IC2_High();
		APP_S0IC2_Low();
		break;

	case ExternalSensors:
		/*select output I3A IC1*/
		APP_S1IC1_High();
		APP_S0IC1_High();

		/*select output I0A IC2*/
		APP_S1IC2_Low();
		APP_S0IC2_Low();
		break;

	case RotationDiscCheck:
		/*select output I3B IC1*/
		APP_S1IC1_High();
		APP_S0IC1_High();

		/*select output I3A IC2*/
		APP_S1IC2_High();
		APP_S0IC2_High();
		break;

	default:
		//do nothing
		break;
	}
}

/*enable mux output*/
void APP_MuxEnableOutputs(void)
{
	/* enable IC1 MUX */
	APP_EnableAIC1_Enable();
	APP_EnableBIC1_Enable();

	/* enable IC2 MUX */
	APP_EnableAIC2_Enable();
	APP_EnableBIC2_Disable(); //not used
}

/*disable mux output*/
void APP_MuxDisableOutputs(void)
{
	/* disable  IC1 MUX */
	APP_EnableAIC1_Disable();
	APP_EnableBIC1_Disable();

	/* disable IC2 MUX */
	APP_EnableAIC2_Disable();
	APP_EnableBIC2_Disable(); //not used
}

/*disable all LEDs*/
void APP_DisableLEDs(void)
{
	/*disable LEDs*/
	APP_LED1_DISABLE();
	APP_LED2_DISABLE();
	APP_LED3_DISABLE();
	APP_LED4_DISABLE();
	APP_LEDCW_DISABLE();
	APP_LEDCCW_DISABLE();
}

/*replace the current values with the default RAM*/
void APP_SetRamDefaultValues()
{
	/*replace the data flash with the RAM variables*/
	memcpy(&TAU_tstTimingValue.u16SensorsDelay[0], &au16SensorsDelayTemp[0],10);
	memcpy(&TAU_tstTimingValue.u16SensorsExcitationWidth[0],&au16SensorsPulseTemp[0], 10);
	memcpy(&TAU_tstTimingValue.u16ReferenceDelay[0], &au16ReferenceDelayTemp[0],10);
	memcpy(&TAU_tstTimingValue.u16ReferenceExcitationWidth[0],&au16ReferenceExcitationWidthTemp[0], 10);
	memcpy(&TAU_tstTimingValue.u16MeasurementPoint[0],&au16MeasurementPointTemp[0], 10);
	TAU_tstTimingValue.u16IntervalTimer = 0x0FFF;
	TAU_tstTimingValue.u8CheckDiscState = 0;

	/*save to data flash*/
	DIAG_vSaveToDataFlash();
}
