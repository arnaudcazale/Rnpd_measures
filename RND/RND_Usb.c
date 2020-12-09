/*******************************************************************************
 * RUNPAD     			                                                       *
 *******************************************************************************
 * Project:		RUNPAD V2
 * Component:	Prototype
 * Element:
 * File:
 * Description:
 *******************************************************************************
 * Historique:
 *
 * Version      Date        Author      Description
 * --------------------------------------------------------------------------- *
 * 1.0
 *******************************************************************************/
#define LOGGING

/*******************************************************************************
 * Included Files
 *******************************************************************************/
#include "RND_Main.h"
#include "usb_host.h"
//#include "usbh_diskio.h"
#include "fatfs.h"

/*******************************************************************************
 * Locals
 *******************************************************************************/
char buff[80];

/*******************************************************************************
 * Globals
 *******************************************************************************/
bool					FS_Connected = FALSE;

/*******************************************************************************
 * Externals
 *******************************************************************************/
extern ApplicationTypeDef 	Appli_state;
extern uint8_t retUSBH;    /* Return value for USBH */
extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */
extern uint16_t right_sensor[ TOTAL_SENSORS_PER_SIDE];
extern uint16_t left_sensor [ TOTAL_SENSORS_PER_SIDE];

/*******************************************************************************
 * Function     : RND_Usb_IsPresent
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
bool
RND_Usb_IsPresent( void)
{
	return (FS_Connected == TRUE);
}

/*******************************************************************************
 * Function     : RND_Usb_Process
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
RND_Usb_Process( void)
{
	//osDelay(SECOND);

	while(TRUE)
	{
		osDelay( SECOND / 4);

		MX_USB_HOST_Process();

		switch( Appli_state)
		{
		case APPLICATION_START:
			break;

		case APPLICATION_DISCONNECT:
			/* device disconected from USB */
			if (f_mount(NULL, "", 0) != FR_OK)
			{
				LOG("Error: cannot deinitialize fatfs\n");
			}

			if( FATFS_UnLinkDriver( USBHPath) != 0)
			{
				LOG("Error: cannot unlink USB fatfs driver\n");
			}

			LOG("Umounted USB Driver\n");
			FS_Connected = FALSE;
			Appli_state = APPLICATION_IDLE;
			break;

		case APPLICATION_READY:
			/* device connect to usb */
			if( FATFS_LinkDriver( &USBH_Driver, USBHPath) != 0)
			{
				LOG("Failed to attach driver\n");
				FS_Connected = FALSE;
				Appli_state = APPLICATION_IDLE;
				break;
			}

			if( f_mount( &USBHFatFS, USBHPath, 1) != FR_OK)
			{
				LOG("Failed to mount file system\n");
				FS_Connected = FALSE;
				Appli_state = APPLICATION_IDLE;
				break;
			}

			LOG("Attached file system\n");
			FS_Connected = TRUE;
			Appli_state = APPLICATION_IDLE;
			break;

		case APPLICATION_IDLE:
			break;
		}
	}
}

/*******************************************************************************
 * Function     : RND_Usb_Write_Matrix
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
RND_Usb_Write_Matrix( char *name, uint16_t *ptr)
{
t_return ret = E_ERROR;
FRESULT	fr;
FIL		fp;

	if( FS_Connected != TRUE)
	{
		//LOG("No file system attached\n");
		goto end;
	}

	LOG("Writting data to \"%s\" file\n", name);

	/* open file */
	fr = f_open( &fp, name, FA_CREATE_ALWAYS | FA_WRITE);
	if( fr != FR_OK)
	{
		LOG("Failed to open/create \"%s\"\n", name);
		goto end;
	}

//	f_printf( &fp, "\n#include \"RND_Main.h\"\n");
//	f_printf( &fp, "\n\nuint16_t left_sensor_tab[] = \n{\n");

	for( uint16_t i = 0; i < TOTAL_SENSORS_PER_SIDE; i++)
	{
//		f_printf( &fp, "%d", ptr[i]);
//		if( i && !((i+1)%8) )
//			f_printf( &fp, "\n");
//		else
//			f_printf( &fp, ",");

		f_printf( &fp, "\"%d\"", ptr[i]);
		if( i && !((i+1)%8) )
			f_printf( &fp, "\n");
		else
			f_printf( &fp, ";");
	}

//	f_printf( &fp, "};\n");

	f_sync( &fp);
	f_close( &fp);
	ret = E_OK;

end:
	return ret;
}

