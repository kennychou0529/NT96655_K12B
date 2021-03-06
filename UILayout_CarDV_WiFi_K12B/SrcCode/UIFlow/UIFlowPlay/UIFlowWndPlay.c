//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIFlowWndPlayRes.c"
#include "UIFlowWndPlay.h"
#include "PrjCfg.h"

//---------------------UIFlowWndPlayCtrl Debug Definition -----------------------------
#define _UIFLOWWNDPLAY_ERROR_MSG        1
#define _UIFLOWWNDPLAY_TRACE_MSG        0

#if (_UIFLOWWNDPLAY_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIFlowWndPlayErrMsg(...)            debug_msg ("^R UIFlowWndPlay: "__VA_ARGS__)
#else
#define UIFlowWndPlayErrMsg(...)
#endif

#if (_UIFLOWWNDPLAY_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIFlowWndPlayTraceMsg(...)          debug_msg ("^B UIFlowWndPlay: "__VA_ARGS__)
#else
#define UIFlowWndPlayTraceMsg(...)
#endif

//---------------------UIFlowWndPlayCtrl Global Variables -----------------------------

//---------------------UIFlowWndPlayCtrl Prototype Declaration  -----------------------

//---------------------UIFlowWndPlayCtrl Public API  ----------------------------------

//---------------------UIFlowWndPlayCtrl Private API  ---------------------------------
//---------------------UIFlowWndPlayCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlay)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticICN_DSCMode)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Flash)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_EV)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Size)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_WB)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Quality)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Sharpness)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Storage)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Battery)
CTRL_LIST_ITEM(UIFlowWndPlay_TabHistogram)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Filename)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticICN_Protect)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Date)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Time)
CTRL_LIST_ITEM(UIFlowWndPlay_Histo)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_MovPlay)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_MovBwd)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_MovPlayTime)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_MovFwd)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticICN_Print)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Speed)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_MovStop)
CTRL_LIST_END

//----------------------UIFlowWndPlayCtrl Event---------------------------
INT32 UIFlowWndPlay_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyRight(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyZoomin(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyZoomout(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyDisplay(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnChildClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnBattery(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyShutter2(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnMovieFinish(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnMovieOneSec(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnStorageInit(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnStorageChange(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlay)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndPlay_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndPlay_OnClose)
EVENT_ITEM(NVTEVT_KEY_UP,UIFlowWndPlay_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIFlowWndPlay_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIFlowWndPlay_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIFlowWndPlay_OnKeyRight)
EVENT_ITEM(NVTEVT_KEY_ZOOMIN,UIFlowWndPlay_OnKeyZoomin)
EVENT_ITEM(NVTEVT_KEY_ZOOMOUT,UIFlowWndPlay_OnKeyZoomout)
EVENT_ITEM(NVTEVT_KEY_MENU,UIFlowWndPlay_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_DISPLAY,UIFlowWndPlay_OnKeyDisplay)
EVENT_ITEM(NVTEVT_KEY_MODE,UIFlowWndPlay_OnKeyMode)
EVENT_ITEM(NVTEVT_CHILD_CLOSE,UIFlowWndPlay_OnChildClose)
EVENT_ITEM(NVTEVT_BATTERY,UIFlowWndPlay_OnBattery)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIFlowWndPlay_OnKeyShutter2)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIFlowWndPlay_OnKeyEnter)
EVENT_ITEM(NVTEVT_CB_MOVIE_FINISH,UIFlowWndPlay_OnMovieFinish)
EVENT_ITEM(NVTEVT_CB_MOVIE_ONE_SEC,UIFlowWndPlay_OnMovieOneSec)
EVENT_ITEM(NVTEVT_STORAGE_INIT,UIFlowWndPlay_OnStorageInit)
EVENT_ITEM(NVTEVT_STORAGE_CHANGE,UIFlowWndPlay_OnStorageChange)
EVENT_END

INT32 UIFlowWndPlay_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyZoomin(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyZoomout(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyDisplay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnChildClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CHILD_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnBattery(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnMovieFinish(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnMovieOneSec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnStorageInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnStorageChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndPlay_StaticICN_DSCModeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticICN_DSCMode)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_FlashCtrl Event---------------------------
INT32 UIFlowWndPlay_StatusICN_Flash_OnKeyRight(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Flash)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIFlowWndPlay_StatusICN_Flash_OnKeyRight)
EVENT_END

INT32 UIFlowWndPlay_StatusICN_Flash_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndPlay_StatusICN_EVCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_EV)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_SizeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Size)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_WBCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_WB)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_QualityCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Quality)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_SharpnessCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Sharpness)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_StorageCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Storage)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_BatteryCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Battery)
EVENT_END

//----------------------UIFlowWndPlay_TabHistogramCtrl Event---------------------------
INT32 UIFlowWndPlay_TabHistogram_OnRedraw(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlay_TabHistogram)
EVENT_ITEM(NVTEVT_REDRAW,UIFlowWndPlay_TabHistogram_OnRedraw)
EVENT_END

INT32 UIFlowWndPlay_TabHistogram_OnRedraw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_REDRAW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndPlay_StaticTXT_FilenameCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Filename)
EVENT_END

//----------------------UIFlowWndPlay_StaticICN_ProtectCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticICN_Protect)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_DateCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Date)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_TimeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Time)
EVENT_END

//----------------------UIFlowWndPlay_HistoCtrl Event---------------------------
INT32 UIFlowWndPlay_Histo_OnRedraw(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlay_Histo)
EVENT_ITEM(NVTEVT_REDRAW,UIFlowWndPlay_Histo_OnRedraw)
EVENT_END

INT32 UIFlowWndPlay_Histo_OnRedraw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndPlay_StatusICN_MovPlayCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_MovPlay)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_MovBwdCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_MovBwd)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_MovPlayTime)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_MovFwdCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_MovFwd)
EVENT_END

//----------------------UIFlowWndPlay_StaticICN_PrintCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticICN_Print)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_SpeedCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Speed)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_MovStopCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_MovStop)
EVENT_END

