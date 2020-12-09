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
static t_acq_mat	left_filtered;
static t_acq_mat	right_filtered;

static t_acq		data;

#if (OPTIMIZE==1)
__OPTIMIZE_START
#endif

/*******************************************************************************
 * Globals
 *******************************************************************************/

/*******************************************************************************
 * Externals
 *******************************************************************************/
extern uint8_t left_bin [TOTAL_LINES][TOTAL_COL/2];
extern uint8_t right_bin[TOTAL_LINES][TOTAL_COL/2];
//extern uint16_t right_sensor[TOTAL_SENSORS_PER_SIDE];
//extern uint16_t left_sensor [TOTAL_SENSORS_PER_SIDE];

#if (SIMULATION==1)
extern uint16_t right_sensor_tab[];
extern uint16_t left_sensor_tab[];
#endif

/*******************************************************************************
 * Function     : 
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
void
_filter_mat( t_acq_tab inTab, t_bin_mat bin, t_acq_mat outMat)
{
t_acq_mat *p = (t_acq_mat *)inTab;
register int i,j;

	for( i = 0; i < TOTAL_LINES; i++)
		for( j = 0; j < TOTAL_COL/2; j++)
			outMat[i][j] = bin[i][j] ? (*p)[i][j] : 0;

	return;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
uint32_t
_sum_mat( t_acq_mat in, uint8_t start_line, uint8_t end_line)
{
uint32_t sum = 0;
register int i,j;

	for( i = start_line; i < end_line; i++)
		for( j = 0; j < TOTAL_COL / 2; j++)
			sum += in[i][j];

	return sum;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
void
_cal_pron( t_acq_mat in, uint8_t start_line, uint8_t end_line, uint32_t *lpres, uint32_t *rpres)
{
//uint32_t lp,rp;
//register int i,j;
//uint8_t start, end, med_idx;


//	lp = rp = 0;
//
//	for( i = start_line; i < end_line; i++)
//	{
//		/* get median idex */
//		start = 0; end = 0;
//
//		for( j = 0; j < TOTAL_COL/2; j ++)
//		{
//			if( in[i][j])
//			{
//				start = j;
//				break;
//			}
//		}
//
//		for( j = TOTAL_COL/2 -1; j >= 0; j --)
//		{
//			if( in[i][j])
//			{
//				end = j;
//				break;
//			}
//		}
//
//		med_idx = start + ((end - start)/2);
//
//		/* sum up each side */
//		for( j = 0; j < TOTAL_COL/2; j++)
//		{
//			if( j < med_idx)
//				lp += in[i][j];
//			else if ( j > med_idx)
//				rp += in[i][j];
//		}
//	}
//	*lpres = lp;
//	*rpres = rp;


	uint32_t lp, rp;
	register int i,j;
	uint8_t top_i = start_line, top_j = TOTAL_COL/2;
	uint8_t bottom_i = end_line, bottom_j = TOTAL_COL/2;
	bool found = FALSE;

	double a,b;


		lp = rp = 0;

		for( i = start_line; (i < end_line) && (found == FALSE); i++)
		{
			for( j = 0; j < TOTAL_COL/2; j++)
			{
				if( in[i][j])
				{
					top_i = i;
					top_j = j;
					found = TRUE;
					break;
				}
			}
		}

		found = FALSE;
		for( i = end_line - 1; (i >= start_line) && (found == FALSE); i--)
		{
			for( j = 0; j < TOTAL_COL/2; j++)
			{
				if( in[i][j])
				{
					bottom_i = i;
					bottom_j = j;
					found = TRUE;
					break;
				}
			}
		}

		a = ((double)bottom_j - (double)top_j) / ((double)bottom_i - (double)top_i);
		b = (double)top_j - ( a * (double)top_i);

		for( i = start_line; i < end_line; i++)
		{
			for( j = 0; j < TOTAL_COL/2; j++)
			{
				if( in[i][j])
				{
					if( (double)j < ((double)i * a) + b)
						lp += in[i][j];
					else
					if( (double)j > ((double)i * a) + b)
						rp += in[i][j];
				}
			}
		}

		*lpres = lp;
		*rpres = rp;
	return;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
RND_Gvt_Get( t_measure *p)
{
t_return ret = E_ERROR;
uint8_t left_median, right_median;
uint32_t left_upper_sum, left_lower_sum, right_upper_sum, right_lower_sum;
uint32_t total_sum;
double gvt;

	/* Acquire data */
	RND_Acq_Multiple( &data, 5);

	/* make binary images */
#if (SIMULATION==1)
	_binarize( left_sensor_tab,  left_bin);
	_binarize( right_sensor_tab, right_bin);
#else
	_binarize( data.left,  left_bin);
	_binarize( data.right, right_bin);
#endif

	/* filter matrixes */
#if (SIMULATION==1)
	_filter_mat( left_sensor_tab, left_bin, left_filtered);
	_filter_mat( right_sensor_tab, right_bin, right_filtered);
#else
	_filter_mat( data.left, left_bin, left_filtered);
	_filter_mat( data.right, right_bin, right_filtered);
#endif

	/* get median lines for each */
	left_median  = _get_median_line( left_bin);
	right_median = _get_median_line( right_bin);

	left_upper_sum  = _sum_mat( left_filtered, 0, left_median);
	left_lower_sum  = _sum_mat( left_filtered, left_median, TOTAL_LINES);
	right_upper_sum = _sum_mat( right_filtered, 0, right_median);
	right_lower_sum = _sum_mat( right_filtered, right_median, TOTAL_LINES);

	total_sum = right_lower_sum + right_upper_sum +
				left_lower_sum + left_upper_sum;

	gvt = (double)(right_lower_sum + left_lower_sum) / (double)total_sum;
	LOG("gvt = %5.2f\n", gvt);
	p->gvt = gvt;

	if( gvt <= 2.0 / 3.0)
		p->igvt  = 0;
	else
	{
		gvt = (gvt - (2.0 / 3.0)) / 0.02;
		p->igvt = (uint8_t)gvt;
	}

	/* cal pronation */
	uint32_t left_extern_p, left_intern_p;
	uint32_t right_extern_p, right_intern_p;
	uint32_t extern_p, intern_p;

	_cal_pron( left_filtered, 0, TOTAL_LINES, &left_extern_p, &left_intern_p);
	LOG("left_sensor: extern_p = %d, intern_p = %d\n", left_extern_p, left_intern_p);
	p->left_extern_pressure = left_extern_p;
	p->left_intern_pressure = left_intern_p;

	_cal_pron( right_filtered, 0, TOTAL_LINES, &right_intern_p, &right_extern_p);
	LOG("right_sensor: extern_p = %d, intern_p = %d\n", right_extern_p, right_intern_p);
	p->right_extern_pressure = right_extern_p;
	p->right_intern_pressure = right_intern_p;


	extern_p = left_extern_p + right_extern_p;
	intern_p = left_intern_p + right_intern_p;

	if( abs( extern_p - intern_p) < ((extern_p+intern_p) * 20)/100)
		p->pronation = NEUTRE_t;
	else if ( extern_p > intern_p)
		p->pronation = CONTROL_t;
	else if ( extern_p < intern_p)
		p->pronation = SUPINAL_t;

	return ret;
}

#if (OPTIMIZE==1)
__OPTIMIZE_END
#endif

/*** End Of File ***/
