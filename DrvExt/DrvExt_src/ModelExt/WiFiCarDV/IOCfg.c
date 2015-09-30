/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       IOCfg.c
    @ingroup    mIPRJAPCommonIO

    @brief      IO config module
                This file is the IO config module

    @note       Nothing.

    @date       2005/12/24
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#include "Type.h"
#include "DrvExt.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "debug.h"
#include "IOCfg.h"
#include "Utility.h"
#include "Pll.h"
#include "IOInit.h"
//#include "Timer.h"
#include "GSensor.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (USE_VIO == ENABLE)
UINT32 Virtual_IO[VIO_MAX_ID] = {0};
UINT32 vio_getpin(UINT32 id){if(id>=VIO_MAX_ID)return 0; return Virtual_IO[id];}
void vio_setpin(UINT32 id, UINT32 v){if(id>=VIO_MAX_ID)return; Virtual_IO[id] = v;}
#endif

#define GPIO_SET_NONE           0xffffff
#define GPIO_SET_OUTPUT_LOW     0x0
#define GPIO_SET_OUTPUT_HI      0x1

GPIO_INIT_OBJ uiGPIOMapInitTab[] = {
    //CARD
    {  GPIO_CARD_POWER,         GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   GPIO_SET_NONE         },
    {  GPIO_CARD_DETECT,        GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_CARD_DETECT       },
    {  GPIO_CARD_WP,            GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_CARD_WP           },
    //LCD
    {  GPIO_LCD_BLG_PCTL,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_LCD_BLG_PCTL      },
    //{  GPIO_LCD_RESET,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     GPIO_SET_NONE       },
    //LED
    {  GPIO_GREEN_LED,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_GREEN_LED      },
    {  GPIO_RED_LED,              GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_RED_LED         },
    {  GPIO_FOCUS_LED,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_FOCUS_LED         },
    {  GPIO_DC_DET,            GPIO_DIR_INPUT,     PAD_PULLDOWN,           PAD_DC_DET             },
    //KEY
    {  GPIO_KEY_MODE,           GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MODE          },
    //Sensor
    {  GPIO_SENSOR_PWM0,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST     },
    {  GPIO_SENSOR_PWM1,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST     },
    {  GPIO_SENSOR_RESET,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     GPIO_SET_NONE       },
    {  GPIO_GPS_DET,            GPIO_DIR_INPUT,     PAD_PULLDOWN,           PAD_GPS_DET             },
    {  GPIO_WIFI_LED,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_WIFI_LED         },
    {  GPIO_LDWS_BEEP,         GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,   GPIO_SET_NONE  },
//    {  GPIO_SENSOR_RESET,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
//    {  GPIO_SENSOR_STANDBY,     GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
    //Wi-Fi power
    {  GPIO_WIFI_POWER_PWM5,    GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
#if 0
     {  GPIO_KEY_ZOOMOUT,       GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ZOOMOUT       },
     {  GPIO_KEY_ZOOMIN,        GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ZOOMIN       },
     {  GPIO_KEY_LEFT,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_LEFT       },
     {  GPIO_KEY_ENTER,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ENTER       },
     {  GPIO_KEY_UP,            GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_UP       },
     {  GPIO_KEY_RIGHT,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_RIGHT       },
     {  GPIO_KEY_PLAYBACK,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_PLAYBACK       },
     {  GPIO_KEY_DOWN,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_DOWN       },
     {  GPIO_KEY_MENU,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MENU       },

     //#NT#2012/07/25#Isiah Chang -begin
     //#NT#Added GPIO map for Lens&motor driver.
     {  GPIO_LENS_ZOOM_PI,      GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_ZOOM_PI       },
     {  GPIO_LENS_ZOOM_PR,      GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_ZOOM_PR       },
     {  GPIO_LENS_FOCUS_PI,     GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_FOCUS_PI      },

     {  GPIO_LENS_IN1A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN1B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN2A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN2B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN3A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN3B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN4A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN4B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     //#NT#2012/07/25#Isiah Chang -end
#endif
};

UINT32 totalGpioInitCount = sizeof(uiGPIOMapInitTab)/sizeof((uiGPIOMapInitTab)[0]);

BOOL GPIOMap_DCDet(void)
{
    return (gpio_getPin(GPIO_DC_DET)? FALSE : TRUE);
    //return (gpio_getPin(GPIO_DC_DET)? TRUE : FALSE);
}

void IO_InitGensor(void);

#include "rtc.h"

//should be call after rtc_open()
void IO_GetPowerSrc(void)
{
    UINT32 pwrlost, pwsrc;
    pwrlost = rtc_isPowerLost();
    if(pwrlost)
    {
        DBG_DUMP("^GPowerOn Pwr Lost!\r\n"); //"firs time power-on" or "lost power of Gold capacitor"

        //should notify user to configure current date-time!
    }

    pwsrc = rtc_getPWROnSource();
    if(pwsrc == RTC_PWRON_SRC_PWR_SW)
    {
        DBG_DUMP("^GPowerOn Src = PWR key\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW2)
    {
        DBG_DUMP("^GPowerOn Src = PB Key\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW3)
    {
        DBG_DUMP("^GPowerOn Src = USB plug\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW4)
    {
        DBG_DUMP("^GPowerOn Src = DC plug\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_ALM)
    {
        DBG_DUMP("^GPowerOn Src = PWR alarm\r\n");
    }
}

/**
  Do GPIO initialization

  Initialize input/output pins, and pin status

  @param void
  @return void
*/
void IO_InitGPIO(void)
{
    UINT32 iSValue;

    DBG_IND("GPIO START\r\n");
    //--------------------------------------------------------------------
    // Open GPIO driver
    //--------------------------------------------------------------------
    #if 1 //_MIPS_TODO
    gpio_open();
    for(iSValue=0 ; iSValue<totalGpioInitCount ; iSValue++)
    {
        if (uiGPIOMapInitTab[iSValue].GpioDir == GPIO_DIR_INPUT)
        {
            gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_INPUT);
            pad_setPullUpDown(uiGPIOMapInitTab[iSValue].PadPin, uiGPIOMapInitTab[iSValue].PadDir);
        }
        else
        {
            gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_OUTPUT);
            if (uiGPIOMapInitTab[iSValue].PadDir == GPIO_SET_OUTPUT_HI)
            {
                gpio_setPin(uiGPIOMapInitTab[iSValue].GpioPin);
            }
            else
            {
                gpio_clearPin(uiGPIOMapInitTab[iSValue].GpioPin);
            }
        }
    }
    #endif

    //--------------------------------------------------------------------
    // Use Non-Used PWM to be Delay Timer
    //--------------------------------------------------------------------
    #if defined(PWMID_TIMER)
    Delay_setPwmChannels(PWMID_TIMER);
    #endif

    DBG_IND("GPIO END\r\n");

}
/**
  Initialize Gsensor IIC bus

  Initialize Gsensor IIC bus

  @param void
  @return void
*/

int  i2c_read_block_data( unsigned char base_addr, unsigned char count, unsigned char *data){
	int i = 0;
		
	for(i = 0; i < count;i++)
	{
		*data =  (unsigned char)GSensor_I2C_ReadReg(base_addr+i);
               data ++;
	}	
		
	return count;
}


int     i2c_read_byte_data( unsigned char addr, unsigned char *data){
	*data =  (unsigned char)GSensor_I2C_ReadReg(addr);
	return 0;
}

/*return value: 0: is ok    other: is failed*/
int     i2c_write_byte_data( unsigned char addr, unsigned char data){
	GSensor_I2C_WriteReg(addr,data);
	return 0;
}
int mir3da_register_read( unsigned char addr, unsigned char *data){
    int     res = 0;

    res = i2c_read_byte_data(addr, data);
    if(res != 0) {
          return res;
    }	

    return res;
}

int mir3da_register_write( unsigned char addr, unsigned char data){
    int     res = 0;

    res = i2c_write_byte_data(addr, data);
    if(res != 0) {
         return res;
    }

    return res;
}

int mir3da_register_read_continuously( unsigned char addr, unsigned char count, unsigned char *data)
{
    int     res = 0;

    res = (count==i2c_read_block_data(addr, count, data)) ? 0 : 1;
    if(res != 0) {
         return res;
    }

    return res;
}

int mir3da_register_mask_write(unsigned char addr, unsigned char mask, unsigned char data){
    int     res = 0;
    unsigned char      tmp_data;

    res = mir3da_register_read(addr, &tmp_data);
    if(res) {
        return res;
    }

    tmp_data &= ~mask; 
    tmp_data |= data & mask;
    res = mir3da_register_write(addr, tmp_data);

    return res;
}

int mir3da_read_data(short *x, short *y, short *z)
{
    unsigned char    tmp_data[6] = {0};

    if (mir3da_register_read_continuously(NSA_REG_ACC_X_LSB, 6, tmp_data) != 0) {
        return -1;
    }
    
    *x = ((short)(tmp_data[1] << 8 | tmp_data[0]))>> 4;
    *y = ((short)(tmp_data[3] << 8 | tmp_data[2]))>> 4;
    *z = ((short)(tmp_data[5] << 8 | tmp_data[4]))>> 4;

     //debug_msg("------mir3da x= %4d y= %4d z= %4d-----\r\n",*x,*y,*z); 	

    return 0;
}

int mir3da_i2c_init(void){
	 GSENSOR_INFO GsensorInfo;  
	 GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE; 
	 GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;  
	 GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;    
	 GsensorInfo.I2C_Slave_WAddr = 0x4e;   
	 GsensorInfo.I2C_Slave_RAddr = 0x4f;   

	 if (GSensor_I2C_Init(GsensorInfo) == TRUE)    {     
	 	debug_msg("------mir3da i2c init OK-----\r\n"); 
		return 0;
	 }

	 return -1;
}

int mir3da_chip_init(void){
	int             res = 0;
	unsigned char data=0;

/*
	if(mir3da_i2c_init()){
		return -1;
		debug_msg("------mir3da i2c init error-----\r\n"); 
	}
*/
	mir3da_register_read(NSA_REG_WHO_AM_I,&data);
	if(data != 0x13){
	       debug_msg("------mir3da read chip id  error= %x-----\r\n",data); 
		return -1;
	}

	debug_msg("------mir3da chip id = %x-----\r\n",data); 

	res =  mir3da_register_mask_write(NSA_REG_SPI_I2C, 0x24, 0x24);

	Delay_DelayMs(5);

	res |= mir3da_register_mask_write(NSA_REG_G_RANGE, 0x03, 0x02);
	res |= mir3da_register_mask_write(NSA_REG_POWERMODE_BW, 0xFF, 0x1E);
	res |= mir3da_register_mask_write(NSA_REG_ODR_AXIS_DISABLE, 0xFF, 0x07);
	
	res |= mir3da_register_mask_write(NSA_REG_INT_LATCH, 0x8F, 0x8F);
	
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0x83);
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0x69);
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0xBD);

	return res;	    	
}
int mir3da_open_interrupt(int num){
	int   res = 0;

	res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x03);
	res = mir3da_register_write(NSA_REG_ACTIVE_DURATION,0x03 );
	res = mir3da_register_write(NSA_REG_ACTIVE_THRESHOLD,0x26 );////DEBUG//38//DF
			
	switch(num){

		case 0:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x04 );
			break;

		case 1:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING3,0x04 );
			break;
	}

	return res;
}

int mir3da_close_interrupt(int num){
	int   res = 0;

	res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x00 );
			
	switch(num){

		case 0:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x00 );
			break;

		case 1:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING3,0x00 );
			break;
	}

	return res;
}

