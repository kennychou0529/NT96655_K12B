#ifndef _DXOUTPUT_H_
#define _DXOUTPUT_H_

#include "type.h"

///////////////////////////////////////////////////////////////////////////////
// LED
///////////////////////////////////////////////////////////////////////////////
// Definitions for GPIOMap_EnableLED() and GPIOMap_IsLEDOn

//#NT#2010/02/02#Lincy Lin -begin
//#NT#set FCS LED brightness level
typedef enum
{
    LCSBRT_LVL_00 = 0,
    LCSBRT_LVL_01,
    LCSBRT_LVL_02,
    LCSBRT_LVL_03,
    LCSBRT_LVL_04,
    LCSBRT_LVL_05,
    LCSBRT_LVL_06,
    LCSBRT_LVL_SETTING_MAX
}LCSBRT_LVL_SETTING;
//#NT#2010/02/02#Lincy Lin -end


#define GPIOMAP_LED_GREEN           0
#define GPIOMAP_LED_RED             1
#define GPIOMAP_LED_FCS             2
extern void     LED_TurnOnLED(UINT32 uiLED);
extern void     LED_TurnOffLED(UINT32 uiLED);
extern BOOL     LED_IsLEDOn(UINT32 uiLED);
//#NT#2010/02/02#Lincy Lin -begin
//#NT#set FCS LED brightness level
extern void     LED_SetFcsLevel(UINT32 uiLevel);
//#NT#2010/02/02#Lincy Lin -end
BOOL LED_OnCommand(CHAR *pcCmdStr);

#endif //_DXOUTPUT_H_

