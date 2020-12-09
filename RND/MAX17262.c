/*******************************************************************************
 * Texense                                                                     *
 *******************************************************************************
 * Project:
 * Component: 	MAX17262 driver
 * Element:
 * File:     	MAX172762.c
 * Description:
 *******************************************************************************
 * History:
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
 * Externals
 *******************************************************************************/
extern I2C_HandleTypeDef hi2c1;
#define hi2c			hi2c1

/*******************************************************************************
 * Locals
 *******************************************************************************/
MAX17262_configuration_t MAX17262_Current_configuration;

/*******************************************************************************
 * Function     : MAX17262_Probe
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
int
MAX17262_Probe( void)
{
HAL_StatusTypeDef ret;

	if( RND_I2C_GetAccess() == E_OK)
	{
		ret = HAL_I2C_IsDeviceReady( &hi2c, MAX17262_I2C_ADDRESS, 5, MAX17262_I2C_TIMEOUT);

		RND_I2C_GiveAccess();

		switch (ret)
		{
		case HAL_TIMEOUT:
			return -1;
			break;

		case HAL_OK:
			return 0;
			break;

		case HAL_ERROR:
		case HAL_BUSY:
		default:
			/* should reset the i2c bus and signal the error */
			return -1;
			break;
		}
	}
	else
		return -1;
}

/*******************************************************************************
 * Function     : MAX17262_RegWriteWord
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static int
MAX17262_RegWriteWord( uint8_t reg, uint16_t value)
{
HAL_StatusTypeDef status = HAL_OK;

	if( RND_I2C_GetAccess() == E_OK)
	{
		status = HAL_I2C_Mem_Write( &hi2c, MAX17262_I2C_ADDRESS, (uint16_t)reg,
    			I2C_MEMADD_SIZE_8BIT, (uint8_t*)(&value), 2, MAX17262_I2C_TIMEOUT);

		RND_I2C_GiveAccess();

		if( status != HAL_OK)
			return -1;
		else
			return 0;
	}
	else
		return -1;
}

/*******************************************************************************
 * Function     : MAX17262_RegReadWord
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
static int
MAX17262_RegReadWord( uint8_t reg, uint16_t *value)
{
HAL_StatusTypeDef status = HAL_OK;

	if( RND_I2C_GetAccess() == E_OK)
	{
		status = HAL_I2C_Mem_Read( &hi2c, MAX17262_I2C_ADDRESS, (uint16_t)reg,
				I2C_MEMADD_SIZE_8BIT, (uint8_t *)value, 2, MAX17262_I2C_TIMEOUT);

		RND_I2C_GiveAccess();

		if( status != HAL_OK)
		{
			LOG("I2C read error \n",*value);
			return -1;
		}
		else
			return 0;
	}
	else
		return -1;
}

/*******************************************************************************
 * Function     : MAX17262_Init
 * Arguments    :
 * Outputs      :
 * Return code  :
 * Description  :
 *******************************************************************************/
