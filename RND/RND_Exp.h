/*******************************************************************************
 * RUNPAD     			                                                       *
 *******************************************************************************
 * Project:		RUNPAD V2
 * Component:	Prototype
 * Element:
 * File:
 * Description:
 *******************************************************************************
 * History:
 *
 * Version      Date        Author      Description
 * --------------------------------------------------------------------------- *
 * 1.0			03/08/2020	LC			Initial
 *******************************************************************************/
#ifndef __RND_EXP_H__
#define __RND_EXP_H__

/*******************************************************************************
 * pre-include files
 *******************************************************************************/

/*******************************************************************************
 * Configuration
 *******************************************************************************/

/*******************************************************************************
 * Definitions
 *******************************************************************************/
#define 	PCAL6408_I2C_ADD		0x40
#define 	PCAL6408_TIMEOUT		100

#define 	PCAL6408_INPUT_PORT		0x00
#define 	PCAL6408_OUTPUT_PORT	0x01
#define 	PCAL6408_POL_INVERT		0x02
#define 	PCAL6408_CONF			0x03
#define 	PCAL6408_OUT_DRIVE_0	0x40
#define 	PCAL6408_OUT_DRIVE_1	0x41
#define 	PCAL6408_IN_LATCH		0x42
#define 	PCAL6408_PULL_EN		0x43
#define 	PCAL6408_PULL_SEL		0x44
#define 	PCAL6408_INT_MSK		0x45
#define 	PCAL6408_INT_STS		0x46
#define 	PCAL6408_OUT_CONF		0x4F

/*******************************************************************************
 * types, enums ...
 *******************************************************************************/

/*******************************************************************************
 * Included Files
 *******************************************************************************/

/*******************************************************************************
 * Exported
 *******************************************************************************/
t_return	RND_EXP_Init			(void);
t_return	RND_EXP_SetLed			(uint8_t ledsts);
t_return	RND_EXP_SetBuzzer		(bool state);
//t_return	RND_EXP_SetCol			(bool colA, bool colB);
t_return	RND_EXP_Beep			(void);

#endif
/*** End Of File ***/
