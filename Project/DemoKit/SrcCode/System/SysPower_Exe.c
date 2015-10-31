/*
    System Power Callback

    System Callback for Power Module.

    @file       SysPower_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"
#include "GxInput.h"
#include "DxPower.h"
#include "DxLens.h"
#include "GxFlash.h"
#include "GxPower.h"
#include "DbgUtApi.h"

#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UsbDevDef.h"
#endif

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysPwrExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
#include "UIFlowMovieFuncs.h"
#include "Gsensor.h"

extern INT32 Get_GSensorSensitivity(void);
extern BOOL UIFlowWndMovie_IsGsensorTrig(void);
extern void UIFlowWndMovie_SetGsensorTrigFlag(BOOL En);
extern BOOL DevCtrl_IsACCPoerOn(void);


#define KEYSCAN_AUTOLCDOFF_DISABLED   0
#define KEYSCAN_DELAYOFF_DISABLED   0
#define KEYSCAN_GPS_DISABLED  0

#if 1
BOOL g_bLapseRecFlag = FALSE;
#define LAPSE_REC_TRIG_VALUE   (17)  //eric@2015021
#define GSENSOR_STATIC_COUNTER (3000) //(9000)//eric@2015021
#define GSENSOR_MOTION_COUNTER (1)//(100)//eric@2015021
#endif

static volatile UINT32  g_uiKeyScanAutoLcdoffCnt;
static volatile BOOL    g_bKeyScanAutoLcdoffEn = FALSE;
static volatile UINT32  g_uiKeyScanDelayoffCnt;
static volatile BOOL    g_bKeyScanDelayoffEn = TRUE;

static volatile UINT32  g_uiKeyScanGpsoffCnt;
static volatile BOOL    g_bKeyScanGpsoffEn = FALSE;

static UINT32   uiSystemAutoLcdoffTime     = KEYSCAN_AUTOLCDOFF_DISABLED;
static UINT32   uiSystemDelayoffTime     = KEYSCAN_DELAYOFF_DISABLED;
static UINT32   uiSystemGpsoffTime     = KEYSCAN_GPS_DISABLED;
extern BOOL g_bWifistaus;//ERIC EDIT 0909-1
static BOOL bDetGsensorStatus = FALSE;
static BOOL bGsensorFirst = FALSE;
static BOOL bDetMOVIStatus = FALSE;
#define FIVE_MINUTES    300
#define HALF_AN_HOUR  1800

int SX_TIMER_DET_SLEEP_ID = -1;
int SX_TIMER_DET_AUTOPOWEROFF_ID = -1;
int SX_TIMER_DET_BATT_ID = -1;
int SX_TIMER_DET_CHARGE_ID = -1;
int SX_TIMER_DET_SHOWADC = -1;
int SX_TIMER_DET_BATTINS_ID = -1;
int SX_TIMER_DET_DCPLUG_ID = -1;
int SX_TIMER_DET_GSENSOR_ID = -1;

int SX_TIMER_DET_LCDOFF_ID = -1;
int SX_TIMER_DET_DELAYOFF_ID = -1;
int SX_TIMER_DET_GPS_ID = -1;
int SX_TIMER_DET_DELAYSHUTDOWN = -1;

INT32 nLapseRecLedOffCounter = 0;//vincent@20150919-6
UINT32 nLapseRecStatusCounter = 0;
static int m_is_screen_status = 0;
void GxCustom_DetAutoLcdoff(void);
void GxCustom_DetDelayoff(void);



void UI_DetAutoSleep(void);
void UI_DetAutoPoweroff(void);
void UI_DetBattery(void);
void UI_DetBatteryInsert(void);
void System_DetShowADC(void);
void GxCustom_DetDCPlug(void);
void UI_DetGsensor(void);
void UI_DetDelayPowerOff(void);  //eric edit
int app_check_screen_status(void);
void app_set_screen_status(int val);

#if (AUTOSLEEP_FUNCTION == ENABLE)
SX_TIMER_ITEM(GxPower_DetSleep, UI_DetAutoSleep, 50, TRUE)
#endif
#if (AUTOPWROFF_FUNCTION == ENABLE)
SX_TIMER_ITEM(GxPower_DetAutoPoweroff, UI_DetAutoPoweroff, 50, TRUE)
#endif
#if (PWRLEVEL_FUNCTION == ENABLE)
SX_TIMER_ITEM(GxPower_DetBattery, UI_DetBattery, 25, TRUE)
#endif
#if (USB_CHARGE_FUNCTION == ENABLE)
SX_TIMER_ITEM(GxPower_DetCharge, GxPower_DetCharge, 50, TRUE)
#endif
#if (SHOW_ADC_ON_SCREEN == ENABLE)
SX_TIMER_ITEM(System_DetShowADC, System_DetShowADC, 15, FALSE)
#endif
#if (USB_CHARGE_FUNCTION == ENABLE)
SX_TIMER_ITEM(GxPower_DetBatteryIns, UI_DetBatteryInsert, 25, TRUE)
#endif

SX_TIMER_ITEM(GxCustom_DetDCPlug, GxCustom_DetDCPlug, 50, TRUE)
#if (POWERON_TESTFLOW == ENABLE)
BOOL System_Is_USBPC_DeadBattery(void);
#endif
SX_TIMER_ITEM(GxCustom_DetAutoLcdoff, GxCustom_DetAutoLcdoff, 50, TRUE)
SX_TIMER_ITEM(UI_DetDelayPowerOff, UI_DetDelayPowerOff, 50, FALSE)
SX_TIMER_ITEM(GxCustom_DetDelayoff, GxCustom_DetDelayoff, 50, FALSE)
SX_TIMER_ITEM(UI_DetGsensor, UI_DetGsensor, 1, TRUE)

UINT32 DxPower_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
    #if LENS_FUNCTION
    switch (event)
    {
        case DRVPWR_CB_IS_LENS_MOVING:
            return GxFlash_GetData(FLASH_SUSPEND_CALLER_CNT);
        case DRVPWR_CB_DUMMY_LOAD_START:
            {
                PMOTOR_TAB pMotorObj = motor_getObject();
                PLENS_DEVICE_OBJ pLensDevice =DrvLens_GetDevice();


                //first time,need to init motor dirver gpio
                pMotorObj->init(pLensDevice);
                pMotorObj->setSignal(MD_SIGNAL_STANDBY,ON);
                //Open shutter
                pMotorObj->shutter_setState(MOTOR_SHUTTER_DUMMYLOAD,SHUTTER_DUMMYLOAD_START);
            }
            return 0;
        case DRVPWR_CB_DUMMY_LOAD_END:
            {
                PMOTOR_TAB pMotorObj = motor_getObject();
                pMotorObj->shutter_setState(MOTOR_SHUTTER_DUMMYLOAD,SHUTTER_DUMMYLOAD_END);
                pMotorObj->setSignal(MD_SIGNAL_STANDBY,OFF);
            }
            return 0;
        default:
            DBG_ERR("event(%d)\r\n",event);
            return 0;

    }
    #else
    return 0;
    #endif
}

void System_OnPowerPreInit(void)
{
    // register DxPower Callback function
    DrvPower_RegCB(DxPower_CB);

#if (POWERON_TESTFLOW == ENABLE)
    gIsUSBChargePreCheck = System_Is_USBPC_DeadBattery();
#endif

    if(GxKey_GetData(GXKEY_NORMAL_KEY) & FLGKEY_UP)
        GxPower_SetControl(GXPWR_CTRL_IGNORE_BATT_CHECK, TRUE);
    else
        GxPower_SetControl(GXPWR_CTRL_IGNORE_BATT_CHECK, FALSE);
#if (POWERON_TESTFLOW == ENABLE)
    if(gIsUSBChargePreCheck)
        GxPower_SetControl(GXPWR_CTRL_IGNORE_BATT_CHECK, TRUE);
#else
    //GxPower_OnSystem(SYSTEM_CMD_POWERON); //Do Dummy Load
    GxPower_PowerON();
#endif

    #if _MIPS_TODO
    HwClock_EnablePowerAlarm(FALSE);
    #endif

    //If press up key, enter "Battery Calibration" mode.
    #if 0
    if(GxKey_GetData(GXKEY_NORMAL_KEY) & FLGKEY_UP)
    #else
    if(GxPower_GetControl(GXPWR_CTRL_IGNORE_BATT_CHECK))
    #endif
    {
        //"battery calibration" path
        DBG_IND("USER force to ignore low-battery check.\r\n");

        GxPower_SetControl(GXPWR_CTRL_BATTERY_DETECT_EN, FALSE);
    }
    else
    {
        //normal path
        DBG_IND("Start low-battery check (dummyload).\r\n");

        //if under low-battery condition
        if(!GxPower_DummyLoad())
        {
            System_SetState(SYS_STATE_POWERON, SYS_POWERON_LOWPOWER);
            System_SetState(SYS_STATE_POWEROFF, SYS_POWEROFF_LOWPOWER);
        }
        GxPower_SetControl(GXPWR_CTRL_BATTERY_DETECT_EN, TRUE);
    }
}

extern void Power_CB(UINT32 event, UINT32 param1, UINT32 param2);

void System_OnPowerInit(void)
{
    //PHASE-1 : Init & Open Drv or DrvExt
    {
    GxPower_RegCB(Power_CB);     //Register CB function of GxPower
    }
    //PHASE-2 : Init & Open Lib or LibExt
    {
    //1.砞﹚init
    //2.砞﹚CB,

    //3.爹SxJob狝叭 ---------> System Job
    //4.爹SxTimer狝叭 ---------> Detect Job
    #if (AUTOSLEEP_FUNCTION == ENABLE)
    SX_TIMER_DET_SLEEP_ID = SxTimer_AddItem(&Timer_GxPower_DetSleep);
    #endif
    #if (AUTOPWROFF_FUNCTION == ENABLE)
    SX_TIMER_DET_AUTOPOWEROFF_ID = SxTimer_AddItem(&Timer_GxPower_DetAutoPoweroff);
    #endif
    #if (PWRLEVEL_FUNCTION == ENABLE)
    SX_TIMER_DET_BATT_ID = SxTimer_AddItem(&Timer_GxPower_DetBattery);
    #endif
    #if (USB_CHARGE_FUNCTION == ENABLE)
    SX_TIMER_DET_CHARGE_ID = SxTimer_AddItem(&Timer_GxPower_DetCharge);
    #endif
    #if (SHOW_ADC_ON_SCREEN == ENABLE)
    SX_TIMER_DET_SHOWADC = SxTimer_AddItem(&Timer_System_DetShowADC);
    #endif
    #if (USB_CHARGE_FUNCTION == ENABLE)
    SX_TIMER_DET_BATTINS_ID = SxTimer_AddItem(&Timer_GxPower_DetBatteryIns);
    #endif
	SX_TIMER_DET_DCPLUG_ID = SxTimer_AddItem(&Timer_GxCustom_DetDCPlug);
    SX_TIMER_DET_GSENSOR_ID = SxTimer_AddItem(&Timer_UI_DetGsensor);
	SX_TIMER_DET_LCDOFF_ID = SxTimer_AddItem(&Timer_GxCustom_DetAutoLcdoff);
    SX_TIMER_DET_DELAYOFF_ID = SxTimer_AddItem(&Timer_GxCustom_DetDelayoff);
    //5.爹SxCmd狝叭 ---------> Cmd Function
    //GxPower_InstallCmd();
    //DbgUt_InstallCmd();
    }
}

void System_OnPowerExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
    }
    //PHASE-1 : Close Drv or DrvExt
    {
    }
}

///////////////////////////////////////////////////////////////////////////////

#if (AUTOSLEEP_FUNCTION == ENABLE)
void UI_DetAutoSleep(void)
{
    if((!UI_IsForceLock()) && (!UI_IsForceLockAutoSleep()))
    {
        //debug_msg("DetAutoSleep\r\n");
        GxPower_DetSleep();
    }
}
#endif
#if (AUTOPWROFF_FUNCTION == ENABLE)
void UI_DetAutoPoweroff(void)
{
       //debug_msg("*********************************ERIC_DEBUG*********************\n\r");//ERIC_DEBUG
      // debug_msg("%s,%s,%d\r\n",__FILE__,__func__,__LINE__);
	//debug_msg("%d\r\n",g_bWifistaus);
      // debug_msg("*********************************ERIC_DEBUG*********************\n\r");//ERIC_DEBUG	
   // if((!UI_IsForceLock()) && (!UI_IsForceLockAutoPWROff())&& (!g_bWifistaus))//ERIC EDIT 0909-1不需要因为进入WIFI的时候相应的定时器不激活
   if((!UI_IsForceLock()) && (!UI_IsForceLockAutoPWROff()))//ERIC EDIT 0909-1
    {
        GxPower_DetAutoPoweroff();
    }
}
#endif

extern BOOL m_USBPlug;

#if (PWRLEVEL_FUNCTION == ENABLE)
void UI_DetBattery(void)
{
    if(!UI_IsForceLock())
    {
        //#NT#2012/10/23#Philex Lin - begin
        // send NVTEVT_BATTERY event to flash power charging icon in power charging mode
        #if (UI_STYLE==UI_STYLE_DRIVER)
         // power charging case
         if(!m_USBPlug)
            GxPower_DetBattery();
         if (GxUSB_GetConnectType() == USB_CONNECT_CHARGER)
         {
            Ux_PostEvent(NVTEVT_BATTERY, 0);
         } else {
            // normal battery detect case
            GxPower_DetBattery();
            Ux_PostEvent(NVTEVT_BATTERY, 0);
         }
        #else
        GxPower_DetBattery();
        #endif
        //#NT#2012/10/23#Philex Lin - end
    }
}
#endif
#if (USB_CHARGE_FUNCTION == ENABLE)
BOOL gIsBattInsert = TRUE;
BOOL gIsBattDead = FALSE;
void UI_DetBatteryInsert(void)
{
    if(!UI_IsForceLock())
    {
        BOOL bInsert;
        #if (USB_CHARGE_FUNCTION == ENABLE)
        bInsert = GxPower_GetControl(GXPWR_CTRL_IS_BATT_INSERT);
        #else
        bInsert = TRUE;
        #endif
        if(bInsert != gIsBattInsert)
        {
            gIsBattInsert = bInsert;
            debug_msg("  Battery Insert = %s\r\n", gIsBattInsert?"YES":"NO");
        }
    }
}
#endif
#if (SHOW_ADC_ON_SCREEN == ENABLE)
void System_DetShowADC(void)
{
    IRECT  StringRect = { 200, 162, 340, 40 };
    IRECT*  pRect = &StringRect;
    static char ADCStr[20];

    //UIDebug_Sys("aa  = %d\r\n",UI_GetData(UI_ShowADC));
    if (UI_GetData(UI_ShowADC) == TRUE)
    {
        DC**     pDCList;
        UI_SetDisplayDirty(TRUE); //set TRUE to force dirty current begin/end

        pDCList = (DC**)UI_BeginScreen();
        sprintf(ADCStr, "AD0-%ld,AD1-%ld,AD2-%ld", adc_readData(0), adc_readData(1),adc_readData(2));
        GxGfx_SetTextStroke((const FONT*)gDemo_Font, FONTSTYLE_NORMAL, SCALE_1X);
        GxGfx_SetTextColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
        GxGfx_FillRect(((DC**)pDCList)[GxGfx_OSD],pRect->uiLeft,pRect->uiTop,pRect->uiLeft+pRect->uiWidth,pRect->uiTop+pRect->uiHeight);
        GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
        GxGfx_SetTextColor(_OSD_INDEX_RED, _OSD_INDEX_GRAY, 0);
        GxGfx_Text(((DC**)pDCList)[GxGfx_OSD], pRect->uiLeft, pRect->uiTop, ADCStr);
        UI_EndScreen((UINT32)pDCList);
    }

}
#endif


///////////////////////////////////////////////////////////////////////////////

INT32 System_OnBattChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_PostEvent(NVTEVT_BATTERY, 0);
    debug_msg("battery changed %d**********\r\n",GxPower_GetControl(GXPWR_CTRL_BATTERY_LEVEL));

    return NVTEVT_CONSUME;
}

INT32 System_OnBattEmpty(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (UI_GetData(FL_IsCIPARunning))
    {
        if (UI_GetData(FL_IsEmptyBatt) == FALSE)
        {
            UI_SetData(FL_IsEmptyBatt, TRUE);
        }
    }
    #if (EMPTYPWROFF_FUNCTION == ENABLE)
    else
    {
        #if (UI_STYLE==UI_STYLE_DRIVER)
        // set battery low event to ui window
        Ux_PostEvent(NVTEVT_BATTERY_LOW, 0);
        #else
        System_PowerOff(SYS_POWEROFF_BATTEMPTY);
        #endif
    }
    #endif
    return NVTEVT_CONSUME;
}

INT32 System_OnPwrChargeOK(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
    //GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
    //GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,FALSE);
   // GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,TRUE);
    return NVTEVT_CONSUME;
}

INT32 System_OnPwrChargeSuspend(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
  //  GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,FALSE);
   // GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
    //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
    //GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
    return NVTEVT_CONSUME;
}

INT32 System_OnPwrChargeResume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
    //GxLED_SetCtrl(KEYSCAN_LED_GREEN,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT);
   // GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,TRUE);
    return NVTEVT_CONSUME;
}

void System_ResetPowerSaveCount(void)
{
    //reset auto power off and sleep counter
    #if (AUTOPWROFF_FUNCTION == ENABLE)
    GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_RESET,0);
    #endif
    #if (AUTOSLEEP_FUNCTION == ENABLE)
    GxPower_SetControl(GXPWR_CTRL_SLEEP_RESET,0);
    #endif
    GxCustom_SetControl(GXCUSTOM_CTRL_AUTOLCDOFF_RESET,0);	
}

#if (USB_CHARGE_FUNCTION == ENABLE)
void Power_StartUSBCharge(void)
{
    if (!gIsBattInsert)
    {
        DBG_DUMP("  NO Battery, Ignore START\r\n");
        return;
    }

    if (GxPower_GetControl(GXPWR_CTRL_BATTERY_CHARGE_EN))
    {
        DBG_DUMP("  Already UNDER Charge, Ignore START\r\n");
        return;
    }

    if((USB_GetSource() == USB_SRC_USB_ADAPTER))
    {
        GxPower_SetControl(GXPWR_CTRL_BATTERY_CHARGE_CURRENT,BATT_CHARGE_CURRENT_HIGH);
        DBG_MSG("  Charge Current = %d\r\n", BATT_CHARGE_CURRENT_HIGH);
    }
#if _MIPS_TODO
    //#NT#2010/12/10#Jeah Yen -begin
    if((USB_GetSource() == USB_SRC_USB_ADAPTER) && (Power_GetSource() != POWER_SRC_USB_ADAPTER))
    {
        //Do not charge, just stop check low-battery
        DBG_DUMP("GxPower: Stop low-battery check.\r\n");
        GxPower_SetControl(GXPWR_CTRL_BATTERY_DETECT_EN, FALSE);
        return;
    }
    //#NT#2010/12/10#Jeah Yen -end
#endif
    //#NT#2010/12/10#Jeah Yen -begin
    if((USB_GetSource() == USB_SRC_USB_PC))
    {
        GxPower_SetControl(GXPWR_CTRL_BATTERY_CHARGE_CURRENT,BATT_CHARGE_CURRENT_MEDIUM);
        DBG_MSG("  Charge Current = %d\r\n", BATT_CHARGE_CURRENT_MEDIUM);
    }
    //#NT#2010/12/10#Jeah Yen -end

    //#NT#2010/12/10#Jeah Yen -begin
    DBG_DUMP("GxPower: Stop low-battery check.\r\n");
    GxPower_SetControl(GXPWR_CTRL_BATTERY_DETECT_EN, FALSE);
    DBG_DUMP("GxPower: Start to Charge Battery\r\n");
#if (USB_CHARGE_VERIFY == ENABLE)
   // GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,TRUE);
#endif
    GxPower_SetControl(GXPWR_CTRL_BATTERY_CHARGE_EN,TRUE);
    //#NT#2010/12/10#Jeah Yen -end
#if (USB_CHARGE_VERIFY == ENABLE)
#else
    //GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
#endif
    //GxLED_SetCtrl(KEYSCAN_LED_RED,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT);
    //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,TRUE);
  //  GxLED_SetCtrl(KEYSCAN_LED_GREEN,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT);
  //  GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,TRUE);

    // disable flash re-charge
    SxTimer_SetFuncActive(SX_TIMER_DET_RECHARGE_ID,FALSE);
}
void Power_StopUSBCharge(void)
{
    //DBG_MSG("  gIsUSBInsert=%d,gIsBattInsert=%d,gIsBattDead=%d,gIsUSBAdapter=%d\r\n",gIsUSBInsert,gIsBattInsert,gIsBattDead,gIsUSBAdapter);
    if (!gIsBattInsert)
    {
        DBG_DUMP("  NO Battery, Ignore STOP\r\n");
        return;
    }
#if _MIPS_TODO
    //#NT#2010/12/10#Jeah Yen -begin
    if((USB_GetSource() == USB_SRC_USB_ADAPTER) && (Power_GetSource() != POWER_SRC_USB_ADAPTER))
    {
        //Not charge yet, just start check low-battery
        DBG_DUMP("GxPower: Start low-battery check.\r\n");
        GxPower_SetControl(GXPWR_CTRL_BATTERY_DETECT_EN, TRUE);
    }
    //#NT#2010/12/10#Jeah Yen -end

    //#NT#2011/3/30#Jeah Yen -begin
    if((USB_GetSource() == USB_SRC_NONE) && (Power_GetSource() != POWER_SRC_USB_ADAPTER))
    {
        //Cannot charge, start check low-battery
        DBG_DUMP("GxPower: Start low-battery check.\r\n");
        GxPower_SetControl(GXPWR_CTRL_BATTERY_DETECT_EN, TRUE);
    }
    //#NT#2011/3/30#Jeah Yen -end

    if((USB_GetSource() == USB_SRC_USB_ADAPTER))
    {
        //DBG_MSG("  USB Src = Adaptor\r\n");
        GxPower_SetControl(GXPWR_CTRL_BATTERY_CHARGE_CURRENT,BATT_CHARGE_CURRENT_LOW);
        DBG_MSG("  Charge Current = %d\r\n", BATT_CHARGE_CURRENT_LOW);
    }
#endif
    if((USB_GetSource() == USB_SRC_USB_PC))
    {
        //DBG_MSG("  USB Src = PC\r\n");
        GxPower_SetControl(GXPWR_CTRL_BATTERY_CHARGE_CURRENT,BATT_CHARGE_CURRENT_LOW);
        DBG_MSG("  Charge Current = %d\r\n", BATT_CHARGE_CURRENT_LOW);
    }

    if (!GxPower_GetControl(GXPWR_CTRL_BATTERY_CHARGE_EN))
    {
        DBG_DUMP("  Already NOT Charge, Ignore STOP\r\n");
        return;
    }

    //#NT#2010/12/10#Jeah Yen -begin
    DBG_DUMP("GxPower: Stop to Charge Battery\r\n");
    GxPower_SetControl(GXPWR_CTRL_BATTERY_CHARGE_EN,FALSE);
#if _MIPS_TODO
    if(!gIsUSBChargePreCheck
        && (GxSystem_GetState(SYSTEM_STATE_POWERON) != SYSTEM_POWERON_CHARGE)
        )
    {
#if (USB_CHARGE_VERIFY == ENABLE)
#else
       // GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,FALSE);
       // GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,TRUE);
#endif
        //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
        //GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
    }
#endif
    DBG_DUMP("GxPower: Start low-battery check.\r\n");
    GxPower_SetControl(GXPWR_CTRL_BATTERY_DETECT_EN, TRUE);
    // enable flash re-charge
    SxTimer_SetFuncActive(SX_TIMER_DET_RECHARGE_ID,TRUE);
    //#NT#2010/12/10#Jeah Yen -end

}
#endif

BOOL IsACCPlug(void)
{
	if(GPIOMap_DCDet())
		return TRUE;
	else
		return FALSE;
}

static BOOL IsACCPowerON =FALSE;
static BOOL SixMinPowerOFF = FALSE;
static BOOL HalfHourPowerOFF = FALSE;

BOOL GxCustom_IsDCPowerON(void)
{
	return IsACCPowerON;
	
}

void System_OnDCInsert(void)
{
	SixMinPowerOFF = FALSE;
	HalfHourPowerOFF = FALSE;
}

void System_OnDCRemove(void)
{
	SixMinPowerOFF = TRUE;
	HalfHourPowerOFF = FALSE;
}


void GxCustom_DetDCPlug(void)
{

 //   debug_msg("^G GxCustom_DetDCPlug IS_TurnONLCDBacklight: %d  \r\n",IS_TurnONLCDBacklight());
#if 0
    static BOOL bLastDc5vDet     = FALSE;
    static BOOL bLastDc5vStatus  = FALSE;
    BOOL        bCurDc5vDet, bCurDc5vStatus;
    static BOOL 	gFirstPowerON = TRUE;
    char message[160];
    static  int TimeCount = 0;
    bCurDc5vDet = GPIOMap_DCDet();
	
    if(gFirstPowerON)
    	{
		gFirstPowerON = FALSE;
		bLastDc5vStatus=DevCtrl_IsACCPoerOn();
		if(bLastDc5vStatus==FALSE)
		{
			HalfHourPowerOFF=TRUE;
		}
    	}
    bCurDc5vStatus = (BOOL)(bCurDc5vDet & bLastDc5vDet);

    if (bCurDc5vStatus != bLastDc5vStatus)
    {
        // DC 5V plugging in
        if (bCurDc5vStatus == TRUE)
        {
		// g_bDc5vPlug = TRUE;
		System_OnDCInsert();
		TimeCount = 0;
		debug_msg("DC Plug================1\r\n");
        }
        // DC 5V unplugged
        else
        {
		//  g_bDc5vPlug = FALSE;
		System_OnDCRemove();
		TimeCount = 0;
		debug_msg("DC Unplug================2\r\n");
        }
    }

	if((SixMinPowerOFF==TRUE) ||(HalfHourPowerOFF==TRUE))
	{
		TimeCount++;
		debug_msg("###  ACC Unplug  shutdown cnt = %d,SixMinPowerOFF:%d,HalfHourPowerOFF:%d\r\n",TimeCount,SixMinPowerOFF,HalfHourPowerOFF);
		if(SixMinPowerOFF &&TimeCount >= FIVE_MINUTES){
			SixMinPowerOFF=FALSE;
			SxTimer_SetFuncActive(SX_TIMER_DET_DCPLUG_ID,FALSE);
        		System_PowerOff(SYS_POWEROFF_NORMAL);  
			}
		if(HalfHourPowerOFF && TimeCount >= HALF_AN_HOUR){
			HalfHourPowerOFF=FALSE;
			SxTimer_SetFuncActive(SX_TIMER_DET_DCPLUG_ID,FALSE);			
        		System_PowerOff(SYS_POWEROFF_NORMAL);
			}
	}
    bLastDc5vDet     = bCurDc5vDet;
    bLastDc5vStatus  = bCurDc5vStatus;
    #endif
}

#if 0//GMA301
void UI_DetGsensor(void)
{
    Gsensor_Data GS_Data;
    INT32 gsensor_trig;
    static UINT32 X,Y,Z;
        if (System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_MOVIE)
        {
        	switch(UIFlowWndWiFiMovie_GetStatus())	
        	{
        	     case WIFI_MOV_ST_RECORD:
		        if(UI_GetData(FL_GSENSOR)!=GSENSOR_OFF)
			  {			  
				if(GSensor_GetData(&GS_Data)==TRUE)
		            {
		            		gsensor_trig=Get_GSensorSensitivity();
		                   if((abs(GS_Data.Axis.Xacc-X)>gsensor_trig)||(abs(GS_Data.Axis.Yacc-Y)>gsensor_trig)
						||(abs(GS_Data.Axis.Zacc-Z)>gsensor_trig))
		                   	{
		            			debug_msg("gsensor trig TRUE ...%d,%d,%d, threshold:%d,%d\r\n",GS_Data.Axis.Xacc,GS_Data.Axis.Yacc,GS_Data.Axis.Zacc,gsensor_trig,UIFlowWndMovie_IsGsensorTrig());
						if(UIFlowWndMovie_IsGsensorTrig()==FALSE)
						{
							//UIFlowWndMovie_SetGsensorTrigFlag(TRUE);
                        				Ux_PostEvent(NVTRET_PROTECT, 0);
						}	
		                   	}
					X=GS_Data.Axis.Xacc;
					Y=GS_Data.Axis.Yacc;
					Z=GS_Data.Axis.Zacc;					
		            }		
			    }
			break;
        	}
        }
}
#else  //DA380
void UI_MYDetGsensorStatus(void)
{
    short x = 0,y = 0,z = 0;
    static short prev_x= 0,prev_y= 0,prev_z= 0;
    static BOOL xyz_first = FALSE;
    INT32 xyz;
	
    if(UI_GetData(FL_GSENSOR)!=GSENSOR_OFF)
    {
        mir3da_read_data(&x,&y,&z);
        if(xyz_first == FALSE)
        {
            xyz_first = TRUE;
            prev_x = x;
            prev_y = y;
            prev_z = z;
        }

        {
            xyz = Get_GSensorSensitivity();
		
            if ((abs(x-prev_x )> xyz)||
                (abs(y-prev_y )> xyz)||
                (abs(z-prev_z )> xyz))
            {
	         	if(UI_GetData(FL_WIFI_MOVIE_FMT) == WIFI_RTSP_REC ||gMovData.State == MOV_ST_REC )
	         	{
                 	Ux_PostEvent(NVTRET_PROTECT, 0);
	         	}
            }
       }
		
	  if (SysGetFlag(FL_MOVIE_PARKING) == MOVIE_PARKING_ON)
	  {
	  		if ((abs(x-prev_x )> LAPSE_REC_TRIG_VALUE)||
                 (abs(y-prev_y )> LAPSE_REC_TRIG_VALUE)||
                 (abs(z-prev_z )> LAPSE_REC_TRIG_VALUE))
       		{
       			g_bLapseRecFlag = TRUE;
       		}
       		else
       		{
            	g_bLapseRecFlag = FALSE;
       		}
	   }  
	}
	
     prev_x = x;
     prev_y = y;
     prev_z = z;

}
#if 1
void UI_DetGsensor(void)
{
    static UINT32 staticCounter = 0;
    static UINT32 motionCounter = 0;
    static UINT32 DetGsensorCounter = 0;
	
   UI_MYDetGsensorStatus();
    if(UI_GetData(FL_GSENSOR)!=GSENSOR_OFF)
    {
         if (g_bLapseRecFlag)
	   debug_msg("######111111#######g_bLapseRecFlag:%d\r\n",g_bLapseRecFlag);
		 
        if (System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_MOVIE)
        {
        	switch(gMovData.State)	
        	{
        	     case MOV_ST_REC:
		     case MOV_ST_REC|MOV_ST_ZOOM:
			 	 if (SysGetFlag(FL_MOVIE_PARKING) == MOVIE_PARKING_ON)
			 	 	{

                                     	
                   				 if (SysGetFlag(FL_MOVIE_TIMELAPSE_REC) == MOVIE_TIMELAPSEREC_OFF)
    			  				  {
    			         				 if (!g_bLapseRecFlag)
                         						{
                        							staticCounter++;
        	            							motionCounter = 0;
						                        if ((!DxUSB_GetIsUSBPlug())&&(staticCounter >= 1000))//start time lapse recording
        	            							{
        	                						   FlowMovie_StopRec();
										               Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);			  
	        	                                             } 
        	           							 if (staticCounter > GSENSOR_STATIC_COUNTER)//start time lapse recording
        	            							{
        	                							Ux_PostEvent(NVTEVT_RECORDING_MODE_TRIG, 1, 1);
														bDetMOVIStatus = TRUE;
														nLapseRecLedOffCounter = 30;
        	           	            				} 
                        						}
    			    					}
    			    			else
    			    			{
    			                       if (g_bLapseRecFlag)
                    	               {
                        		                  motionCounter++;
					                  //DetGsensorCounter++;
    		            				     staticCounter = 0;
    		            		               if (motionCounter > GSENSOR_MOTION_COUNTER )//start normal recording
    		            		                {
    		            		                   //DetGsensorCounter=0;
												   GPIOMap_TurnOnLCDBacklight();
    		                		            	Ux_PostEvent(NVTEVT_RECORDING_MODE_TRIG, 1, 0);
						           					bDetMOVIStatus = TRUE;
													nLapseRecStatusCounter = 5;
    		            			          }	
                    	           }
    			    }
			 	 	}
			    break;
				 
		      case MOV_ST_VIEW:
                   case MOV_ST_VIEW|MOV_ST_ZOOM:	
				   if (bDetMOVIStatus == TRUE)
				   	{
				          bDetMOVIStatus =FALSE;
				          Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
			         break;
        	                      }
        		}
        }
    }
}
#else
void UI_DetGsensor(void)
{
    static UINT32 staticCounter = 0;
    static UINT32 motionCounter = 0;
	
   UI_MYDetGsensorStatus();
   
   if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE)
   {	 
         

        	switch(gMovData.State)	
        	{
        	 	case MOV_ST_REC:
		     	case MOV_ST_REC|MOV_ST_ZOOM:
					if(UI_GetData(FL_GSENSOR)!= GSENSOR_OFF)
					{
			 	 		if (SysGetFlag(FL_MOVIE_PARKING) == MOVIE_PARKING_ON)
			 	 		{
    			         	if (!g_bLapseRecFlag)
                         	{
                        		staticCounter++;
        	            		motionCounter = 0;
									
        	            		if ((staticCounter > GSENSOR_STATIC_COUNTER)&&(SysGetFlag(FL_MOVIE_TIMELAPSE_REC) == MOVIE_TIMELAPSEREC_OFF))//start time lapse recording
        	            	    {
        	            	    	debug_msg("magic_start time lapse recording\r\n");
        	                	 	Ux_PostEvent(NVTEVT_RECORDING_MODE_TRIG, 1, 1);
									nLapseRecLedOffCounter = 10;//vincent@20150919-6
        	           	        } 
                            }
    			    	    else
    			    		{
    			            	motionCounter++;
    		            	    staticCounter = 0;
    		            		if ((motionCounter > GSENSOR_MOTION_COUNTER ) && (SysGetFlag(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF))//start normal recording
    		            		{
    		            			GPIOMap_TurnOnLCDBacklight();
                            		debug_msg("magic_start normal recording\r\n");
    		                		Ux_PostEvent(NVTEVT_RECORDING_MODE_TRIG, 1, 0);
    		            	    }	
    			    	     }
			 	 	}
				}
			  break;		 
        }
    }
}
#endif

#endif

UINT32  GxCustom_GetControl(GXCUSTOM_CTRL CustomCtrl)
{
    UINT32 getv = 0;

    switch (CustomCtrl)
    {
        case GXCUSTOM_CTRL_AUTOLCDOFF_EN:
            getv = (UINT32)g_bKeyScanAutoLcdoffEn;
            break;
        case GXCUSTOM_CTRL_AUTOLCDOFF_TIME:
            getv = uiSystemAutoLcdoffTime;
            break;
        case GXCUSTOM_CTRL_DELAYOFF_EN:
            getv = (UINT32)g_bKeyScanDelayoffEn;
            break;
        case GXCUSTOM_CTRL_DELAYOFF_TIME:
            getv = (UINT32)uiSystemDelayoffTime;
            break;
        case GXCUSTOM_CTRL_GPS_TIME:
            getv = (UINT32)uiSystemGpsoffTime;
            break;
        
        default:
            DBG_ERR("CustomCtrl(%d)\r\n",CustomCtrl);
            getv = 0;
            break;
    }
    return getv;
}

void GxCustom_SetControl(GXCUSTOM_CTRL CustomCtrl, UINT32 value)
{
    DBG_IND("cmd=(%d), value=%d\r\n",CustomCtrl,value);
    switch (CustomCtrl)
    {
        case GXCUSTOM_CTRL_AUTOLCDOFF_EN:
            if (g_bKeyScanAutoLcdoffEn == FALSE && value == TRUE)
            {
                g_uiKeyScanAutoLcdoffCnt = 0;
            }
            g_bKeyScanAutoLcdoffEn = (INT32)value;
            break;
        case GXCUSTOM_CTRL_AUTOLCDOFF_TIME:
            uiSystemAutoLcdoffTime = value;
            DBG_IND("GXCUSTOM_CTRL_AUTOLCDOFF_TIME cnt = %d\r\n", uiSystemAutoLcdoffTime);
            break;
	  case GXCUSTOM_CTRL_AUTOLCDOFF_RESET:
	  	
	  	g_uiKeyScanAutoLcdoffCnt=0;
  //  debug_msg("--->GXCUSTOM_CTRL_AUTOLCDOFF_RESET-->%d\r\n",g_uiKeyScanAutoLcdoffCnt);
		
	  	break;
        case GXCUSTOM_CTRL_DELAYOFF_EN:
            if (g_bKeyScanDelayoffEn == FALSE && value == TRUE)
            {
                g_uiKeyScanDelayoffCnt = 0;
            }
            g_bKeyScanDelayoffEn = (INT32)value;
            DBG_IND("g_bKeyScanDelayoffEn cnt = %d\r\n", g_bKeyScanDelayoffEn);			
            break;
        case GXCUSTOM_CTRL_DELAYOFF_TIME:
            uiSystemDelayoffTime = value;
            DBG_IND("GXCUSTOM_CTRL_DELAYOFF_TIME cnt = %d\r\n", uiSystemDelayoffTime);
            break;

	 case GXCUSTOM_CTRL_DELAYOFF_RESET:
	 	g_uiKeyScanDelayoffCnt = 0;
	 	break;
        case GXCUSTOM_CTRL_GPS_EN:
            if (g_bKeyScanGpsoffEn == FALSE && value == TRUE)
            {
                g_uiKeyScanGpsoffCnt = 0;
            }
            g_bKeyScanGpsoffEn = (INT32)value;
            break;
        case GXCUSTOM_CTRL_GPS_TIME:
            uiSystemGpsoffTime = value;
            DBG_IND("GXCUSTOM_CTRL_GPS_TIME cnt = %d\r\n", uiSystemGpsoffTime);
            break;
        
        default:
            DBG_ERR("CustomCtrl(%d)\r\n",CustomCtrl);
            break;
    }
}

void GxCustom_DetAutoLcdoff(void)
{
    if ((GxCustom_GetControl(GXCUSTOM_CTRL_AUTOLCDOFF_TIME) != KEYSCAN_AUTOLCDOFF_DISABLED)
        && (g_bKeyScanAutoLcdoffEn == TRUE))
    {
        DBG_IND("Auto Lcd Off Cnt = %d\r\n", g_uiKeyScanAutoLcdoffCnt);

        g_uiKeyScanAutoLcdoffCnt++;

        if (g_uiKeyScanAutoLcdoffCnt >= GxCustom_GetControl(GXCUSTOM_CTRL_AUTOLCDOFF_TIME))
        {
            DBG_IND("Do Auto Lcd Off!\r\n");
            //System_PowerOff(SYS_POWEROFF_NORMAL);
            if(GPIOMap_IsLCDBacklightOn()==TRUE)
            {
			GPIOMap_TurnOffLCDBacklight();
			app_set_screen_status(1);
			GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,FALSE);
			GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);

                    
	      }               
        }
    }
}

void GxCustom_DetDelayoff(void)
{
      debug_msg("GxCustom_DetDelayoff*********************************ERIC_DEBUG\n\r");//ERIC_DEBUG
    //((GxCustom_GetControl(GXCUSTOM_CTRL_DELAYOFF_TIME) != KEYSCAN_DELAYOFF_DISABLED)
        //&& (g_bKeyScanDelayoffEn == TRUE))
    {
        DBG_IND("Auto DelayPwrOff Cnt = %d\r\n", g_uiKeyScanDelayoffCnt);
        g_uiKeyScanDelayoffCnt++;

       if ((g_uiKeyScanDelayoffCnt >= GxCustom_GetControl(GXCUSTOM_CTRL_DELAYOFF_TIME))||GetBatteryLevel() == BATTERY_EXHAUSTED||GetBatteryLevel() == BATTERY_EMPTY)
        {
            DBG_IND("Do AutoDelayPwrOff!\r\n");
            if(System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_MOVIE)
            {
                debug_msg("GxCustom_DetDelayoff================1\r\n");
                if(gMovData.State == MOV_ST_REC)
                {
          
                    FlowMovie_StopRec();
                }		
            }
			Ux_PostEvent(NVTEVT_KEY_POWER_REL, 1, NVTEVT_KEY_RELEASE); //Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
            
        }
    }
}

//eric edit
void UI_DetDelayPowerOff(void)  
{
    if ((GxCustom_GetControl(GXCUSTOM_CTRL_DELAYOFF_TIME) != KEYSCAN_DELAYOFF_DISABLED)
        && (g_bKeyScanDelayoffEn == TRUE))
    {
        debug_msg("Auto DelayPwrOff Cnt = %d\r\n", g_uiKeyScanDelayoffCnt);
        g_uiKeyScanDelayoffCnt++;

        if (g_uiKeyScanDelayoffCnt >=GxCustom_GetControl(GXCUSTOM_CTRL_DELAYOFF_TIME))
        {
             DBG_IND("Do AutoDelayPwrOff!\r\n");
	        if (System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_MOVIE)
	        {
	        	switch(gMovData.State)	
	        	{
	        	     case MOV_ST_REC:
			     case MOV_ST_REC|MOV_ST_ZOOM:
				//MediaRec_Stop(MEDIAREC_WAIT_END);  	
                		FlowMovie_StopRec();				
			     break;
	        	}
	        }			 
		Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
            
        }
    }
}
////eric edit


int app_check_screen_status(void)
{
	return m_is_screen_status;
}

void app_set_screen_status(int val)
{
	m_is_screen_status = val;
}