/*******************************************************************************
 * Function     : RND_Usb_Write_Matrix_2
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
RND_Usb_Write_Matrix_2( t_measure *p, char *name)
{
t_return ret = E_ERROR;
FRESULT	fr;
FIL		fp;
uint16_t	index;

	if( FS_Connected != TRUE)
		goto end;

	LOG("Writting data to \"%s\" file\n", name);

	/* open file */
	fr = f_open( &fp, name, FA_CREATE_ALWAYS | FA_WRITE);
	if( fr != FR_OK)
	{
		LOG("Failed to open/create \"%s\"\n", name);
		goto end;
	}

	f_printf( &fp, "\n");
	sprintf( buff, "%5.2f", p->left_angle); f_printf( &fp, "left_angle = %s\n", buff);
	f_printf( &fp, "left_hi = %d\n", p->left_hi);
	f_printf( &fp, "left_lo = %d\n", p->left_lo);
	sprintf( buff, "%5.2f", p->left_size); f_printf( &fp, "left_size = %s\n", buff);
	f_printf( &fp, "left_extern_pressure = %d\n", p->left_extern_pressure);
	f_printf( &fp, "left_intern_pressure = %d\n", p->left_intern_pressure);
	f_printf( &fp, "\n");
	sprintf( buff, "%5.2f", p->right_angle); f_printf( &fp, "right_angle = %s\n", buff);
	f_printf( &fp, "right_hi = %d\n", p->right_hi);
	f_printf( &fp, "right_lo = %d\n", p->right_lo);
	sprintf( buff, "%5.2f", p->right_size); f_printf( &fp, "right_size = %s\n", buff);
	f_printf( &fp, "right_extern_pressure = %d\n", p->right_extern_pressure);
	f_printf( &fp, "right_intern_pressure = %d\n", p->right_intern_pressure);
	f_printf( &fp, "\n");
	sprintf( buff, "%5.2f", p->pointure); f_printf( &fp, "pointure = %s\n", buff);
	sprintf( buff, "%5.2f", p->gvt); f_printf( &fp, "gvt = %s\n", buff);
	f_printf( &fp, "igvt = %d\n", p->igvt);
	if( p->pronation == NEUTRE_t)
	{
		f_printf( &fp, "pronation = NEUTRE\n");
	}
	else if( p->pronation == CONTROL_t)
	{
		f_printf( &fp, "pronation = CONTROL\n");
	}
	else if( p->pronation == SUPINAL_t)
	{
		f_printf( &fp, "pronation = SUPINAL\n");
	}
	f_sync( &fp);
	f_printf( &fp, "\n\n");

	for( uint8_t lines = 0; lines < TOTAL_LINES; lines ++)
	{
		f_printf( &fp, ";;;");

		for( uint8_t cols = 0; cols < TOTAL_COL; cols ++)
		{
			index = INDEX_FROM_LINE_COL( lines, cols);

			if( cols < TOTAL_COL / 2)
			{
				f_printf( &fp, "\"%d\"", left_sensor[index]);
				if( index & !((index+1)%8))
					f_printf( &fp, ";;");
				else
					f_printf( &fp, ";");
			}
			else
			{
				f_printf( &fp, "\"%d\"", right_sensor[index]);
				if( index & !((index+1)%8))
					f_printf( &fp, "\n");
				else
					f_printf( &fp, ";");
			}
		}
	}

	f_sync( &fp);
	f_close( &fp);
	ret = E_OK;

end:
	return ret;
}

/*******************************************************************************
 * Function     : RND_Usb_Write_Matrix_2
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
RND_Usb_Write_Matrix_SIMU( t_measure *p, char *name)
{
t_return ret = E_ERROR;
FRESULT	fr;
FIL		fp;
uint16_t	index;
extern uint16_t left_sensor_tab[];
extern uint16_t right_sensor_tab[];

	if( FS_Connected != TRUE)
		goto end;

	LOG("Writting data to \"%s\" file\n", name);

	/* open file */
	fr = f_open( &fp, name, FA_CREATE_ALWAYS | FA_WRITE);
	if( fr != FR_OK)
	{
		LOG("Failed to open/create \"%s\"\n", name);
		goto end;
	}

	f_printf( &fp, "\n");
	sprintf( buff, "%5.2f", p->left_angle); f_printf( &fp, "left_angle = %s\n", buff);
	f_printf( &fp, "left_hi = %d\n", p->left_hi);
	f_printf( &fp, "left_lo = %d\n", p->left_lo);
	sprintf( buff, "%5.2f", p->left_size); f_printf( &fp, "left_size = %s\n", buff);
	f_printf( &fp, "left_extern_pressure = %d\n", p->left_extern_pressure);
	f_printf( &fp, "left_intern_pressure = %d\n", p->left_intern_pressure);
	f_printf( &fp, "\n");
	sprintf( buff, "%5.2f", p->right_angle); f_printf( &fp, "right_angle = %s\n", buff);
	f_printf( &fp, "right_hi = %d\n", p->right_hi);
	f_printf( &fp, "right_lo = %d\n", p->right_lo);
	sprintf( buff, "%5.2f", p->right_size); f_printf( &fp, "right_size = %s\n", buff);
	f_printf( &fp, "right_extern_pressure = %d\n", p->right_extern_pressure);
	f_printf( &fp, "right_intern_pressure = %d\n", p->right_intern_pressure);
	f_printf( &fp, "\n");
	sprintf( buff, "%5.2f", p->pointure); f_printf( &fp, "pointure = %s\n", buff);
	sprintf( buff, "%5.2f", p->gvt); f_printf( &fp, "gvt = %s\n", buff);
	f_printf( &fp, "igvt = %d\n", p->igvt);
	if( p->pronation == NEUTRE_t)
	{
		f_printf( &fp, "pronation = NEUTRE\n");
	}
	else if( p->pronation == CONTROL_t)
	{
		f_printf( &fp, "pronation = CONTROL\n");
	}
	else if( p->pronation == SUPINAL_t)
	{
		f_printf( &fp, "pronation = SUPINAL\n");
	}
	f_sync( &fp);
	f_printf( &fp, "\n\n");

	for( uint8_t lines = 0; lines < TOTAL_LINES; lines ++)
	{
		f_printf( &fp, ";;;");

		for( uint8_t cols = 0; cols < TOTAL_COL; cols ++)
		{
			index = INDEX_FROM_LINE_COL( lines, cols);

			if( cols < TOTAL_COL / 2)
			{
				f_printf( &fp, "\"%d\"", left_sensor_tab[index]);
				if( index & !((index+1)%8))
					f_printf( &fp, ";;");
				else
					f_printf( &fp, ";");
			}
			else
			{
				f_printf( &fp, "\"%d\"", right_sensor_tab[index]);
				if( index & !((index+1)%8))
					f_printf( &fp, "\n");
				else
					f_printf( &fp, ";");
			}
		}
	}

	f_sync( &fp);
	f_close( &fp);
	ret = E_OK;

end:
	return ret;
}

/*** End Of File ***/
