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
#include "DIAG.h"
#include "Config_TAU0_4.h"
#include "Config_TAU0_2.h"
#include "Config_TAU0_0.h"
#include "string.h"
#include "Config_UART0.h"

#include <control_data_flash.h>
#include <control_defines.h>
#include "control_config.h"
#include "r_smc_entry.h"
#include "r_rfd_common_api.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
uint8_t buffer[] = "Rot: -0000000000, L1: 0000000000, L2: 0000000000, L4: 0000000000, L5 0000000000\n\r\0";

volatile DIAG_tenState DIAG_Status;
volatile uint8_t u8DataReceivedCounter = 0x00;


extern TAU_stTimingValue TAU_tstTimingValue;

extern volatile uint16_t u16ErrorCount;
extern volatile int16_t i16RotationCount;
extern volatile uint16_t u16PlanarSensor1Count;
extern volatile uint16_t u16PlanarSensor2Count;
extern volatile uint16_t u16FerriteSensor1Count;
extern volatile uint16_t u16FerriteSensor2Count;

extern volatile uint8_t UART_u8TxState;

volatile uint8_t DIAG_EventState = 0;

/***********************************************************************************************************************
 Pragma directive
 ***********************************************************************************************************************/

///***********************************************************************************************************************
// Global variables and functions
//***********************************************************************************************************************/




/*call function to send information regarding number of rotation*/
void DIAG_SendCountInformation(int16_t RotationCount, uint16_t RotationError,
		uint16_t Planar1Count, uint16_t Planar2Count,
		uint16_t Ferrite1Count, uint16_t Ferrite2Count)
{
	uint8_t i;

	/*convert rotation counter*/
	if (RotationCount >= 0)
	{
		buffer[5] =' ';
	}
	else
	{
		RotationCount = -RotationCount;
	}

	i = 15;
	do
	{
		buffer[i--] = RotationCount%10 + '0';
	}
	while (0 != (RotationCount/=10));

	//	/*convert rotation errors*/
	//	buffer[23]= RotationError/100 +'0';
	//	buffer[24]= ((RotationError/10) % 10) +'0';
	//	buffer[25]= (RotationError % 10) + '0';

	/*convert planar S1 counter*/
	i = 31;
	do
	{
		buffer[i--] = Planar1Count%10 + '0';
	}
	while (0 != (Planar1Count/=10));

	/*convert planar S2 counter*/
	i = 47;
	do
	{
		buffer[i--] = Planar2Count%10 + '0';
	}
	while (0 != (Planar2Count/=10));

	/*convert ferrite S1 counter*/
	i = 63;
	do
	{
		buffer[i--] = Ferrite1Count%10 + '0';
	}
	while (0 != (Ferrite1Count/=10));

	/*convert ferrite S2 counter*/
	i = 78;
	do
	{
		buffer[i--] = Ferrite2Count%10 + '0';
	}
	while (0 != (Ferrite2Count/=10));

	/*check if another transmission is in progress and wait for it*/
	while(1 == UART_u8TxState);
	/*enable UART transmission flag*/
	UART_u8TxState = 1U;
	/*send sensor counters informations*/
	R_Config_UART0_Send(buffer, strlen(buffer));
}


void DIAG_vResetToDefaultValues()
{
	APP_SetRamDefaultValues();
}

void DIAG_vSaveToDataFlash()
{
	DI();
	Write_DataFlashControl(WRITE_ADDRESS, sizeof(TAU_tstTimingValue), (uint8_t *)&TAU_tstTimingValue);
	EI();
}

