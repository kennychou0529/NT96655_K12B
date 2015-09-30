/*
    System Sensor Callback

    System Callback for Sensor Module.

    @file       SysSensor_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#if (AUTO_BACKLIGHT_FUNCTION == ENABLE)
#include "UIAppPhoto.h" //for CURR_EV()
#include "MenuSetupLCDBrt.h" //for MenuSetup_GetV2BacklightValue()
#endif
#include "GxFlash.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysSensorExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (AUTO_BACKLIGHT_FUNCTION == ENABLE)
int SX_TIMER_DET_EV_ID = -1;
void Sensor_DetEV(void);
SX_TIMER_ITEM(Sensor_DetEV, Sensor_DetEV, 50, TRUE)
#endif
int SX_TIMER_DET_FLASH_ID = -1;
int SX_TIMER_DET_RECHARGE_ID = -1;
SX_TIMER_ITEM(GxFlash_DetFlash, GxFlash_DetFlash, 5, TRUE)
SX_TIMER_ITEM(GxFlash_DetReCharge, GxFlash_DetReCharge, 50, TRUE)
//void DscCommon_Open(void);
void IPLCtrl_Open(void);
void IPLCtrl_Close(void);

//void Sensor_RegCmdTab(void);

void System_OnSensorInit(void)
{
    //PHASE-1 : Init & Open Drv or DrvExt
    {
        //for command table
        //Sensor_RegCmdTab();
    }
    //PHASE-2 : Init & Open Lib or LibExt
    {
        //DscCommon_Open();
        //IPLCtrl_Open();
        #if (AUTO_BACKLIGHT_FUNCTION == ENABLE)
        SX_TIMER_DET_EV_ID = SxTimer_AddItem(&Timer_Sensor_DetEV);
        #endif
        SX_TIMER_DET_FLASH_ID = SxTimer_AddItem(&Timer_GxFlash_DetFlash);
        SX_TIMER_DET_RECHARGE_ID = SxTimer_AddItem(&Timer_GxFlash_DetReCharge);
    }
}

void System_OnSensorExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
        //IPLCtrl_Close();
    }
    //PHASE-1 : Close Drv or DrvExt
    {
    }
}

#if (AUTO_BACKLIGHT_FUNCTION == ENABLE)
extern BOOL g_bIsInitSystemFinish;

void Sensor_DetEV(void)
{
    //check is enable to AUTO adjust?
    if(UI_GetData(FL_BrightAdjIndex) !=  LCDBRT_ADJ_AUTO)
        return;
    //Sleep not control backlight
    if(GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) > 0)
        return;
    //Playback mode can not get LV value
    if(UI_GetData(FL_ModeIndex) == DSC_MODE_PLAYBACK)
        return;

    {
    UINT32 uiLV;
    uiLV = CURR_EV()/10;
    //#NT#2011/03/31#Lincy Lin -begin
    //#NT#
    if (g_bIsInitSystemFinish )
    {
        //#NT#2011/04/09#Lincy Lin -begin
        //#NT#
        //DetBLcount++;
        //if (DetBLcount > 2)
        //#NT#2011/04/09#Lincy Lin -end
        {
            //debug_msg("CURR_EV()=%d\r\n", CURR_EV());
            //debug_msg("LV=%d\r\n", uiLV);
            //debug_msg(" set bright=%d\r\n",MenuSetup_GetV2BacklightValue(uiLV));
            Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_BRIGHTLVL,1,MenuSetup_GetV2BacklightValue(uiLV));
        }
    }
    //#NT#2011/03/31#Lincy Lin -end
    }
}
#endif