int
MAX17262_Init( void)
{
	/* test chip presence */
	if( MAX17262_Probe())
	{
		LOG("MAX1762 chip is not detected\n");
		return -1;
	}

	LOG("Dev Name = 0x%.4x\n", MAX17262_GetDevName());

	uint8_t retry_count = 0;
	MAX17262_RegReadWord( MAX17262_REGADDR_STATUS, &MAX17262_Current_configuration.Status.value);

	if( MAX17262_Current_configuration.Status.POR)
	{
		LOG("MAX17262 was rebooted\n");

		MAX17262_RegReadWord( MAX17262_REGADDR_FSTAT, &MAX17262_Current_configuration.FStat.value);
		for( retry_count = 0; retry_count < 3; retry_count ++)
		{
            if( !MAX17262_Current_configuration.FStat.DNR)
                break;

            osDelay(100);
            MAX17262_RegReadWord( MAX17262_REGADDR_FSTAT, &MAX17262_Current_configuration.FStat.value);
		}

		if( retry_count >= 3)
		{
			LOG("chip fails to complete startup \n");
			return -1;
		}

		uint16_t Hib_CFG;
		MAX17262_RegReadWord ( MAX17262_REGADDR_HIBCFG, 		&Hib_CFG);
		MAX17262_RegWriteWord( MAX17262_REGADDR_SOFT_WAKEUP, 	0x0090);
		MAX17262_RegWriteWord( MAX17262_REGADDR_HIBCFG, 		0x0000);
		MAX17262_RegWriteWord( MAX17262_REGADDR_SOFT_WAKEUP, 	0x0000);

		MAX17262_RegWriteWord( MAX17262_REGADDR_DESIGNCAP, 		MAX17262_BATTERY_NOMINAL_CAPACITY);
		MAX17262_RegWriteWord( MAX17262_REGADDR_ICHGTERM, 		0x0640);//USe default value
		MAX17262_RegWriteWord( MAX17262_REGADDR_VEMPTY, 		MAX17262_BATTERY_VEMPTY);

		MAX17262_RegWriteWord( MAX17262_REGADDR_MODELCFG,		0x8400);	/* or 0x8000 if charge voltage < 4.275V */

		retry_count = 0;
		do
		{
			osDelay(10);
			MAX17262_RegReadWord( MAX17262_REGADDR_MODELCFG, &MAX17262_Current_configuration.ModelCFG.value);
		}	while( MAX17262_Current_configuration.ModelCFG.Refresh && (retry_count ++ < 100));

		if (retry_count >= 100)
		{
			LOG("init failed\n");
			return -1;
		}

		/* restore */
		MAX17262_RegWriteWord( MAX17262_REGADDR_HIBCFG, 		Hib_CFG);

		LOG("Clearing POR\n");
		MAX17262_RegReadWord( MAX17262_REGADDR_STATUS, 			&MAX17262_Current_configuration.Status.value);
		MAX17262_Current_configuration.Status.POR = 0;
		MAX17262_RegWriteWord( MAX17262_REGADDR_STATUS, 		MAX17262_Current_configuration.Status.value);
	}

	return 0;
}

/*******************************************************************************
 * Function     : MAX17262_GetAvgCurrent
 * Arguments    :
 * Outputs      :
 * Return code  : Consommation de courant (1mA/bit)
 * Description  : Cette fonction vient lire la valeur moyenne de consommation de
 * 				  courant mesurée par le MAX17262
 *******************************************************************************/
uint16_t
MAX17262_GetAvgCurrent( void)
{
    uint16_t AvgCurrent;
    MAX17262_RegReadWord( MAX17262_REGADDR_AVGCURRENT, &AvgCurrent);

    AvgCurrent = ~AvgCurrent + 1;
    AvgCurrent = (uint16_t)( (float)AvgCurrent * 0.15625);

    return AvgCurrent;
}

/*******************************************************************************
 * Function     : MAX17262_GetBatterySOC
 * Arguments    :
 * Outputs      :
 * Return code  : capacité : 1mAh/bit
 * Description  : Cette fonction retourne la capacité restante de batterie
 *******************************************************************************/
uint16_t
MAX17262_GetBatterySOC( void)
{
    uint16_t Capacity;

    MAX17262_RegReadWord( MAX17262_REGADDR_REPSOC, &Capacity);
    return Capacity;
}

/*******************************************************************************
 * Function     : MAX17262_GetBatteryCapacity
 * Arguments    :
 * Outputs      :
 * Return code  : capacité : 1mAh/bit
 * Description  : Cette fonction retourne la capacité restante de batterie
 *******************************************************************************/
uint16_t
MAX17262_GetBatteryCapacity( void)
{
    uint16_t Capacity;

    MAX17262_RegReadWord( MAX17262_REGADDR_QH, &Capacity);

    Capacity/=2;

    return Capacity;
}

/*******************************************************************************
 * Function     : MAX17262_GetBatteryVoltage
 * Arguments    :
 * Outputs      :
 * Return code  : capacité : 1mV/bit
 * Description  : Cette fonction retourne la tension batterie
 *******************************************************************************/
uint16_t
MAX17262_GetBatteryVoltage( void)
{
    uint16_t Voltage;

    MAX17262_RegReadWord( MAX17262_REGADDR_VCELL, &Voltage);
    Voltage = (uint16_t)(((float)Voltage * 1.25) / 16);
    return Voltage;
}

/*******************************************************************************
 * Function     : MAX17262_GetDevName
 * Arguments    :
 * Outputs      :
 * Return code  : devName non signé 16 bits
 * Description  : Cette fonction retourne l'identifiant du composant. Utile pour
 * 				  tester la com au démarrage.
 *******************************************************************************/
uint16_t
MAX17262_GetDevName( void)
{
    uint16_t devName;
    MAX17262_RegReadWord( MAX17262_REGADDR_DEVNAME, &devName);
    return devName;
}

/*** End of File ***/
