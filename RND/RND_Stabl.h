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
#ifndef __RND_STABL__
#define __RND_STABL__

/*******************************************************************************
 * pre-include files
 *******************************************************************************/

/*******************************************************************************
 * Configuration
 *******************************************************************************/

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*******************************************************************************
 * types, enums ...
 *******************************************************************************/
/* reduced matrix */
typedef uint16_t t_Msimple[12][2];

typedef struct
{
	t_Msimple left;
	t_Msimple right;
}	t_simple_matrix;

/*******************************************************************************
 * Included Files
 *******************************************************************************/

/*******************************************************************************
 * Exported
 *******************************************************************************/
void		RND_Calc_Normalise			(void);
uint16_t	RND_Calc_Global_Mean		(t_ACQ_Type type);
bool		RND_Calc_MvtDetect			(void);
bool		RND_Calc_IsUserOn			(void);
bool		RND_Calc_IsStable			(uint16_t iter_limit);

#endif
/*** End Of File ***/