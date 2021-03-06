//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDMOVIE_H
#define UIFLOWWNDMOVIE_H

#include "UIFramework.h"

extern VControl UIFlowWndMovieCtrl;//Panel type 

extern VControl UIFlowWndMovie_Static_cameraCtrl;//Static type 

extern VControl UIFlowWndMovie_Status_CyclicRecCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_CyclicRec_ICONID_NULL,
UIFlowWndMovie_Status_CyclicRec_ICON_CYCLIC_REC_3MIN,
UIFlowWndMovie_Status_CyclicRec_ICON_CYCLIC_REC_5MIN,
UIFlowWndMovie_Status_CyclicRec_ICON_CYCLIC_REC_10MIN,
UIFlowWndMovie_Status_CyclicRec_MAX
}UIFlowWndMovie_Status_CyclicRec_Setting;

extern VControl UIFlowWndMovie_Status_MotionDetCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_MotionDet_ICONID_NULL,
UIFlowWndMovie_Status_MotionDet_ICON_MOTION_DET,
UIFlowWndMovie_Status_MotionDet_MAX
}UIFlowWndMovie_Status_MotionDet_Setting;

extern VControl UIFlowWndMovie_Status_batteryCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_battery_ICON_BATTERY_FULL,
UIFlowWndMovie_Status_battery_ICON_BATTERY_MED,
UIFlowWndMovie_Status_battery_ICON_BATTERY_LOW,
UIFlowWndMovie_Status_battery_ICON_BATTERY_EMPTY,
UIFlowWndMovie_Status_battery_ICON_BATTERY_ZERO,
UIFlowWndMovie_Status_battery_ICON_BATTERY_CHARGE,
UIFlowWndMovie_Status_battery_MAX
}UIFlowWndMovie_Status_battery_Setting;

extern VControl UIFlowWndMovie_YMD_StaticCtrl;//Static type 

extern VControl UIFlowWndMovie_HMS_StaticCtrl;//Static type 

extern VControl UIFlowWndMovie_PanelCtrl;//Panel type 

extern VControl UIFlowWndMovie_Status_StorageCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Storage_ICON_INTERNAL_FLASH,
UIFlowWndMovie_Status_Storage_ICON_SD_CARD,
UIFlowWndMovie_Status_Storage_ICON_SD_LOCK,
UIFlowWndMovie_Status_Storage_MAX
}UIFlowWndMovie_Status_Storage_Setting;

extern VControl UIFlowWndMovie_Static_timeCtrl;//Static type 

extern VControl UIFlowWndMovie_Static_maxtimeCtrl;//Static type 

extern VControl UIFlowWndMovie_Static_resolutionCtrl;//Static type 

extern VControl UIFlowWndMovie_Zoom_StaticCtrl;//Static type 

extern VControl UIFlowWndMovie_StatusICN_EVCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_StatusICN_EV_ICON_EV_P2P0,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P1P6,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P1P3,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P1P0,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P0P6,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P0P3,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P0P0,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M0P3,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M0P6,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M1P0,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M1P3,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M1P6,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M2P0,
UIFlowWndMovie_StatusICN_EV_MAX
}UIFlowWndMovie_StatusICN_EV_Setting;

extern VControl UIFlowWndMovie_Status_HDRCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_HDR_ICONID_NULL,
UIFlowWndMovie_Status_HDR_ICON_HDR,
UIFlowWndMovie_Status_HDR_MAX
}UIFlowWndMovie_Status_HDR_Setting;

extern VControl UIFlowWndMovie_StaticIcon_PIMCCtrl;//Static type 

extern VControl UIFlowWndMovie_Status_GsensorCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Gsensor_ICON_GSENSOR_OFF,
UIFlowWndMovie_Status_Gsensor_ICON_GSENSOR_L,
UIFlowWndMovie_Status_Gsensor_ICON_GSENSOR_M,
UIFlowWndMovie_Status_Gsensor_ICON_GSENSOR_H,
UIFlowWndMovie_Status_Gsensor_MAX
}UIFlowWndMovie_Status_Gsensor_Setting;

extern VControl UIFlowWndMovie_Static_LockCtrl;//Static type 

