#include "GSensor.h"
#include "Gsensor_GMA301.h"
#include "SwTimer.h"
#include "Debug.h"
#include <stdlib.h>

//********************************************************************************
//
//GSENSOR DRA07
//
//********************************************************************************
static INT32 GsXData = 0,OldGsXData = 0;
static INT32 GsYData = 0,OldGsYData = 0;
static INT32 GsZData = 0,OldGsZData = 0;
static BOOL  g_bGsensorOpened = FALSE;
static UINT32 GMA301Threshold=0xFF;/*1G= X/16;*/
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;
//**********************************************************************************
//
//
//**********************************************************************************


INT32 GSensor_GMA301_GetAxisMsg(UINT32 uiInt)
{
    INT32 Data  = 0;

    if (uiInt >511) //0x3F
    {
        //negative number
        Data =-(uiInt-511);
        debug_msg("GSensor_GMA301_GetAxisMsg1  = %d\r\n",Data);

    }
    else
    {
        Data = uiInt;
        debug_msg("GSensor_GMA301_GetAxisMsg2  = %d\r\n",Data);
    }

    return Data;//(bzNegative)?(-1*Data):(Data);

}

static UINT32 GSensor_GMA301_GetSensitivityLevel(void)
{
    switch(g_GSensorSensitivity)
   	{
		case GSENSOR_SENSITIVITY_OFF:
			GMA301Threshold =0xFF;
			break;
		case GSENSOR_SENSITIVITY_LOW:
			GMA301Threshold =0x95;//25
			break;
		case GSENSOR_SENSITIVITY_MED:
			GMA301Threshold =0x85;//20
			break;
		case GSENSOR_SENSITIVITY_HIGH:
			GMA301Threshold =0x65;//16
			break;
		default:
			GMA301Threshold =0xFF;
			break;
   	}
	return GMA301Threshold;
}

static void GSensor_GMA301_OpenInterface(void)
{

}

static void GSensor_GMA301_CloseInterface(void)
{

}


