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
#ifndef __RND_USB_H__
#define __RND_USB_H__

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

/*******************************************************************************
 * Included Files
 *******************************************************************************/

/*******************************************************************************
 * Exported
 *******************************************************************************/
t_return 	RND_Usb_Process			(void);
bool		RND_Usb_IsPresent		(void);
t_return	RND_Usb_Write_Matrix	(char *name, uint16_t *ptr);
t_return	RND_Usb_Write_Matrix_2	( t_measure *p, char *name);
t_return	RND_Usb_Write_Matrix_SIMU	( t_measure *p, char *name);
#endif
/*** End Of File ***/
