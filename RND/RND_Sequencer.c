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

/*******************************************************************************
 * Locals
 *******************************************************************************/
#define _ButtonPressed() 		(BSP_PB_GetState( BUTTON_WAKEUP) != 0)
#define	_WaitButtonPress()		do { while( ! BSP_PB_GetState( BUTTON_WAKEUP) ) \
								osDelay(100); osDelay(200);} while(0);
#define _InitButton()			BSP_PB_Init( BUTTON_WAKEUP, BUTTON_MODE_GPIO);

/*******************************************************************************
 * Globals
 *******************************************************************************/

/*******************************************************************************
 * Externals
 *******************************************************************************/
extern uint16_t right_sensor_tab [TOTAL_LINES][TOTAL_COL/2];
extern uint16_t left_sensor_tab[TOTAL_LINES][TOTAL_COL/2];


/*******************************************************************************
 * Function     : RND_SEQ_Step_T1
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_seq_state
RND_SEQ_Step_T1( void)
{
	LOG("Sequencer state = T1\n");

	RND_Led_SendCommand( T_ETEINT);

	/* print runpad logo */
	osDelay(4*SECOND);

	/* goto main screen */
	RND_Print("main");
	return SEQ_T8;
}

/*******************************************************************************
 * Function     : RND_SEQ_Step_T2
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_seq_state
RND_SEQ_Step_T2( void)
{
uint8_t count = 20;

	LOG("Sequencer state = T2\n");

	RND_Led_SendCommand( T_CHENILLARD_LENT);

	RND_Print( "Scannez le\nQR Code\n");

//	_WaitButtonPress();
	osDelay(3*SECOND);

	while( count)
	{
		if( RND_Calc_IsUserOn() == TRUE)
			return SEQ_T3;

		osDelay(100);
		count --;
	}

	return SEQ_T7;
}

/*******************************************************************************
 * Function     : RND_SEQ_Step_T3
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_seq_state
RND_SEQ_Step_T3( void)
{
t_measure	Measure = {0};

	LOG("Sequencer state = T3\n");

	RND_Led_SendCommand( T_ALLUMAGE_FIXE);
	RND_Print( "Stabilisez\nvotre position\n");

	/* wait user is stable on device */
	//while( RND_Calc_IsStable(5) == FALSE)
	//{
	//	osDelay(100);
	//	if( RND_Calc_IsUserOn() == FALSE)
	//		return SEQ_T2;
	//}

	/* do calc and print results */
	//RND_Print("Mesures de la\npronation"); osDelay(SECOND);
	RND_Gvt_Get( &Measure);

	//RND_Print("Mesures de la\npointure");osDelay(SECOND);
	RND_Size_Get( &Measure);


	for (uint8_t i = 0; i < 2; i++)
	{
		if (Measure.pronation == NEUTRE_t)
			RND_Print("TYPE DE\nFOULÉE\nNEUTRE");
		else if (Measure.pronation == CONTROL_t)
			RND_Print("TYPE DE\nFOULÉE\nCONTROLE");
		else if (Measure.pronation == SUPINAL_t)
			RND_Print("TYPE DE\nFOULÉE\nSUPINAL");

		osDelay(3 * SECOND);
		RND_Print("POINTURE\nRUNNING\n%4.1f", Measure.pointure);
		osDelay(3 * SECOND);
		RND_Print("GRAVITY\nDROP\n%d", Measure.igvt);
		osDelay(3 * SECOND);
	}

	RND_Calc_AcqCsv( &Measure);

	return SEQ_T4;
}

/*******************************************************************************
 * Function     : RND_SEQ_Step_T4
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_seq_state
RND_SEQ_Step_T4( void)
{
	LOG("Sequencer state = T4\n");

	/* wait user gets off device */
	while( RND_Calc_IsUserOn() == TRUE)
		osDelay(600);

	RND_Led_SendCommand( T_ETEINT);

	//return SEQ_T5;
	return SEQ_T7;
}

/*******************************************************************************
 * Function     : RND_SEQ_Step_T5
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_seq_state
RND_SEQ_Step_T5( void)
{
uint8_t count = 10;

	LOG("Sequencer state = T5\n");
	//RND_EXP_SetBuzzer( TRUE);

	RND_Led_SendCommand( T_CHENILLARD_LENT_INVERSE);
	RND_Print( "Nouvelle\nMesure ?\n");

//	_WaitButtonPress();
	while(  count)
	{
		if( RND_Calc_IsUserOn() == TRUE)
			return SEQ_T3;

		osDelay( 200);
		count --;
	}

	return SEQ_T6;
}

/*******************************************************************************
 * Function     : RND_SEQ_Step_T6
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_seq_state
RND_SEQ_Step_T6( void)
{
	LOG("Sequencer state = T6\n");
	//RND_EXP_SetBuzzer( FALSE);

	RND_Led_SendCommand( T_CHENILLARD_LENT);
	RND_Print("Scannez le\nQR Code\n");

	osDelay( 3*SECOND);
	//RND_Print(" ");

	return SEQ_T7;
}

/*******************************************************************************
 * Function     : RND_SEQ_Step_T7
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_seq_state
RND_SEQ_Step_T7( void)
{
uint8_t cnt = 0;

	LOG("Sequencer state = T7\n");

	RND_Led_SendCommand( T_CHENILLARD_RAPIDE);

	/* goto intro screen */
	RND_Print("start");

	_goto_low_power();

	while( (RND_Calc_IsUserOn() == FALSE) && (cnt < 15))
	{
		osDelay(SECOND);
		cnt ++;
	}

	_resume_from_low__power();

	/* goto intro screen */
	RND_Print("start");

	return SEQ_T1;
}

/*******************************************************************************
 * Function     : RND_SEQ_Step_T7
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_seq_state
RND_SEQ_Step_T8( void )
{
	t_measure	Measure = {0};

	LOG("Sequencer state = T8\n");

	RND_Print("\nstart measure");
	osDelay(SECOND);

	RND_Print( "\nmeasuring");
	RND_send_measure_and_size_get( &Measure );
	RND_Print("POINTURE\nRUNNING\n%4.1f", Measure.pointure);
	osDelay(3 * SECOND);

	return SEQ_T8;
}
/*******************************************************************************
 * Function     : RND_SEQ_Init
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
RND_SEQ_Init( void)
{
	_InitButton();
	return E_OK;
}

/*******************************************************************************
 * Function     : RND_SEQ_Sequencer
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
void
RND_SEQ_Sequencer( void)
{
	static t_seq_state seq_state = SEQ_T1; //SEQ_T1;

	while( TRUE ){
		switch (seq_state)
		{
			case SEQ_T1:	seq_state = RND_SEQ_Step_T1(); 		break;
			case SEQ_T2:	seq_state = RND_SEQ_Step_T2();		break;
			case SEQ_T3:	seq_state = RND_SEQ_Step_T3();		break;
			case SEQ_T4:	seq_state = RND_SEQ_Step_T4();		break;
			case SEQ_T5:	seq_state = RND_SEQ_Step_T5();		break;
			case SEQ_T6:	seq_state = RND_SEQ_Step_T6();		break;
			case SEQ_T7:	seq_state = RND_SEQ_Step_T7();		break;
			case SEQ_T8:	seq_state = RND_SEQ_Step_T8();		break;
			default:		break;
		}
	}
}

/*** End Of File ***/
