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
 * File Name    : APP.h
 * Description  : This file implements device driver for app layer
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes
 *********************************************************************************************************************/
#ifndef APP
#define APP

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#define APP_LED1_ENABLE()			P2_bit.no7 = 1U
#define APP_LED1_DISABLE()			P2_bit.no7 = 0U

#define APP_LED2_ENABLE()			P2_bit.no6 = 1U
#define APP_LED2_DISABLE()			P2_bit.no6 = 0U

#define APP_LED3_ENABLE()			P2_bit.no5 = 1U
#define APP_LED3_DISABLE()			P2_bit.no5 = 0U

#define APP_LED4_ENABLE()			P2_bit.no4 = 1U
#define APP_LED4_DISABLE()			P2_bit.no4 = 0U

#define APP_LEDCW_ENABLE()			P14_bit.no6 = 1U
#define APP_LEDCW_DISABLE()			P14_bit.no6 = 0U

#define APP_LEDCCW_ENABLE()			P13_bit.no0 = 1U
#define APP_LEDCCW_DISABLE()		P13_bit.no0 = 0U

#define APP_EnableAIC1_Enable()		P5_bit.no2 = 0U
#define APP_EnableAIC1_Disable()	P5_bit.no2 = 1U

#define APP_EnableBIC1_Enable()		P5_bit.no5 = 0U
#define APP_EnableBIC1_Disable()	P5_bit.no5 = 1U

#define APP_S0IC1_High()			P5_bit.no4 = 1U
#define APP_S0IC1_Low()				P5_bit.no4 = 0U

#define APP_S1IC1_High()			P5_bit.no3 = 1U
#define APP_S1IC1_Low()				P5_bit.no3 = 0U

#define APP_EnableAIC2_Enable()		P7_bit.no4 = 0U
#define APP_EnableAIC2_Disable()	P7_bit.no4 = 1U

#define APP_EnableBIC2_Enable()		P7_bit.no5 = 0U
#define APP_EnableBIC2_Disable()	P7_bit.no5 = 1U

#define APP_S0IC2_High()			P7_bit.no6 = 1U
#define APP_S0IC2_Low()				P7_bit.no6 = 0U

#define APP_S1IC2_High()			P4_bit.no1 = 1U
#define APP_S1IC2_Low()				P4_bit.no1 = 0U

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

typedef enum
{
	RotationSensors = 0,
	PlanarSensors,
	FerriteSensors,
	ExternalSensors,
	RotationDiscCheck
}tSensorsSelector;

/**********************************************************************************************************************
 External global variables adn functions
 *********************************************************************************************************************/

void APP_MuxSelectSensorGroup(tSensorsSelector SensorGroup);
void APP_MuxEnableOutputs(void);
void APP_MuxDisableOutputs(void);
void APP_DisableLEDs(void);
void APP_SetRamDefaultValues(void);

#endif /* APP */

