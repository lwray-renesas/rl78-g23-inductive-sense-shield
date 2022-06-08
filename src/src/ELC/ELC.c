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
 * File Name    : ELC.c
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
extern tSensorsSelector enSensorsSelector;

/*select the sensor group using input pin switching*/
void ELC_SelectSensorGroup(void)
{

	if(enSensorsSelector == RotationSensors)
	{
		P2_bit.no0 = 1;
		P2_bit.no1 = 1;
		P2_bit.no2 = 0;
	}
	else if(enSensorsSelector == PlanarSensors)
	{
		P2_bit.no0 = 1U;
		P2_bit.no1 = 0U;
		P2_bit.no2 = 1U;
	}

	else if (enSensorsSelector == FerriteSensors)
	{
		P2_bit.no0 = 0U;
		P2_bit.no1 = 1U;
		P2_bit.no2 = 1U;
	}
}

/*init the ELC sources and outputs*/
void ELC_InitSensorGroups(void)
{
	PFOE0 &= 0xD5; 		//select TAU3 and TAU5 as ELCL input
	ELISEL0 = 0x15;		//input P20 - ferrite sensors
	ELISEL1 = 0x15;		//input P21 - planar sensors
	ELISEL2 = 0x15; 	//input P22 - rotation sensors

	ELISEL3 = 0x0C;     //input TAU5
	ELISEL4 = 0x0A;		//input TAU3

	/* ferrite sensors logic1 block0 */
	ELL1SEL0 = 0x01;	//input register 0 to be linked - P20
	ELL1SEL1 = 0x05;	//input register 3 to be linked - TAU5

	/* ferrite reference logic1 block1 */
	ELL1SEL2 = 0x01;	//input register 1 to be linked - P20
	ELL1SEL3 = 0x04;	//input register 3 to be linked - TAU3

	ELL1CTL = 0x0A;		//'OR' circuit for both of cell0 and cell1 for block L1
	ELL1LNK0 = 0x01;
	ELL1LNK1 = 0x02;
	ELL1LNK2 = 0x03;
	ELL1LNK3 = 0x04;

	/* planar sensors logic2 block0 */
	ELL2SEL0 = 0x02;	//input register 0 to be linked - P21
	ELL2SEL1 = 0x05;	//input register 3 to be linked - TAU5

	/* planar reference logic2 block1 */
	ELL2SEL2 = 0x02;	//input register 1 to be linked - P21
	ELL2SEL3 = 0x04;	//input register 3 to be linked - TAU3

	ELL2CTL = 0x0A;		//'OR' circuit for both of cell0 and cell1 for block L2
	ELL2LNK0 = 0x01;
	ELL2LNK1 = 0x02;
	ELL2LNK2 = 0x03;
	ELL2LNK3 = 0x04;

	/* rotation sensors logic3 block0 */
	ELL3SEL0 = 0x03;	//input register 0 to be linked - P22
	ELL3SEL1 = 0x05;	//input register 3 to be linked - TAU5

	/* rotation reference logic3 block1 */
	ELL3SEL2 = 0x03;	//input register 1 to be linked - P22
	ELL3SEL3 = 0x04;	//input register 3 to be linked - TAU3

	ELL3CTL = 0x0A;		//'OR' circuit for both of cell0 and cell1 for block L3
	ELL3LNK0 = 0x01;
	ELL3LNK1 = 0x02;
	ELL3LNK2 = 0x03;
	ELL3LNK3 = 0x04;
}

/*select the output group and disable the unused pins*/
void ELC_SelectOutputGroup(void)
{
	if(enSensorsSelector == RotationSensors)
	{
		/* disable all ELC channels */
		PMCE0 = 0x00;
		PMCE1 = 0x00;
		PMCE5 = 0x00;

		/* stop output */
		ELOENCTL = 0x00;

		/* select linker source  - rotation sensors and reference */
		ELOSEL0 = 0x0B;		//L3 cell0 - rotation sensors
		ELOSEL5 = 0x0C; 	//L3 cell1 - rotation reference

		/* enable output */
		ELOENCTL = 0x21;

		/* enable ELC channels */
		//PMCE0 |= 0x02; //P01
		PMCE1 |= 0x20; //P15
		PMCE0 |= 0x02; //P01
	}

	else if(enSensorsSelector == PlanarSensors)
	{
		/* disable all ELC channels */
		PMCE0 = 0x00;
		PMCE1 = 0x00;
		PMCE5 = 0x00;

		/* stop output */
		ELOENCTL = 0x00;

		/* select linker source  - ferrite + planar sensors and reference */
		ELOSEL0 = 0x01; 	//L1 cell0 - ferrite + external sensors
		ELOSEL1 = 0x06; 	//L2 cell0 - planar sensors
		ELOSEL6 = 0x07; 	//L2 cell1 - planar reference
		ELOSEL7 = 0x02; 	//L1 cell1 - ferrite + external reference

		/* enable output */
		ELOENCTL = 0xC3;

		/* enable ELC channels */
		PMCE1 |= 0xC1; //P10, P16, P17
		PMCE5 |= 0x02; //P51
	}

	else if(enSensorsSelector == FerriteSensors)
	{
		/* disable all ELC channels */
		PMCE0 = 0x00;
		PMCE1 = 0x00;
		PMCE5 = 0x00;

		/* stop output */
		ELOENCTL = 0x00;

		/* select linker source  - ferrite + planar sensors and reference */
		ELOSEL0 = 0x01; 	//L1 cell0 - ferrite + external sensors
		ELOSEL1 = 0x06; 	//L2 cell0 - planar sensors
		ELOSEL6 = 0x07; 	//L2 cell1 - planar reference
		ELOSEL7 = 0x02; 	//L1 cell1 - ferrite + external reference

		/* enable output */
		ELOENCTL = 0xC3;

		/* enable ELC channels */
		PMCE1 |= 0xC1; //P10, P16, P17
		PMCE5 |= 0x02; //P51
	}

	else if(enSensorsSelector ==  ExternalSensors)
	{
		//do nothing
	}

	else if(enSensorsSelector ==  RotationDiscCheck)
	{
		/* disable all ELC channels */
		PMCE0 = 0x00;
		PMCE1 = 0x00;
		PMCE5 = 0x00;

		/* stop output */
		ELOENCTL = 0x00;

		/* select linker source  - rotation sensors and reference */
		ELOSEL4 = 0x0B;		//L3 cell0 - rotation sensor 3
		ELOSEL5 = 0x0C; 	//L3 cell1 - rotation reference

		/* enable output */
		ELOENCTL = 0x30;

		/* enable ELC channels */
		PMCE1 |= 0x30; //P14; P15
	}
}
