/**********************************************************************************************************************
    Program Name    : Sample program for Renesas Flash Driver (RFD RL78 Type01) (Data Flash)

    File Name       : sample_control_data_flash.c
    Program Version : V1.00.00
    Device(s)       : RL78/G23 microcontroller
    Description     : Sample program for data flash control
 **********************************************************************************************************************/

/**********************************************************************************************************************
    DISCLAIMER
    This software is supplied by Renesas Electronics Corporation and is only intended for use with
    Renesas products. No other uses are authorized. This software is owned by Renesas Electronics
    Corporation and is protected under all applicable laws, including copyright laws.
    THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
    TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR
    ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
    BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
    Renesas reserves the right, without notice, to make changes to this software and to discontinue the
    availability of this software. By using this software, you agree to the additional terms and conditions
    found by accessing the following link:
    http://www.renesas.com/disclaimer

    Copyright (C) 2020-2021 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <control_common.h>
#include <control_data_flash.h>
#include <control_defines.h>
#include <control_types.h>
#include "r_rfd_common_api.h"
#include "r_rfd_data_flash_api.h"
#include "r_rfd_code_flash_api.h"

#include "r_cg_macrodriver.h"

/**********************************************************************************************************************
 * Function name : Sample_DataFlashControl
 *********************************************************************************************************************/
#define  SAMPLE_START_SECTION_SMP_DF
#include <control_memmap.h>
#include "r_cg_macrodriver.h"

/*********************************************************************************************************************/
/**
 *  Sample function for data flash control.
 *  BlankCheck -> Erase -> Write
 *  
 *  @param[in]      i_u32_start_addr : 
 *                    Command start address (The start address of the block)
 *                  i_u16_write_data_length : 
 *                    Write data length (MAX : 1 block size)
 *                  inp_u08_write_data : 
 *                    Write data pointer
 *  @return         Execution result status
 *                  - SAMPLE_ENUM_RET_STS_OK               : Successful completion
 *                  - SAMPLE_ENUM_RET_ERR_MODE_MISMATCHED  : Mode mismatched error
 *                  - SAMPLE_ENUM_RET_ERR_CHECK_WRITE_DATA : Check write data error
 *                  - SAMPLE_ENUM_RET_ERR_CMD_ERASE        : Erase command error
 *                  - SAMPLE_ENUM_RET_ERR_CMD_WRITE        : Write command error
 *                  - SAMPLE_ENUM_RET_ERR_CMD_BLANKCHECK   : Blankcheck command error
 */
