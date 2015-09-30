#ifndef _SENSOR_DRV_H
#define _SENSOR_DRV_H

#include "type.h"
#include "Sensor.h"

/************* Sensor 1 *************/

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_S5K2P1YX_)
SENSOR_DRVTAB* Sensor_GetDrvTab_S5K2P1YX(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_IMX083CQK_)
SENSOR_DRVTAB* Sensor_GetDrvTab_IMX083CQK(void);
#endif


#if  (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330P_)
SENSOR_DRVTAB* Sensor_GetDrvTab_AR0330(void);
#endif


#if  (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_)
SENSOR_DRVTAB* Sensor_GetDrvTab_AR0330M(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_MI5130_)
SENSOR_DRVTAB* Sensor_GetDrvTab_MI5130(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_MI5100_)
SENSOR_DRVTAB* Sensor_GetDrvTab_MI5100(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_)
SENSOR_DRVTAB* Sensor_GetDrvTab_MN34110(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_IMX208PQH_)
SENSOR_DRVTAB* Sensor_GetDrvTab_IMX208PQH(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_DUMMY_)
SENSOR_DRVTAB* Sensor_GetDrvTab_Dummy(void);
#endif

/************* Sensor 2 *************/

#if  (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_)
SENSOR_DRVTAB* Sensor_GetDrvTab_NT99141(void);
#endif

#if  (_SENSORLIB2_ == _SENSORLIB2_DUMMY_)
SENSOR_DRVTAB* Sensor_GetDrvTab_Dummy2(void);
#endif

#endif //_SENSOR_DRV_H
