//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndCalibrationRes.c"
#include "UIMenuWndCalibration.h"
#include "PrjCfg.h"

//---------------------UIMenuWndCalibrationCtrl Debug Definition -----------------------------
#define _UIMENUWNDCALIBRATION_ERROR_MSG        1
#define _UIMENUWNDCALIBRATION_TRACE_MSG        0

#if (_UIMENUWNDCALIBRATION_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIMenuWndCalibrationErrMsg(...)            debug_msg ("^R UIMenuWndCalibration: "__VA_ARGS__)
#else
#define UIMenuWndCalibrationErrMsg(...)
#endif

#if (_UIMENUWNDCALIBRATION_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIMenuWndCalibrationTraceMsg(...)          debug_msg ("^B UIMenuWndCalibration: "__VA_ARGS__)
#else
#define UIMenuWndCalibrationTraceMsg(...)
#endif

//---------------------UIMenuWndCalibrationCtrl Global Variables -----------------------------

//---------------------UIMenuWndCalibrationCtrl Prototype Declaration  -----------------------

//---------------------UIMenuWndCalibrationCtrl Public API  ----------------------------------

//---------------------UIMenuWndCalibrationCtrl Private API  ---------------------------------
//---------------------UIMenuWndCalibrationCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndCalibration)
CTRL_LIST_ITEM(UIMenuWndCalibration_Background)
CTRL_LIST_ITEM(UIMenuWndCalibration_TitleBar)
CTRL_LIST_ITEM(UIMenuWndCalibration_TipsBar)
CTRL_LIST_ITEM(UIMenuWndCalibration_Item)
CTRL_LIST_END

//----------------------UIMenuWndCalibrationCtrl Event---------------------------
INT32 UIMenuWndCalibration_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndCalibration_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndCalibration_OnChildClose(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndCalibration)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndCalibration_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndCalibration_OnClose)
EVENT_ITEM(NVTEVT_CHILD_CLOSE,UIMenuWndCalibration_OnChildClose)
EVENT_END

INT32 UIMenuWndCalibration_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndCalibration_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndCalibration_OnChildClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CHILD_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndCalibration_BackgroundCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndCalibration_Background)
CTRL_LIST_END

//----------------------UIMenuWndCalibration_BackgroundCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndCalibration_Background)
EVENT_END

//---------------------UIMenuWndCalibration_TitleBarCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndCalibration_TitleBar)
CTRL_LIST_ITEM(UIMenuWndCalibration_TitleName)
CTRL_LIST_END

//----------------------UIMenuWndCalibration_TitleBarCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndCalibration_TitleBar)
EVENT_END

//----------------------UIMenuWndCalibration_TitleNameCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndCalibration_TitleName)
EVENT_END

//---------------------UIMenuWndCalibration_TipsBarCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndCalibration_TipsBar)
CTRL_LIST_ITEM(UIMenuWndCalibration_UDOKTips)
CTRL_LIST_END

//----------------------UIMenuWndCalibration_TipsBarCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndCalibration_TipsBar)
EVENT_END

//---------------------UIMenuWndCalibration_UDOKTipsCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndCalibration_UDOKTips)
CTRL_LIST_END

//----------------------UIMenuWndCalibration_UDOKTipsCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndCalibration_UDOKTips)
EVENT_END

//----------------------UIMenuWndCalibration_ItemCtrl Event---------------------------
INT32 UIMenuWndCalibration_Item_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndCalibration_Item_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndCalibration_Item_OnKeyEnter(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndCalibration_Item)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndCalibration_Item_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndCalibration_Item_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndCalibration_Item_OnKeyEnter)
EVENT_END

INT32 UIMenuWndCalibration_Item_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndCalibration_Item_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndCalibration_Item_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
