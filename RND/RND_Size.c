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
#include "math.h"

/*******************************************************************************
 * Locals
 *******************************************************************************/
static t_return	_calc_size					(t_acq data, t_measure *p);
static t_acq data;

/*******************************************************************************
 * Globals
 *******************************************************************************/
uint8_t left_bin [TOTAL_LINES][TOTAL_COL/2];
uint8_t right_bin[TOTAL_LINES][TOTAL_COL/2];

/*******************************************************************************
 * Externals
 *******************************************************************************/
#if (SIMULATION==1)
extern uint16_t right_sensor_tab[];
extern uint16_t left_sensor_tab[];
#endif

#if (OPTIMIZE==1)
__OPTIMIZE_START
#endif

/*******************************************************************************
 * Function     : 
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
uint16_t
_calc_means( t_acq_tab in)
{
double mean = 0.0;

	for( register int i = 0; i < TOTAL_SENSORS_PER_SIDE; i++)
		mean += (double)( in[ i]);
	mean /= (double)TOTAL_SENSORS_PER_SIDE;

	return mean;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
_binarize( t_acq_tab inTab, t_bin_mat outMat)
{
t_acq_mat *p = (t_acq_mat *)inTab;

	/* calculate means of both matrixes */
	uint16_t means = _calc_means( inTab);

	/* make binary matrixes */
	for( uint16_t i = 0; i < TOTAL_LINES; i++)
	{
		for( uint16_t j = 0; j < TOTAL_COL / 2; j++)
		{
			if( (*p)[i][j] > means)
				outMat[i][j] = 1;
			else
				outMat[i][j] = 0;
		}
	}

	return E_OK;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
