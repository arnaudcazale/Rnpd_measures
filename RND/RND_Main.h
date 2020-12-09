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
#ifndef __RND_MAIN_H__
#define __RND_MAIN_H__

/*******************************************************************************
 * pre-include files
 *******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

/*******************************************************************************
 * Configuration
 *******************************************************************************/

/*******************************************************************************
 * Definitions
 *******************************************************************************/
#define	HW_MAJOR_VERSION		0
#define HW_MINOR_VERSION		9
#define SW_MAJOR_VERSION		0
#define SW_MINOR_VERSION		93

#define	REAL_HARDWARE			1
#define OPTIMIZE				1
#define SIMULATION				0

/*******************************************************************************
 * types, enums ...
 *******************************************************************************/
typedef enum
{
	E_ERROR,
	//E_SUCCESS,
	E_OK,
	E_TIMEOUT
}	t_return;

typedef enum
{
	NEUTRE_t,
	CONTROL_t,
	SUPINAL_t,
}	t_pronation;

typedef struct
{
	double 		left_angle;
	double 		left_size;
	uint8_t		left_hi;
	uint8_t		left_lo;

	uint32_t	left_extern_pressure;
	uint32_t	left_intern_pressure;

	double 		right_angle;
	double 		right_size;
	uint8_t		right_hi;
	uint8_t		right_lo;

	uint32_t	right_extern_pressure;
	uint32_t	right_intern_pressure;

	double		pointure;
	t_pronation	pronation;
	double		gvt;
	uint8_t 	igvt;

}	t_measure;

#ifndef __cplusplus
#if !defined(TRUE) && !defined(FALSE)
typedef	enum
{
	TRUE = 1,
	FALSE = 0,
}	bool;
#else
typedef	_Bool 	bool;
#endif
#else
#define TRUE	true
#define FALSE	false
#endif

#define	SECOND				(1000)
#define Byte_0(a)			((a>>24)&0xff)
#define Byte_1(a)			((a>>16)&0xff)
#define Byte_2(a)			((a>>8)&0xff)
#define Byte_3(a)			(a&0xff)
#define Word_0(a)			((a>>16)&0xffff)
#define Word_1(a)			(a&0xffff)
#define LOW_WORD(a)			(Word_0(a))
#define HIGH_WORD(a)		(Word_1(a))

#define _priority_fix(a) 	(a + abs( tskIDLE_PRIORITY-osPriorityIdle))
#define _IN_DATA_RAM		__attribute__((section(".dtaram")))

#define __OPTIMIZE_START	_Pragma("GCC push_options") \
							_Pragma("GCC optimize (\"O3\")")
#define __OPTIMIZE_END		_Pragma("GCC pop_options")

#define _MAX(x,y) ( \
    { __auto_type __x = (x); __auto_type __y = (y); \
      __x > __y ? __x : __y; })

/*******************************************************************************
 * Included Files
 *******************************************************************************/
#include "main.h"
#include "stm32469i_discovery.h"
#include "stm32469i_discovery_sdram.h"
#include "stm32469i_discovery_qspi.h"
#include "cmsis_os.h"
#include "time.h"
#include "RND_Log.h"
#include "RND_Led.h"
#include "RND_I2c.h"
#include "RND_Disp.h"
#include "RND_Print.h"
#include "RND_Sequencer.h"
#include "RND_Calc.h"
#include "RND_Exp.h"
#include "RND_MAT.h"
#include "RND_Stabl.h"
#include "RND_Usb.h"
#include "RND_Size.h"
#include "RND_Gvt.h"
#include "MAX17262.h"

/*******************************************************************************
 * Exported
 *******************************************************************************/
t_return	RND_Init				(void);
void		RND_Main_App			(void);
void		_goto_low_power			(void);
void		_resume_from_low__power	(void);
void 		DWT_Delay_us			(volatile uint32_t microseconds);
uint32_t	DWT_Get					(void);
char *		RND_GetVersionString	(void);

#endif
/*** End Of File ***/
