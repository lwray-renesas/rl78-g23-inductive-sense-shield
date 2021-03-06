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
 *  File Name    : RL78G23_Inductive_Sensor_LLVM.c
 *  Description  : Main Program
 *  Creation Date: 2021-05-21
 *  This file was generated by Smart Configurator.
 ***********************************************************************************************************************/
#include <control_data_flash.h>
#include <control_defines.h>
#include "control_config.h"
#include "r_smc_entry.h"
#include "r_rfd_common_api.h"
#include "read_data_flash.h"

#include "ELC.h"
#include "APP.h"
#include "TAU.h"
#include "DIAG.h"
#include "Config_TAU0_4.h"
#include "Config_TAU0_2.h"
#include "Config_TAU0_0.h"
#include "Config_UART0.h"
#include "r_cg_macrodriver.h"
#include "r_smc_entry.h"

extern tSensorsSelector enSensorsSelector;
extern volatile uint8_t DIAG_EventState;
extern TAU_stTimingValue TAU_tstTimingValue;
extern volatile uint8_t UART_u8State;

volatile uint8_t u8MainStopState = 0U;

/*code version 1.6
 * excitation pulses based on timers and ELC switching
 * Multiplexer IC control based on output ports
 * Interval Timer wake up based on IT interrupt - SMS deactivated
 * STOP mode after measurement
 * UART communication enabled in normal and snooze mode
 * Data Flash implemented:
 * 			 - read at initialization
 * 			 - check if memory is erased and replace with default RAM values
 * 			 - write based on UART command*/
int main(void);
void R_Main_Init();

int main(void)
{
	R_Main_Init();
	EI();

	while (1)
	{
		/*set in STOP mode after all measurements are done and UART communication is disabled*/
		if ((1 == u8MainStopState) && (UART_u8State == UART_Disconnected))
		{
			u8MainStopState = 0U;

			/*check the serial status register*/
			while( ((SSR00 & 0x0040U) > 0U) || ((SSR01 & 0x0040U) > 0U) )
			{
				NOP();
			}

			/*switch the UART to snooze mode*/
			R_Config_UART0_Stop();
			/* Enable Snooze mode but disable generation of error interrupts*/
			SSC0 |= 0x0003U;
			R_Config_UART0_Start();

			/*enable stop mode*/
			STOP();
		}
	}
	return 0;
}

/* init function */
void R_Main_Init()
{
	/*local variable for read Data Flash process*/
	e_rfd_ret_t    l_e_rfd_status_flag;

	/*init data flash*/
	l_e_rfd_status_flag = R_RFD_Init(CPU_FREQUENCY);

	/*check the data flash initialization state*/
	if(R_RFD_ENUM_RET_STS_OK == l_e_rfd_status_flag)
	{
		/*update the timing structure with data flash values*/
		vReadDataFlash();
	}
	else
	{
		/*wrong initialization of data flash memory*/
		return;
	}

	/*init interrupts*/
	EI();

	/* select sensor group */
	enSensorsSelector = RotationSensors;

	/*define diagnosis state*/
	DIAG_EventState = DIAG_EventNoStates;

	/*set interval timer period*/
	ITLCMP00 = TAU_tstTimingValue.u16IntervalTimer;

	/*init IT - required for real interrupt */
	R_ITL_Set_PowerOn();
	R_ITL_Start_Interrupt();
	/*start 32bit IT - 125ms*/
	R_Config_ITL000_ITL001_Start();

	/*init shield LEDs */
	APP_DisableLEDs();

	/* start comparators */
	R_Config_COMP0_Start();
	R_Config_COMP1_Start();

	/*define registers for ELC algorithm */
	ELC_InitSensorGroups();
	/* enable rotation output */
	ELC_SelectOutputGroup();

	/*set selection channels for rotation sensors group */
	APP_MuxSelectSensorGroup(enSensorsSelector);

	/*update custom timing for rotation sensor group */
	TAU_UpdateTimerValues(enSensorsSelector);

	/*start UART communication -enable UART peripheral and interrupts*/
	R_Config_UART0_Start();

	/*init measurement timers*/
	R_Config_TAU0_0_Start();
	R_Config_TAU0_2_Start();
	R_Config_TAU0_4_Start();
}
