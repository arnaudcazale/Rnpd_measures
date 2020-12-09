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
#ifndef __MAX17262_H__
#define __MAX17262_H__

/*******************************************************************************
 * pre-include files
 *******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Configuration
 *******************************************************************************/

/*******************************************************************************
 * Definitions
 *******************************************************************************/
typedef struct
{
    union
    {
        uint16_t value;
        struct
        {
            unsigned            :1;
            unsigned    POR     :1;
            unsigned    Imn     :1;
            unsigned    Bst     :1;
            unsigned            :2;
            unsigned    Imx     :1;
            unsigned    dSOCi   :1;
            unsigned    Vmn     :1;
            unsigned    Tmn     :1;
            unsigned    Smn     :1;
            unsigned    Bi      :1;
            unsigned    Vmx     :1;
            unsigned    Tmx     :1;
            unsigned    Smx     :1;
            unsigned    Br      :1;
        };
    }Status;

    union
    {
        uint16_t value;
        struct
        {
            unsigned    DNR     :1;
            unsigned            :5;
            unsigned    RelDt2  :1;
            unsigned    FQ      :1;
            unsigned    EDet    :1;
            unsigned    RelDet  :1;
            unsigned            :6;
        };
    }FStat;

    union
    {
        uint16_t value;
        struct
        {
            unsigned  HibConfig_LSB  :8;
            unsigned  HibConfig_MSB  :7;
            unsigned    EnHib        :1;
        };
    }HibCFG;

    union
    {
        uint16_t value;
        struct
        {
            unsigned            :4;
            unsigned ModelID    :4;
            unsigned            :2;
            unsigned VChg       :1;
            unsigned            :2;
            unsigned R100       :1;
            unsigned            :1;
            unsigned Refresh    :1;
        };
    }ModelCFG;
}MAX17262_configuration_t;

#define MAX17262_I2C_ADDRESS            	0x6C
//#define MAX17262_I2C_ADDRESS            	0x1A
#define MAX17262_I2C_TIMEOUT				1000