/*********************************************************************************************************************/
R_RFD_FAR_FUNC e_sample_ret_t Write_DataFlashControl(uint32_t i_u32_start_addr,
		uint16_t i_u16_write_data_length,
		uint8_t __near * inp_u08_write_data)
{
	/* Local variable definition */
	e_rfd_ret_t    l_e_rfd_ret_status;
	e_sample_ret_t l_e_sam_ret_status;
	e_sample_ret_t l_e_sam_ret_value;
	bool           l_e_sam_error_flag;
	uint16_t       l_u16_count;
	uint8_t        l_u08_block_number;
	uint32_t       l_u32_check_write_data_addr;

	/* Set local variables */
	l_e_sam_ret_value           = SAMPLE_ENUM_RET_STS_OK;
	l_e_sam_error_flag          = false;
	/* This expression (actual block number) never exceeds the range of casting uint8_t */
	l_u08_block_number          = (uint8_t)((i_u32_start_addr - SAMPLE_VALUE_U32_DF_BASE_ADDR)
			>> SAMPLE_VALUE_U08_SHIFT_ADDR_TO_BLOCK_DF);
	l_u32_check_write_data_addr = i_u32_start_addr;

	/******************************************************************************************************************
	 * Enable data flash access
	 *****************************************************************************************************************/
	R_RFD_SetDataFlashAccessMode(R_RFD_ENUM_DF_ACCESS_ENABLE);

		/******************************************************************************************************************
		 * Set the data flash programming mode
		 *****************************************************************************************************************/
		l_e_rfd_ret_status = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_DATA_PROGRAMMING);

		if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
		{
			l_e_sam_error_flag = true;
			l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_MODE_MISMATCHED;
		}
		else
		{
			/* No operation */
		}

		/******************************************************************************************************************
		 * BLANKCHECK -> ERASE
		 *****************************************************************************************************************/
		if (false == l_e_sam_error_flag)
		{
			/* BLANKCHECK (1 block) */
			R_RFD_BlankCheckDataFlashReq(l_u08_block_number);
			l_e_sam_ret_status = Sample_CheckCFDFSeqEnd();

			if (SAMPLE_ENUM_RET_ERR_ACT_BLANKCHECK == l_e_sam_ret_status)
			{
				/* ERASE (1 block) */
				R_RFD_EraseDataFlashReq(l_u08_block_number);
				l_e_sam_ret_status = Sample_CheckCFDFSeqEnd();

				if (SAMPLE_ENUM_RET_STS_OK != l_e_sam_ret_status)
				{
					l_e_sam_error_flag = true;
					l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_CMD_ERASE;
				}
				else
				{
					/* No operation */
				}
			}
			else if (SAMPLE_ENUM_RET_STS_OK != l_e_sam_ret_status)
			{
				l_e_sam_error_flag = true;
				l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_CMD_BLANKCHECK;
			}
			else
			{
				/* No operation */
			}
		}
		else /* true == l_e_sam_error_flag */
		{
			/* No operation */
		}

		/******************************************************************************************************************
		 * WRITE
		 *****************************************************************************************************************/
		if (false == l_e_sam_error_flag)
		{
			for (l_u16_count = 0u; l_u16_count < i_u16_write_data_length; l_u16_count++)
			{
				R_RFD_WriteDataFlashReq(i_u32_start_addr + l_u16_count, &inp_u08_write_data[l_u16_count]);
				l_e_sam_ret_status = Sample_CheckCFDFSeqEnd();

				if (SAMPLE_ENUM_RET_STS_OK != l_e_sam_ret_status)
				{
					l_e_sam_error_flag = true;
					l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_CMD_WRITE;
					break;
				}
				else
				{
					/* No operation */
				}
			}
		}
		else /* true == l_e_sam_error_flag */
				{
			/* No operation */
				}

		/******************************************************************************************************************
		 * Set unprogrammable mode
		 *****************************************************************************************************************/
		l_e_rfd_ret_status = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_UNPROGRAMMABLE);

		if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
		{
			l_e_sam_error_flag = true;
			l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_MODE_MISMATCHED;
		}
		else
		{
			/* No operation */
		}

		/******************************************************************************************************************
		 * Check write data
		 *****************************************************************************************************************/
		if (false == l_e_sam_error_flag)
		{
			for (l_u16_count = 0u; l_u16_count < i_u16_write_data_length; l_u16_count++)
			{
				if (inp_u08_write_data[l_u16_count] != (*(uint8_t  *)l_u32_check_write_data_addr))
				{
					l_e_sam_error_flag = true;
					l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_CHECK_WRITE_DATA;
					break;
				}
				else
				{
					/* No operation */
				}
				l_u32_check_write_data_addr++;
			}
		}
		else /* true == l_e_sam_error_flag */
		{
			/* No operation */
		}
	/******************************************************************************************************************
	 * Disable data flash access
	 *****************************************************************************************************************/
	R_RFD_SetDataFlashAccessMode(R_RFD_ENUM_DF_ACCESS_DISABLE);

	return (l_e_sam_ret_value);
}

#define  SAMPLE_END_SECTION_SMP_DF
#include <control_memmap.h>
/**********************************************************************************************************************
 End of function Sample_DataFlashControl
 *********************************************************************************************************************/