void DIAG_vReadServiceDispatcher(uint16_t u16ServiceDID,uint8_t *pu8DataBuffer)
{
	uint16_t u16Aux = 0;

	switch(u16ServiceDID)
	{
	/*Coil buttons diagnostic services*/
	case DIAG_enButton_PlanarSensor_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsExcitationWidth[PlanarSensors] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_PlanarReference_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceExcitationWidth[PlanarSensors] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_FerriteSensor_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsExcitationWidth[FerriteSensors] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_FerriteReference_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceExcitationWidth[FerriteSensors] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_ExternalSensor_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsExcitationWidth[ExternalSensors] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_ExternalReference_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceExcitationWidth[ExternalSensors] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_PlanarSensor_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsDelay[PlanarSensors] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_PlanarReference_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceDelay[PlanarSensors] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_FerriteSensor_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsDelay[FerriteSensors] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_FerriteReference_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceDelay[FerriteSensors] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_ExternalSensor_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsDelay[ExternalSensors] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_ExternalReference_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceDelay[ExternalSensors] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_PlanarSensor_MeasurementPoint:
	{
		/*Compute new measurement point*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16MeasurementPoint[PlanarSensors] = TAU_u16ConvertUsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_FerriteSensor_MeasurementPoint:
	{
		/*Compute new measurement point*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16MeasurementPoint[FerriteSensors] = TAU_u16ConvertUsToCounts(u16Aux);

		break;
	}
	case DIAG_enButton_ExternalSensor_MeasurementPoint:
	{
		/*Compute new measurement point*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16MeasurementPoint[ExternalSensors] = TAU_u16ConvertUsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Sensor12_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsExcitationWidth[RotationSensors] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Sensor3_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsExcitationWidth[RotationDiscCheck] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Reference12_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceExcitationWidth[RotationSensors] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Reference3_Width:
	{
		/*Compute new width*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceExcitationWidth[RotationDiscCheck] = TAU_u16ConvertSlaveNsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Sensor12_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsDelay[RotationSensors] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Sensor3_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16SensorsDelay[RotationDiscCheck] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Reference12_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceDelay[RotationSensors] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Reference3_Delay:
	{
		/*Compute new delay*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16ReferenceDelay[RotationDiscCheck] = TAU_u16ConvertMasterNsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Sensor12_MeasurementPoint:
	{
		/*Compute new measurement point*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16MeasurementPoint[RotationSensors] = TAU_u16ConvertUsToCounts(u16Aux);

		break;
	}
	case DIAG_enRotation_Sensor3_MeasurementPoint:
	{
		/*Compute new measurement point*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);
		TAU_tstTimingValue.u16MeasurementPoint[RotationDiscCheck] = TAU_u16ConvertUsToCounts(u16Aux);

		break;
	}

	case DIAG_enSetRotationDiscPresenceEnable:
	{
		/*Compute enable state*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);

		if(u16Aux == 0xFFFF)
		{
			TAU_tstTimingValue.u8CheckDiscState = 1U;
		}
		else
		{
			TAU_tstTimingValue.u8CheckDiscState = 0;
		}

		break;
	}
	case DIAG_enSetExcitation_Period:
	{
		/*extract period*/
		u16Aux = *(pu8DataBuffer + 3);
		u16Aux = u16Aux << 8;
		u16Aux |= *(pu8DataBuffer + 2);

		/*update Interval Time register*/
		TAU_tstTimingValue.u16IntervalTimer = TAU_u16ConvertItMsToCounts(u16Aux);

		/*set interval timer period*/
		ITLCMP00 = TAU_tstTimingValue.u16IntervalTimer;
		break;
	}
	default :
	{
		break;
	}
	}

	/*clean diag state*/
	DIAG_EventState = DIAG_EventNoStates;
}

void DIAG_vWriteServiceDispatcher(uint16_t u16ServiceDID)
{
	switch(u16ServiceDID)
	{
	/*Coil buttons diagnostic services*/
	case DIAG_enGetDataSingleTime:
	{
		DIAG_SendCountInformation(i16RotationCount, u16ErrorCount, u16PlanarSensor1Count,
				u16PlanarSensor2Count, u16FerriteSensor1Count, u16FerriteSensor2Count);

		break;
	}
	default:
	{
		break;
	}
	}
	/*clean diag state*/
	DIAG_EventState = DIAG_EventNoStates;
}