/*return value: 1:has intterupt     0: no intterupt*/
int mir3da_read_interrupt_statu(){

	unsigned char data=0;

	mir3da_register_read(NSA_REG_MOTION_FLAG,&data);

	if(data&0x04)
		return 1;

	return 0;
}



void IO_InitGensor(void)
{
#if 0//GMA301
    GSensor_open();
#else
        mir3da_i2c_init(); 
		
	/*if (mir3da_read_interrupt_statu())
	{
		//SetGsensorPowerOn(TRUE);
	}
	else
	{
		//SetGsensorPowerOn(FALSE);
	}*/
	
        //mir3da_read_interrupt_statu();//key pwron/gsensor pwron
        mir3da_chip_init();

#endif
}
/**
  Initialize voltage detection

  Initialize voltage detection for battery and flash

  @param void
  @return void
*/

void IO_InitADC(void)
{

    if (adc_open(ADC_CH_VOLDET_BATTERY) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
        return;
    }

    if (adc_open(ADC_CH_VOLDET_KEY1) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for key key1 detection\r\n");
        return;
    }
    if (adc_open(ADC_CH_VOLDET_KEY2) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for key key1 detection\r\n");
        return;
    }

    //650 Range is 250K Hz ~ 2M Hz
    adc_setConfig(ADC_CONFIG_ID_OCLK_FREQ, 250000); //250K Hz

    //battery voltage detection
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_INTEN, FALSE);

    //key key1 detection
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_INTEN, FALSE);

    //key key2 detection
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_INTEN, FALSE);

    // Enable adc control logic
    adc_setEnable(TRUE);

    Delay_DelayMs(15); //wait ADC stable  //for pwr on speed up
}

