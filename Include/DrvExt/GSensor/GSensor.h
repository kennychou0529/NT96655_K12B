
#ifndef __Gsensor_H
#define __Gsensor_H

#include "i2c.h"

typedef enum
{
    GSENSOR_I2C_REGISTER_1BYTE = 1,
    GSENSOR_I2C_REGISTER_2BYTE = 2,
    ENUM_DUMMY4WORD(GSENSOR_I2C_REGISTER_BYTES)
} GSENSOR_I2C_REGISTER_BYTES, *PGSENSOR_I2C_REGISTER_BYTES;


typedef struct
{
    I2C_SECTION I2C_Channel;
    GSENSOR_I2C_REGISTER_BYTES  I2C_RegBytes;
    I2C_PINMUX      I2C_PinMux;
    I2C_BUS_CLOCK   I2C_BusClock;
    UINT32          I2C_Slave_WAddr;
    UINT32          I2C_Slave_RAddr;
} GSENSOR_INFO,*PGSENSOR_INFO;

// for axis data
typedef enum
{
    GSENSOR_AXIS_X,
    GSENSOR_AXIS_Y,
    GSENSOR_AXIS_Z,
    ENUM_DUMMY4WORD(GSENSOR_AXIS)
} GSENSOR_AXIS;

// for get status
typedef enum
{
    GSENSOR_ORIENT_0,
    GSENSOR_ORIENT_90,
    GSENSOR_ORIENT_180,
    GSENSOR_ORIENT_270,
    ENUM_DUMMY4WORD(GSENSOR_ORIENT)
} GSENSOR_ORIENT;

typedef enum
{
    GSENSOR_SHAKE_LEFT,
    GSENSOR_SHAKE_RIGHT,
    ENUM_DUMMY4WORD(GSENSOR_SHAKE)
} GSENSOR_SHAKE;

typedef enum
{
    GSENSOR_SENSITIVITY_OFF,
    GSENSOR_SENSITIVITY_LOW,
    GSENSOR_SENSITIVITY_MED,
    GSENSOR_SENSITIVITY_HIGH,
    ENUM_DUMMY4WORD(GSENSOR_SENSITIVITY)
}GSENSOR_SENSITIVITY;

typedef struct
{
   UINT32      Xacc;
   UINT32      Yacc;
   UINT32      Zacc;
} AXIS_DATA;

typedef struct {
    AXIS_DATA   Axis;
    GSENSOR_ORIENT      Ori;
    GSENSOR_SHAKE       SH;
    UINT32       Dev;
} Gsensor_Data,*pGsensor_Data;

#if 1
#define NSA_REG_SPI_I2C                 0x00
#define NSA_REG_WHO_AM_I                0x01
#define NSA_REG_ACC_X_LSB               0x02
#define NSA_REG_ACC_X_MSB               0x03
#define NSA_REG_ACC_Y_LSB               0x04
#define NSA_REG_ACC_Y_MSB               0x05
#define NSA_REG_ACC_Z_LSB               0x06
#define NSA_REG_ACC_Z_MSB               0x07
#define NSA_REG_MOTION_FLAG           0x09
#define NSA_REG_G_RANGE                 0x0f
#define NSA_REG_ODR_AXIS_DISABLE        0x10
#define NSA_REG_POWERMODE_BW            0x11
#define NSA_REG_SWAP_POLARITY           0x12
#define NSA_REG_FIFO_CTRL               0x14
#define NSA_REG_INTERRUPT_SETTINGS1     0x16
#define NSA_REG_INTERRUPT_SETTINGS2     0x17
#define NSA_REG_INTERRUPT_MAPPING1      0x19
#define NSA_REG_INTERRUPT_MAPPING2      0x1a
#define NSA_REG_INTERRUPT_MAPPING3      0x1b
#define NSA_REG_INT_PIN_CONFIG          0x20
#define NSA_REG_INT_LATCH               0x21
#define NSA_REG_ACTIVE_DURATION         0x27
#define NSA_REG_ACTIVE_THRESHOLD        0x28
#define NSA_REG_TAP_DURATION            0x2A
#define NSA_REG_TAP_THRESHOLD           0x2B
#define NSA_REG_CUSTOM_OFFSET_X         0x38
#define NSA_REG_CUSTOM_OFFSET_Y         0x39
#define NSA_REG_CUSTOM_OFFSET_Z         0x3a
#define NSA_REG_ENGINEERING_MODE        0x7f
#define NSA_REG_SENSITIVITY_TRIM_X      0x80
#define NSA_REG_SENSITIVITY_TRIM_Y      0x81
#define NSA_REG_SENSITIVITY_TRIM_Z      0x82
#define NSA_REG_COARSE_OFFSET_TRIM_X    0x83
#define NSA_REG_COARSE_OFFSET_TRIM_Y    0x84
#define NSA_REG_COARSE_OFFSET_TRIM_Z    0x85
#define NSA_REG_FINE_OFFSET_TRIM_X      0x86
#define NSA_REG_FINE_OFFSET_TRIM_Y      0x87
#define NSA_REG_FINE_OFFSET_TRIM_Z      0x88
#define NSA_REG_SENS_COMP               0x8c
#define NSA_REG_SENS_COARSE_TRIM        0xd1
 int mir3da_i2c_init(void);
 int mir3da_chip_init(void);
 int mir3da_read_data(short *x, short *y, short *z);
 int mir3da_open_interrupt(int num);
 int mir3da_close_interrupt(int num);
 int mir3da_read_interrupt_statu(void);


#endif


extern BOOL   GSensor_open(void);
extern BOOL   GSensor_close(void);
extern BOOL   GSensor_Init(void);
extern BOOL   GSensor_GetData(Gsensor_Data *GS_Data);
extern void   GSensor_GetStatus(UINT32 status,UINT32 *uiData);
extern BOOL   GSensor_ParkingMode(void);
extern BOOL   GSensor_CrashMode(void);
extern void   GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity);


extern BOOL   GSensor_I2C_Init(GSENSOR_INFO GSensorInfo);
extern void   GSensor_I2C_WriteReg(UINT32 uiAddr, UINT32 uiValue);
extern UINT32 GSensor_I2C_ReadReg(UINT32 uiAddr);

extern void GSensor_GMA301_PowerDown(void);

#endif  //__Gsensor_H
