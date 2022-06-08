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
 * File Name    : DIAG.h
 * Description  : This file implements device driver for custom serial communication
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes
 *********************************************************************************************************************/
#ifndef DIAG
#define DIAG

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#define DIAG_nServiceDataOffset  0
#define DIAG_nDIDLenght 2

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
enum DIAG_enState{
    DIAG_enEnabled = 0x00,
    DIAG_enDisabled
};
typedef enum DIAG_enState DIAG_tenState;

/*Accepted Services SID*/
enum DIAG_enAcceptedSIDS{
    DIAG_ReadDataSID = 0x22,
    DIAG_WriteDataSID = 0x2E,
	DIAG_ResetToDefaultValues = 0x08,
	DIAG_SaveToDataFlash =  0x09,
	DIAG_CloseCommunication = 0x0A,
    DIAG_NoEventSID = 0xFF
};
typedef enum DIAG_enAcceptedSIDS DIAG_tenAcceptedSIDS;

enum DIAG_enEventStatus{
	DIAG_EventNoStates 				= 0x00,
    DIAG_EventReadProcess			= 0x01,
    DIAG_EventWriteProcess 		= 0x02,
};
typedef enum DIAG_enEventStatus DIAG_tenEventStatus;


enum DIAG_enAcceptedDIDs{
	DIAG_enReserved1												= 0xFF00,
	DIAG_enSetExcitation_Period                                     = 0xFF01,

	/*button sensors*/
	DIAG_enButton_PlanarSensor_Width								= 0xFF02,
	DIAG_enButton_PlanarReference_Width								= 0xFF03,
	DIAG_enButton_PlanarSensor_Delay								= 0xFF04,
	DIAG_enButton_PlanarReference_Delay								= 0xFF05,
	DIAG_enButton_PlanarSensor_MeasurementPoint						= 0xFF06,

	DIAG_enButton_FerriteSensor_Width								= 0xFF07,
	DIAG_enButton_FerriteReference_Width							= 0xFF08,
	DIAG_enButton_FerriteSensor_Delay								= 0xFF09,
	DIAG_enButton_FerriteReference_Delay							= 0xFF0A,
	DIAG_enButton_FerriteSensor_MeasurementPoint					= 0xFF0B,

	DIAG_enButton_ExternalSensor_Width								= 0xFF0C,
	DIAG_enButton_ExternalReference_Width							= 0xFF0D,
	DIAG_enButton_ExternalSensor_Delay								= 0xFF0E,
	DIAG_enButton_ExternalReference_Delay							= 0xFF0F,
	DIAG_enButton_ExternalSensor_MeasurementPoint					= 0xFF10,

	/*rotation sensors*/
	DIAG_enRotation_Sensor12_Width									= 0xFF11,
	DIAG_enRotation_Sensor12_Delay									= 0xFF12,
	DIAG_enRotation_Reference12_Width								= 0xFF13,
	DIAG_enRotation_Reference12_Delay								= 0xFF14,
	DIAG_enRotation_Sensor12_MeasurementPoint						= 0xFF15,

	DIAG_enRotation_Sensor3_Width									= 0xFF16,
	DIAG_enRotation_Sensor3_Delay									= 0xFF17,
	DIAG_enRotation_Reference3_Width								= 0xFF18,
	DIAG_enRotation_Reference3_Delay								= 0xFF19,
	DIAG_enRotation_Sensor3_MeasurementPoint						= 0xFF1A,

	/*get data*/
    DIAG_enGetDataSingleTime                        				= 0xFF1B,
	DIAG_enReserved2												= 0xFF1C,

	/*set rotation sensor3 state*/
	DIAG_enSetRotationDiscPresenceEnable							= 0xFF1D,

	/*close UART communication*/
    DIAG_enDeactivateDIAG_UART                                      = 0xFF1E,


    DIAG_enNrOfServices
};
typedef enum DIAG_enAcceptedDIDs DIAG_tenAcceptedDIDs;



/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/


/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

/*send counter information*/
void DIAG_SendCountInformation(int16_t RotationCount, uint16_t RotationError,
		uint16_t Planar1Count, uint16_t Planar2Count,
		uint16_t Ferrite1Count, uint16_t Ferrite2Count);
void DIAG_vResetToDefaultValues();
void DIAG_vSaveToDataFlash();
void DIAG_vReadServiceDispatcher(uint16_t u16ServiceDID,uint8_t *pu8DataBuffer);
void DIAG_vWriteServiceDispatcher(uint16_t u16ServiceDID);

#endif /* DIAG */