#if _MIPS_TODO
static char ADCStr1[32];
static char ADCStr2[32];

char* VolDet_GetStatusString1(void)
{
    sprintf(ADCStr1, "A0=%ld,A1=%ld,A2=%ld,A3=%ld", adc_readData(0), adc_readData(1),adc_readData(2),adc_readData(3));
    return ADCStr1;
}

char* VolDet_GetStatusString2(void)
{
    sprintf(ADCStr2, "A4=%ld,A5=%ld,A6=%ld,A7=%ld", adc_readData(4), adc_readData(5),adc_readData(6),adc_readData(7));
    return ADCStr2;
}
#endif


void Dx_InitIO(void)  // Config IO for external device
{
    IO_InitPinmux(); //initial PINMUX config
    IO_InitGPIO(); //initial GPIO pin status
    IO_InitADC(); //initial ADC pin status
   IO_InitGensor();
#if (_HDMITYPE_ == _HDMI_ON_)
    //thse two pins are default pull-up for GPIO,but must turn off for HDMI DDC I2C
    pad_setPullUpDown(PAD_PIN_PGPIO28, PAD_NONE);
    pad_setPullUpDown(PAD_PIN_PGPIO29, PAD_NONE);
#endif
}

void Dx_UninitIO(void)  // Config IO for external device
{

    // Disable adc control logic
    adc_setEnable(FALSE);
    adc_close(ADC_CH_VOLDET_BATTERY);
    adc_close(ADC_CH_VOLDET_KEY1);
    adc_close(ADC_CH_VOLDET_KEY2);
}

BOOL IS_TurnONLCDBacklight(void)
{
    return gpio_getPin(GPIO_LCD_BLG_PCTL);
}

BOOL DetGPS_IO(void)
{
    return gpio_getPin(GPIO_GPS_DET);
}

void TurnOnWifiLed(void)
{
    gpio_setPin(GPIO_WIFI_LED);
}

void TurnOffWifiLed(void)
{
    gpio_clearPin(GPIO_WIFI_LED);
}

void GPIO_Set_LDWS_BEEP(BOOL  Enb)
{
    if(Enb)
		gpio_setPin(GPIO_LDWS_BEEP);
	else
		gpio_clearPin(GPIO_LDWS_BEEP);
    
}

BOOL GPIO_Get_LDWS_BEEP(void)
{
    return (gpio_getPin(GPIO_LDWS_BEEP)? TRUE : FALSE);
    
}