static BOOL GSensor_GMA301_Init(void)
{
	UINT32 value = 0;
    GSENSOR_INFO GsensorInfo;

    GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
    GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
    GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    // DMARD07 GSensor I2C slave addresses
    GsensorInfo.I2C_Slave_WAddr = 0x30;
    GsensorInfo.I2C_Slave_RAddr = 0x31;
	
	//init
    if (GSensor_I2C_Init(GsensorInfo) == TRUE)
    {	
		GSensor_I2C_WriteReg(0x21,0x52);//
		GSensor_I2C_WriteReg(0x00,0x02);//
		GSensor_I2C_WriteReg(0x00,0x12);//
		GSensor_I2C_WriteReg(0x00,0x02);//
		GSensor_I2C_WriteReg(0x00,0x82);//
		GSensor_I2C_WriteReg(0x00,0x02);//
		//GSensor_I2C_WriteReg(0x1F,0x28);//
		//GSensor_I2C_WriteReg(0x0C,0x8F);//
		//GSensor_I2C_WriteReg(0x00,0x06);//
		GSensor_I2C_WriteReg(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
		//interrupt setup
		GSensor_I2C_WriteReg(0x11,0x40);//IIC 0X07 for no pullup //0x06 High active  0x40 low active
		//GSensor_I2C_WriteReg(0x11,0x06);//IIC 0X06 for no pullup 
		//set Gsensor Level 
		//0x08-->0.5G 
		//0X10-->1G
		GSensor_I2C_WriteReg(0x38,0XFF);//
		GSensor_I2C_WriteReg(0x39,0X40);//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g  (8 : 0.5g)
		
		//GSensor_I2C_WriteReg(0x0F,0x00);//
		//GSensor_I2C_WriteReg(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
		GSensor_I2C_WriteReg(0x1F,0x28);//To disable micro motion interrupt.

		//set active.
		GSensor_I2C_WriteReg(0x0C,0x8F);//
		GSensor_I2C_WriteReg(0x00,0x06);//
		Delay_DelayMs(10); //2014-12-18 added 20ms delay for G-sensor active.
		
		//TimerDelayMs(10);
		//Latched reference data of micro motion.
		
		GSensor_I2C_ReadReg(0x12);
		GSensor_I2C_ReadReg(0x13);
		GSensor_I2C_ReadReg(0x14);
		GSensor_I2C_ReadReg(0x15);
		GSensor_I2C_ReadReg(0x16);
		GSensor_I2C_ReadReg(0x17);
		GSensor_I2C_ReadReg(0x18);
		GSensor_I2C_ReadReg(0x19);
		
		//GSensor_I2C_ReadReg(0x1A);
		//GSensor_I2C_ReadReg(0x1B);

		//GSensor_I2C_WriteReg(0x1F,0x28);//To enable micro motion interrupt.
		//delay(100); //2014_0819 added 1ms delay for micro motion setup itself.

		//GSensor_I2C_WriteReg(0x0B,0x40);

		//GSensor_I2C_WriteReg(0x1F,0x38);//2014-12-19 To enable micro motion interrupt.
		GSensor_I2C_WriteReg(0x1F,0x18);//2014-12-19 To enable micro motion interrupt.
		Delay_DelayMs(1); //2014-12-18 added 1ms delay for micro motion setup itself.
		
		//Sensitivity parameter adjust
		GSensor_I2C_WriteReg(0x0D,0x44);//2014-12-18 Set output data rate to 25HZ. ( 0x70, 0x60, 0x50, 0x40 only)
		GSensor_I2C_WriteReg(0x0F,0x05);//2014-12-19 4 tap.( 0x00~0x05 only )
		
		GSensor_I2C_ReadReg(0x1C);//2014-12-18 Clear interrupt flag.
		GSensor_I2C_ReadReg(0x1D);

		GSensor_I2C_WriteReg(0x0E,0x1C);//To enable interrupt.//parking monitor
		//GSensor_GMA301_ReadReg_2Byte(0x1C,value);
    	}
    return TRUE;
}

//for  Gensor POWER UP
void GSensor_GMA301_PowerUp(INT32 sen)
{}

void GSensor_GMA301_PowerDown(void)
{
	UINT32 value = 0;
	debug_msg("GSensor_GMA301_PowerDown...\r\n");	
	GSensor_I2C_WriteReg(0x21,0x01);//
}

BOOL GSensor_open(void)
{
   if (g_bGsensorOpened == TRUE)
   {
       debug_msg("Gsensor open already\r\n");
       return TRUE;
   }

   g_bGsensorOpened = TRUE;

   // open Gsensor interface
   GSensor_GMA301_OpenInterface();

   // Gsensor init
   GSensor_GMA301_Init();

   return TRUE;
}

BOOL GSensor_GMA301_close(void)
{

    if (g_bGsensorOpened==FALSE)
    {
        debug_msg("I2C close already\r\n");
        return TRUE;
    }

    // close Gsensor interface
    GSensor_GMA301_CloseInterface();

    g_bGsensorOpened = FALSE;

    return TRUE;
}


BOOL GSensor_GetData(Gsensor_Data *GS_Data)
{
       INT16 data_hi,data_low;
	 INT16 x,y,z;
	 UINT32 value;
	 
	GSensor_I2C_ReadReg(0x12);	 
	GSensor_I2C_ReadReg(0x13);
	
	data_low=GSensor_I2C_ReadReg(0x14);
	data_hi=GSensor_I2C_ReadReg(0x15);
	
	{
		x = data_hi << 8 |data_low;
	}
	
	data_low=GSensor_I2C_ReadReg(0x16);
	data_hi=GSensor_I2C_ReadReg(0x17);
	{
		y=data_hi<<8|data_low;	
	}	
	
	data_low=GSensor_I2C_ReadReg(0x18);
	data_hi=GSensor_I2C_ReadReg(0x19);
	{
		z=data_hi<<8|data_low;	
	}		
	//debug_msg("x-y-z: %d,%d,%d\r\n",x,y,z);
	GS_Data->Axis.Xacc=x;
	GS_Data->Axis.Yacc=y;
	GS_Data->Axis.Zacc=z;	

	return TRUE;
}

BOOL GSensor_CrashMode(void)
{
    return FALSE;
}

void GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
    g_GSensorSensitivity = GSensorSensitivity;
}