#define MAX17262_REGADDR_STATUS         	0x00
#define MAX17262_REGADDR_VALRTTH        	0x01
#define MAX17262_REGADDR_TALRTTH        	0x02
#define MAX17262_REGADDR_SALRTTH        	0x03
#define MAX17262_REGADDR_ATRATE         	0x04
#define MAX17262_REGADDR_REPCAP         	0x05
#define MAX17262_REGADDR_REPSOC         	0x06
#define MAX17262_REGADDR_AGE            	0x07
#define MAX17262_REGADDR_TEMP           	0x08
#define MAX17262_REGADDR_VCELL          	0x09
#define MAX17262_REGADDR_CURRENT        	0x0A
#define MAX17262_REGADDR_AVGCURRENT     	0x0B
#define MAX17262_REGADDR_QRESIDUAL      	0x0C
#define MAX17262_REGADDR_MIXSOC         	0x0D
#define MAX17262_REGADDR_AVSOC          	0x0E
#define MAX17262_REGADDR_MIXCAP         	0x0F
#define MAX17262_REGADDR_FULLCAPREP     	0x10
#define MAX17262_REGADDR_TTE            	0x11
#define MAX17262_REGADDR_QRTABLE00      	0x12
#define MAX17262_REGADDR_FULLSOCTHR     	0x13
#define MAX17262_REGADDR_RCELL          	0x14
#define MAX17262_REGADDR_AVGTA          	0x16
#define MAX17262_REGADDR_CYCLES         	0x17
#define MAX17262_REGADDR_DESIGNCAP      	0x18
#define MAX17262_REGADDR_AVGVCELL       	0x19
#define MAX17262_REGADDR_MAXMINTEMP     	0x1A
#define MAX17262_REGADDR_MAXMINVOLT     	0x1B
#define MAX17262_REGADDR_MAXMINCURR     	0x1C
#define MAX17262_REGADDR_CONFIG         	0x1D
#define MAX17262_REGADDR_ICHGTERM       	0x1E
#define MAX17262_REGADDR_AVCAP          	0x1F
#define MAX17262_REGADDR_TTF            	0x20
#define MAX17262_REGADDR_DEVNAME        	0x21
#define MAX17262_REGADDR_QRTABLE10      	0x22
#define MAX17262_REGADDR_FULLCAPNOM     	0x23
#define MAX17262_REGADDR_AIN            	0x27
#define MAX17262_REGADDR_LEARNCFG       	0x28
#define MAX17262_REGADDR_FILTERCFG      	0x29
#define MAX17262_REGADDR_RELAXCFG       	0x2A
#define MAX17262_REGADDR_MISCCFG        	0x2B
#define MAX17262_REGADDR_TGAIN          	0x2C
#define MAX17262_REGADDR_TOFF           	0x2D
#define MAX17262_REGADDR_CGAIN          	0x2E
#define MAX17262_REGADDR_COFF           	0x2F
#define MAX17262_REGADDR_QRTABLE20      	0x32
#define MAX17262_REGADDR_DIETEMP        	0x34
#define MAX17262_REGADDR_FULLCAP        	0x35
#define MAX17262_REGADDR_RCOMP0         	0x38
#define MAX17262_REGADDR_TEMPCO         	0x39
#define MAX17262_REGADDR_VEMPTY         	0x3A
#define MAX17262_REGADDR_FSTAT          	0x3D
#define MAX17262_REGADDR_TIMER          	0x3E
#define MAX17262_REGADDR_SHDNTIMER      	0x3F
#define MAX17262_REGADDR_QRTABLE30      	0x42
#define MAX17262_REGADDR_RGAIN          	0x43
#define MAX17262_REGADDR_DQACC          	0x45
#define MAX17262_REGADDR_DPACC          	0x46
#define MAX17262_REGADDR_CONVGCFG       	0x49
#define MAX17262_REGADDR_VREFREMCAP     	0x4A
#define MAX17262_REGADDR_QH             	0x4D
#define MAX17262_REGADDR_STATUS2        	0xB0
#define MAX17262_REGADDR_POWER          	0xB1
#define MAX17262_REGADDR_ID             	0xB2
#define MAX17262_REGADDR_AVGPOWER       	0xB3
#define MAX17262_REGADDR_IALRTTH        	0xB4
#define MAX17262_REGADDR_TTFCFG         	0xB5
#define MAX17262_REGADDR_CVMIXCAP       	0xB6
#define MAX17262_REGADDR_CVHALFTIME     	0xB7
#define MAX17262_REGADDR_CGTEMPCO       	0xB8
#define MAX17262_REGADDR_CURVE          	0xB9
#define MAX17262_REGADDR_HIBCFG         	0xBA
#define MAX17262_REGADDR_CONFIG2        	0xBB
#define MAX17262_REGADDR_VRIPPLE        	0xBC
#define MAX17262_REGADDR_RIPPLECFG      	0xBD
#define MAX17262_REGADDR_TIMERH         	0xBE
#define MAX17262_REGADDR_RSENSE         	0xD0
#define MAX17262_REGADDR_SCOCVLIM       	0xD1
#define MAX17262_REGADDR_VGAIN          	0xD2
#define MAX17262_REGADDR_SOCHOLD        	0xD3
#define MAX17262_REGADDR_MAXPEAKPOWER   	0xD4
#define MAX17262_REGADDR_SUSPEAKPOWER   	0xD5
#define MAX17262_REGADDR_PACKRESISTANCE 	0xD6
#define MAX17262_REGADDR_SYSRESISTANCE  	0xD7
#define MAX17262_REGADDR_MINSYSVOLTAGE  	0xD8
#define MAX17262_REGADDR_MPPCURRENT     	0xD9
#define MAX17262_REGADDR_SPPCURRENT     	0xDA
#define MAX17262_REGADDR_MODELCFG       	0xDB
#define MAX17262_REGADDR_ATQRESIDUAL    	0xDC
#define MAX17262_REGADDR_ATTTE          	0xDD
#define MAX17262_REGADDR_ATAVSOC        	0xDE
#define MAX17262_REGADDR_ATAVCAP        	0xDF
#define MAX17262_REGADDR_SOFT_WAKEUP    	0x60

#define MAX17262_FAILURE                	0xFF
#define MAX17262_SUCCESS                	0x00

//Battery informations
#define MAX17262_BATTERY_NOMINAL_CAPACITY   4500 	//2250mAh/0.5 =4500
#define MAX17262_BATTERY_ICHGTERM           4 		//4*1.5625/7=0.89mA
#define MAX17262_BATTERY_VEMPTY             37120 	//37120*0.078125e-3=2900mV
#define MAX17262_BATTERY_NOMINAL_VOLTAGE_MV 3700


int			MAX17262_Init					(void);
uint16_t	MAX17262_GetAvgCurrent			(void);
uint16_t	MAX17262_GetBatteryCapacity		(void);
uint16_t	MAX17262_GetBatteryVoltage		(void);
uint16_t	MAX17262_GetBatterySOC			(void);
uint16_t	MAX17262_GetDevName				(void);

#endif

int			MAX17262_Init					(void);
uint16_t	MAX17262_GetAvgCurrent			(void);
uint16_t	MAX17262_GetBatterySOC			(void);
uint16_t	MAX17262_GetBatteryCapacity		(void);
uint16_t	MAX17262_GetBatteryVoltage		(void);
uint16_t	MAX17262_GetDevName				(void);
/*** End Of File ***/