uint8_t
_get_median_line( t_bin_mat in)
{
register int i,j;
uint8_t sum[TOTAL_LINES] = {0};
uint8_t extx[TOTAL_LINES] = {0}, exty[TOTAL_LINES] = {0}, extd[TOTAL_LINES] = {0};
uint8_t extidx = 0;
int8_t dir = 1, pdir = 1;
uint8_t psum = 0;
uint8_t max = 0, maxidx = 0;
uint8_t min = 255, minidx = 0;

	/* make sum tab */
	for( i = 0; i < TOTAL_LINES; i++)
		for( j = 0; j < TOTAL_COL / 2; j++)
			sum[i] += in[i][j];

	/* locate extremums */
	for( i = 0; i < TOTAL_LINES; i++)
	{
		/* current direction */
		if( sum[i] > psum)
			dir = 1;
		else if( sum[i] < psum)
			dir = -1;
		else dir = 0;

		if( dir != pdir)
		{
			extx[extidx] = i;
			exty[extidx] = psum;
			extd[extidx] = (dir<pdir) ? 1 : 0;
			extidx ++;
		}

		psum = sum[i];
		pdir = dir;
	}

	/* find max from top of image */
	for( i = 0; i < extidx; i++)
	{
		if( (exty[i] > max) && (extd[i] == 1) )
		{
			max = exty[i];
			maxidx = i;
		}
	}

	/* find lowest from previous max */
	for( i = maxidx; i < extidx - 1; i++)
	{
		if( (exty[i] <= min) && (extd[i] == 0) )
		{
			min = exty[i];
			minidx = i;
		}
	}

	return extx[minidx];
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
void
_get_coord_extr_left( t_bin_mat in, uint8_t *xa, uint8_t *ya, uint8_t *xb, uint8_t *yb)
{
register int i,j;
uint8_t tab[TOTAL_LINES] = {0};
uint8_t median = _get_median_line( in);

	for( i = 0; i < TOTAL_LINES; i++)
		for( j = 0; j < TOTAL_COL / 2; j++)
		{
			tab[i] = j;
			if( in[i][j])
				break;
		}

	/* top half -> xa, ya */
	*xa = TOTAL_COL / 2;
	*ya = median;

	for( i = 0; i < median; i++)
	{
		if( tab[i] < *xa)
		{
			*xa = tab[i];
			*ya = i;
		}
	}

	/* botom half -> xb, yb */
	*xb = TOTAL_COL / 2;
	*yb = TOTAL_LINES;

	for( i = median; i < TOTAL_LINES; i++)
	{
		if( tab[i] < *xb)
		{
			*xb = tab[i];
			*yb = i;
		}
	}

	return;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
void
_get_coord_extr_right( t_bin_mat in, uint8_t *xc, uint8_t *yc, uint8_t *xd, uint8_t *yd)
{
register int i,j;
uint8_t tab[TOTAL_LINES] = {0};
uint8_t median = _get_median_line( in);

	for( i = 0; i < TOTAL_LINES; i++)
		for( j = (TOTAL_COL / 2) - 1; j >= 0; j--)
		{
			tab[i] = ((TOTAL_COL / 2) - 1) - j;
			if( in[i][j])
				break;
		}

	/* top half -> xa, ya */
	*xc = (TOTAL_COL / 2) - 1;
	*yc = median;

	for( i = 0; i < median; i++)
	{
		if( tab[i] < *xc)
		{
			*xc = tab[i];
			*yc = i;
		}
	}

	*xc = ((TOTAL_COL / 2) - 1) - *xc;

	/* botom half -> xb, yb */
	*xd = (TOTAL_COL / 2) - 1;
	*yd = TOTAL_LINES;

	for( i = median; i < TOTAL_LINES; i++)
	{
		if( tab[i] < *xd)
		{
			*xd = tab[i];
			*yd = i;
		}
	}

	*xd = ((TOTAL_COL / 2) - 1) - *xd;

	return;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
void
_get_hilo_pos( t_bin_mat in, uint8_t *hi, uint8_t *low)
{
register int i,j;
bool found = FALSE;

	/* find hi position */
	for( i = 0; i < TOTAL_LINES; i ++)
	{
		if (found == FALSE)
		{
			for (j = 0; j < TOTAL_COL / 2; j++)
			{
				if (in[i][j])
				{
					found = TRUE;
					*hi = i;
					break;
				}
			}
		}
		else
			break;
	}

	/* find low position */
	found = FALSE;
	for( i = TOTAL_LINES - 1; i >= 0; i --)
	{
		if( found == FALSE)
		{
			for( j = 0; j < TOTAL_COL/2; j++)
			{
				if( in[i][j])
				{
					found = TRUE;
					*low = i;
					break;
				}
			}
		}
		else
			break;
	}

	return;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
void
_get_hilo_tab_pos( t_acq_tab tab, uint8_t *hi, uint8_t *low)
{
register int i,j;
bool found = FALSE;
t_acq_mat *in = (t_acq_mat *)tab;

	/* find hi position */
	for( i = 0; i < TOTAL_LINES; i ++)
	{
		if (found == FALSE)
		{
			for (j = 0; j < TOTAL_COL / 2; j++)
			{
				if ((*in)[i][j] > 100)
				{
					found = TRUE;
					*hi = i;
					break;
				}
			}
		}
		else
			break;
	}

	/* find low position */
	found = FALSE;
	for( i = TOTAL_LINES - 1; i >= 0; i --)
	{
		if( found == FALSE)
		{
			for( j = 0; j < TOTAL_COL/2; j++)
			{
				if( (*in)[i][j] > 100)
				{
					found = TRUE;
					*low = i;
					break;
				}
			}
		}
		else
			break;
	}

	return;
}

/*******************************************************************************
 * Function     :
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static t_return
_calc_size( t_acq data, t_measure *p)
{
uint8_t xa, ya, xb, yb;
uint8_t xc, yc, xd, yd;
double  a1 = 0, a2 = 0;
double left_angle, right_angle;
double xy_ratio = 16/6.6;
uint8_t hi = 0, lo = 0;
double left_size,right_size;
double offset_heel = 1.5/0.66; //1.5cm convert to foot print for heel
double offset_toe = (1/0.66)+1.5; //On rajoute 1cm pour la définition de la pointure et une pointure et demi supérieurepour la taille de la chaussure réelle

	/* Left sensor data */

#if (SIMULATION==1)
	_binarize( left_sensor_tab,  left_bin);
#else
	_binarize( data.left,  left_bin);
#endif

	_get_coord_extr_left(  left_bin, &xa, &ya, &xb, &yb);
	_get_coord_extr_right( left_bin, &xc, &yc, &xd, &yd);

	xa *= xy_ratio;
	xb *= xy_ratio;
	xc *= xy_ratio;
	xd *= xy_ratio;

	if( (yb - ya) != 0)
		a1 = atan(((double) xb - (double) xa) / ((double) yb - (double) ya));
	else
		a1 = 0;

	/* angle in degres :
	 * a1 = a1 * (180/M_PI);
	 */

	if( (yd - yc) != 0)
		a2 = atan(((double) xd - (double) xc) / ((double) yd - (double) yc));
	else
		a2 = 0;

	/* angle in degres :
	 * a2 = a2 * (180/M_PI);
	 */

	left_angle = (a1 + a2) / 2.0;

	/* right sensor data */

#if (SIMULATION==1)
	_binarize( right_sensor_tab, right_bin);
#else
	_binarize( data.right, right_bin);
#endif

	_get_coord_extr_left(  right_bin, &xa, &ya, &xb, &yb);
	_get_coord_extr_right( right_bin, &xc, &yc, &xd, &yd);

	xa *= xy_ratio;
	xb *= xy_ratio;
	xc *= xy_ratio;
	xd *= xy_ratio;

	if( (yb - ya) != 0)
		a1 = atan(((double) xb - (double) xa) / ((double) yb - (double) ya));
	else
		a1 = 0;

	/* angle in degres :
	 * a1 = a1 * (180/M_PI);
	 */

	if( (yd - yc) != 0)
		a2 = atan(((double) xd - (double) xc) / ((double) yd - (double) yc));
	else
		a2 = 0;

	/* angle in degres :
	 * a2 = a2 * (180/M_PI);
	 */

	/* take median value of angle */
	right_angle = (a1 + a2) / 2.0;

	/* left size */
	//_get_hilo_pos( left_bin, &hi, &lo);

#if (SIMULATION==1)
	_get_hilo_tab_pos( left_sensor_tab, &hi, &lo);
#else
	_get_hilo_tab_pos( data.left, &hi, &lo);
#endif
	left_size = ( (lo - hi) / cos( left_angle)) + offset_heel + offset_toe;
	LOG("left: hi=%d, lo=%d, gap=%d, angle=%4.2f, size=%4.2f\n",
			hi, lo, lo-hi, left_angle *(180/M_PI), left_size);

	p->left_hi = hi;
	p->left_lo = lo;
	p->left_angle = left_angle * (180/M_PI);
	p->left_size = left_size;

	/* right size */
	//_get_hilo_pos( right_bin, &hi, &lo);

#if (SIMULATION==1)
	_get_hilo_tab_pos( right_sensor_tab, &hi, &lo);
#else
	_get_hilo_tab_pos( data.right, &hi, &lo);
#endif

	right_size = (lo - hi) / cos( right_angle) + offset_heel + offset_toe;
	LOG("right: hi=%d, lo=%d, gap=%d, angle=%4.2f, size=%4.2f\n",
			hi, lo, lo-hi, right_angle *(180/M_PI), right_size);

	p->right_hi = hi;
	p->right_lo = lo;
	p->right_angle = right_angle * (180/M_PI);
	p->right_size = right_size;

	/* take max value */
	p->pointure = _MAX( left_size, right_size);
	double pointure_arrondie = round( (p->pointure)*2)/2;
	p->pointure = pointure_arrondie;
	//LOG("pointure_arrondie = %4.2f\n", pointure_arrondie);
	return E_OK;
}

/*******************************************************************************
 * Function     : RND_Size_Get
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
RND_Size_Get( t_measure *p)
{
	//RND_Acq_Multiple( &data, 5);

	RND_Acq_Multiple_Start( 5);
	RND_Print("\nLevez les talons"); osDelay(SECOND);
	RND_Acq_Multiple_End( &data, 5, 10);


	_calc_size( data, p);

	return E_OK;
}

/*******************************************************************************
 * Function     : RND_Size_Get
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
t_return
RND_send_measure_and_size_get( t_measure *p )
{
	//RND_Acq_Multiple( &data, 5);

	RND_Acq_Multiple_Start( 5);
	RND_Print("\nLevez les talons"); osDelay(SECOND);
	RND_Acq_Multiple_End( &data, 5, 10);

	_calc_size( data, p);

	RND_send_UART( data );

	return E_OK;
}

#if (OPTIMIZE==1)
__OPTIMIZE_END
#endif

/*** End Of File ***/
