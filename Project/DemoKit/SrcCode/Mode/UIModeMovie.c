////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMovie.h"
#include "UIAppMovie.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIFlow.h"
#else
#include "FlowMovie.h"
#endif
#include "UIModeWiFi.h"

//UIInfo
#include "UIInfo.h"

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
#include "UIGraphics.h"
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
////////////////////////////////////////////////////////////////////////////////
#if _DEMO_TODO
#include "DxCamera.h"
#endif
#include "DeviceCtrl.h"
#include "EngineerMode.h"

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
extern INT32 MovieExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern BOOL g_bPreviewFirstTime;
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end

int PRIMARY_MODE_MOVIE = -1;      ///< Movie

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
void FlowMovie_Open_FastSensor(void)
{
#if _DEMO_TODO
    // should wait until part-2 finish
    //#NT#2010/09/17#Jeah Yen -begin
    //TM_Begin(CCD_WAITP2);
    INIT_WAITFLAG(FLGINIT_ENDCOPYPART2);
    //TM_End(CCD_WAITP2);
    //#NT#2010/09/17#Jeah Yen -end
#endif

    //speed up: 提前做change mode時sensor要做的事情
    g_bPreviewFirstTime = TRUE; //turn on fast path
    //TM_Begin(APP_OPEN);
    // Init Movie app and open
    MovieExe_OnOpen(0, 0, 0);
    //TM_End(APP_OPEN);

    //TM_Begin(DEVCTRL_CCD);
    DevCtrl_ModeMovie_FastSensor();
    //TM_End(DEVCTRL_CCD);
    DevCtrl_ModePhoto_WaitFinish();
}

void FlowMovie_Open_FastLens(void)
{
    //TM_Begin(DEVCTRL_LENS);
    //Lens_OnSystem(LENS_CMD_POWERON_ADVANCE);
    DevCtrl_Lens_PowerOn_Advance(); //simple path

    //speed up: 提前做change mode時lens要做的事情
    g_bPreviewFirstTime = TRUE; //turn on fast path
    DevCtrl_ModeMovie_FastLens();
    //TM_End(DEVCTRL_LENS);
}
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end


void ModeMovie_Open(void);
void ModeMovie_Close(void);

void ModeMovie_Open(void)
{
    Input_ResetMask();

    // Init movie app and open
    Ux_SetActiveApp(&CustomMovieObjCtrl);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
    if(g_bPreviewFirstTime)
    {
        //skip
        //GxSystem_Mark("APP-SKIP\r\n");
    }
    else
    {
        //TM_Begin(DEVCTRL_CCD);
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeMovie();
        //TM_End(DEVCTRL_CCD);
        //TM_Begin(APP_OPEN);
        // Init Photo app and open
        Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
        //TM_End(APP_OPEN);
    }
    if(!g_bPreviewFirstTime)
    {
        DevCtrl_ModeMovie_WaitFinish();
    }


    //TM_Begin(UI_OPENWIN);
    UI_SetData(FL_ModeIndex, UI_GetData(FL_NextMode));
/*
    //disable video1
    UI_Show(UI_SHOW_PREVIEW, TRUE);
    //enable video2
    UI_Show(UI_SHOW_QUICKVIEW, FALSE);
*/
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_OpenWifiWnd();
    }
    else
    {
#if (UI_STYLE==UI_STYLE_DRIVER)
        gMovData.State = MOV_ST_VIEW;
        Ux_OpenWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
#else
        Ux_OpenWindow((VControl *)(&FlowMovieCtrl), 0);
#endif
    }
    //TM_End(UI_OPENWIN);

    g_bPreviewFirstTime = FALSE; //turn off fast path
    //TM_End(OFF_TO_CAPTURE);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
}
void ModeMovie_Close(void)
{
    UINT32 i;

    //Ux_CloseWindowClear((VControl *)(&FlowMovieCtrl), 0);
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_CloseWifiWnd();
    }
    else
    {
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_CloseWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
#else

        Ux_CloseWindow((VControl *)(&FlowMovieCtrl), 0);
#endif
    }

    //#NT#2010/12/13#Photon Lin -begin
    //#Modify flow of DIS
    #if (MOVIE_DIS == ENABLE)
    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIEDIS_ENABLE,1,0);
    #endif
    //#NT#2010/12/13#Photon Lin -end
    //#NT#2011/03/30#Photon Lin -begin
    //#NT#Wait background task idle
    #if (_CALIBRATION_MODE_ == ENABLE)
    if (!g_bCalbirationOpened)
    #endif
    {
        for(i=0; i<500; i++)
        {
            if (!BKG_GetTskBusy())
            {
                break;
            }
            SwTimer_DelayMs(100);
        }
        if (i==500)
        {
            debug_err(("^R Wait background task idle timeout \r\n"));
        }
    }
    //#NT#2011/03/30#Photon Lin -end
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

void FlowMovie_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize);

SYS_MODE gModeMovie =
{
    "MOVIE",
    ModeMovie_Open,
    ModeMovie_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
