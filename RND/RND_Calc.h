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
#ifndef __RND_CALC_H__
#define __RND_CALC_H__

/*******************************************************************************
 * pre-include files
 *******************************************************************************/

/*******************************************************************************
 * Configuration
 *******************************************************************************/

/*******************************************************************************
 * Definitions
 *******************************************************************************/
#define		TOTAL_LINES					48
#define 	TOTAL_COL					16
#define		TOTAL_SENSORS				(TOTAL_LINES * TOTAL_COL)
#define		TOTAL_SENSORS_PER_SIDE		((TOTAL_LINES * TOTAL_COL)/2)

#define 	TOTAL_LINES_RED				(TOTAL_LINES / 4)
#define 	TOTAL_COL_RED				TOTAL_COL
#define 	TOTAL_SENSORS_RED			(TOTAL_LINES_RED * TOTAL_COL_RED)
#define 	TOTAL_SENSORS_RED_PER_SIDE	(TOTAL_SENSORS_RED / 2)

#define 	LINE_FROM_INDEX(a)			((uint8_t)(a/8) % TOTAL_LINES)
#define 	COL_FROM_INDEX(a)			((a<TOTAL_SENSORS_PER_SIDE) ? (a%(TOTAL_COL/2)) : (a%(TOTAL_COL/2))+8)
#define 	COL_GROUP_FROM_INDEX(a)		(uint8_t)(COL_FROM_INDEX(a)/4)
#define 	INDEX_FROM_LINE_COL(line,col) (col < TOTAL_COL/2) ? ((line*8)+col) : ((line*8)+col-8)

/*******************************************************************************
 * types, enums ...
 *******************************************************************************/
typedef	enum
{
	t_ACQ_FULL,
	t_ACQ_REDUCED
}	t_ACQ_Type;

typedef uint16_t (t_acq_tab)[TOTAL_SENSORS_PER_SIDE];
typedef uint16_t (t_acq_mat)[TOTAL_LINES][TOTAL_COL/2];
typedef uint8_t  (t_bin_mat)[TOTAL_LINES][TOTAL_COL/2];

typedef struct
{
	t_acq_tab 	left;
	t_acq_tab	right;
}	t_acq;

/*******************************************************************************
 * Included Files
 *******************************************************************************/

/*******************************************************************************
 * Exported
 *******************************************************************************/
t_return	RND_Calc_Init			(void);
t_return	RND_Acq_Frame			(t_ACQ_Type type);
t_return	RND_Acq_Multiple		(t_acq *p,  uint8_t n_acq);
t_return	RND_Acq_Multiple_Start	(uint8_t n_acq);
t_return	RND_Acq_Multiple_End	(t_acq *p, uint8_t acq_done, uint8_t total_acq);
t_return	RND_Calc_AcqCsv			(t_measure *p);

#endif
/*** End Of File ***/