extern VControl UIFlowWndMovie_StatusICN_AudioCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_StatusICN_Audio_ICON_VOICE_OFF,
UIFlowWndMovie_StatusICN_Audio_ICON_VOICE_ON,
UIFlowWndMovie_StatusICN_Audio_MAX
}UIFlowWndMovie_StatusICN_Audio_Setting;

extern VControl UIFlowWndMovie_Status_CustomerLogoCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_CustomerLogo_ICON_YOUDAOSHI,
UIFlowWndMovie_Status_CustomerLogo_MAX
}UIFlowWndMovie_Status_CustomerLogo_Setting;

extern VControl UIFlowWndMovie_Status_ParkingMonitorCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_ParkingMonitor_ICONID_NULL,
UIFlowWndMovie_Status_ParkingMonitor_ICON_PARKING_MONITOR,
UIFlowWndMovie_Status_ParkingMonitor_MAX
}UIFlowWndMovie_Status_ParkingMonitor_Setting;

extern VControl UIFlowWndMovie_Status_TimeLapesCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_TimeLapes_ICONID_NULL,
UIFlowWndMovie_Status_TimeLapes_ICON_TIMELAPSE_REC_STATUS,
UIFlowWndMovie_Status_TimeLapes_MAX
}UIFlowWndMovie_Status_TimeLapes_Setting;

extern VControl UIFlowWndMovie_CarNo_StaticCtrl;//Static type 

extern VControl UIFlowWndMovie_Status_ZHCarNoCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_ZHCarNo_STRID_NULL,
UIFlowWndMovie_Status_ZHCarNo_STRID_YUN,
UIFlowWndMovie_Status_ZHCarNo_STRID_BEIJING,
UIFlowWndMovie_Status_ZHCarNo_STRID_NING,
UIFlowWndMovie_Status_ZHCarNo_STRID_CHUAN,
UIFlowWndMovie_Status_ZHCarNo_STRID_LU,
UIFlowWndMovie_Status_ZHCarNo_STRID_TIANJING,
UIFlowWndMovie_Status_ZHCarNo_STRID_ZHE,
UIFlowWndMovie_Status_ZHCarNo_STRID_YU,
UIFlowWndMovie_Status_ZHCarNo_STRID_XIANG,
UIFlowWndMovie_Status_ZHCarNo_STRID_YUE,
UIFlowWndMovie_Status_ZHCarNo_STRID_YU4,
UIFlowWndMovie_Status_ZHCarNo_STRID_GUIZHOU,
UIFlowWndMovie_Status_ZHCarNo_STRID_GAN4,
UIFlowWndMovie_Status_ZHCarNo_STRID_LIAO,
UIFlowWndMovie_Status_ZHCarNo_STRID_LU_2,
UIFlowWndMovie_Status_ZHCarNo_STRID_HEI,
UIFlowWndMovie_Status_ZHCarNo_STRID_XIN,
UIFlowWndMovie_Status_ZHCarNo_STRID_SU,
UIFlowWndMovie_Status_ZHCarNo_STRID_ZANG,
UIFlowWndMovie_Status_ZHCarNo_STRID_MIN,
UIFlowWndMovie_Status_ZHCarNo_STRID_JIN,
UIFlowWndMovie_Status_ZHCarNo_STRID_QIONG,
UIFlowWndMovie_Status_ZHCarNo_STRID_JI,
UIFlowWndMovie_Status_ZHCarNo_STRID_JISHOU,
UIFlowWndMovie_Status_ZHCarNo_STRID_GUI,
UIFlowWndMovie_Status_ZHCarNo_STRID_GAN,
UIFlowWndMovie_Status_ZHCarNo_STRID_WAN,
UIFlowWndMovie_Status_ZHCarNo_STRID_MENG,
UIFlowWndMovie_Status_ZHCarNo_STRID_E,
UIFlowWndMovie_Status_ZHCarNo_STRID_SHAN,
UIFlowWndMovie_Status_ZHCarNo_STRID_QIN,
UIFlowWndMovie_Status_ZHCarNo_MAX
}UIFlowWndMovie_Status_ZHCarNo_Setting;

//---------------------UIFlowWndMovieCtrl User Export---------------------------
#endif
