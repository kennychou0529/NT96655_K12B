//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndPhotoQuickSettingRes.c"
#include "UIMenuWndPhotoQuickSetting.h"
#include "PrjCfg.h"

//---------------------UIMenuWndPhotoQuickSettingCtrl Debug Definition -----------------------------
#define _UIMENUWNDPHOTOQUICKSETTING_ERROR_MSG        1
#define _UIMENUWNDPHOTOQUICKSETTING_TRACE_MSG        0

#if (_UIMENUWNDPHOTOQUICKSETTING_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIMenuWndPhotoQuickSettingErrMsg(...)            debug_msg ("^R UIMenuWndPhotoQuickSetting: "__VA_ARGS__)
#else
#define UIMenuWndPhotoQuickSettingErrMsg(...)
#endif

#if (_UIMENUWNDPHOTOQUICKSETTING_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIMenuWndPhotoQuickSettingTraceMsg(...)          debug_msg ("^B UIMenuWndPhotoQuickSetting: "__VA_ARGS__)
#else
#define UIMenuWndPhotoQuickSettingTraceMsg(...)
#endif

//---------------------UIMenuWndPhotoQuickSettingCtrl Global Variables -----------------------------

//---------------------UIMenuWndPhotoQuickSettingCtrl Prototype Declaration  -----------------------

//---------------------UIMenuWndPhotoQuickSettingCtrl Public API  ----------------------------------

//---------------------UIMenuWndPhotoQuickSettingCtrl Private API  ---------------------------------
//---------------------UIMenuWndPhotoQuickSettingCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPhotoQuickSetting)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_Title_Panel)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_End_Panel)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_Left_Panel)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_Help_Status)
CTRL_LIST_END

//----------------------UIMenuWndPhotoQuickSettingCtrl Event---------------------------
INT32 UIMenuWndPhotoQuickSetting_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoQuickSetting_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoQuickSetting_OnChildClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoQuickSetting_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoQuickSetting_OnKeyShutter2(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPhotoQuickSetting)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndPhotoQuickSetting_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndPhotoQuickSetting_OnClose)
EVENT_ITEM(NVTEVT_CHILD_CLOSE,UIMenuWndPhotoQuickSetting_OnChildClose)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndPhotoQuickSetting_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIMenuWndPhotoQuickSetting_OnKeyShutter2)
EVENT_END

INT32 UIMenuWndPhotoQuickSetting_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoQuickSetting_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoQuickSetting_OnChildClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CHILD_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoQuickSetting_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_KEY_ENTER,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoQuickSetting_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndPhotoQS_Title_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPhotoQS_Title_Panel)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_Tile_Icon)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_Title_Mode_Status)
CTRL_LIST_END

//----------------------UIMenuWndPhotoQS_Title_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPhotoQS_Title_Panel)
EVENT_END

//----------------------UIMenuWndPhotoQS_Tile_IconCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPhotoQS_Tile_Icon)
EVENT_END

//----------------------UIMenuWndPhotoQS_Title_Mode_StatusCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPhotoQS_Title_Mode_Status)
EVENT_END

//---------------------UIMenuWndPhotoQS_End_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPhotoQS_End_Panel)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_EV_Menu)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_AWB_Menu)
CTRL_LIST_END

//----------------------UIMenuWndPhotoQS_End_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPhotoQS_End_Panel)
EVENT_END

//----------------------UIMenuWndPhotoQS_EV_MenuCtrl Event---------------------------
INT32 UIMenuWndPhotoQS_EV_Menu_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoQS_EV_Menu_OnKeyRight(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPhotoQS_EV_Menu)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndPhotoQS_EV_Menu_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIMenuWndPhotoQS_EV_Menu_OnKeyRight)
EVENT_END

INT32 UIMenuWndPhotoQS_EV_Menu_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoQS_EV_Menu_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndPhotoQS_AWB_MenuCtrl Event---------------------------
INT32 UIMenuWndPhotoQS_AWB_Menu_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoQS_AWB_Menu_OnKeyRight(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPhotoQS_AWB_Menu)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndPhotoQS_AWB_Menu_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIMenuWndPhotoQS_AWB_Menu_OnKeyRight)
EVENT_END

INT32 UIMenuWndPhotoQS_AWB_Menu_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoQS_AWB_Menu_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndPhotoQS_Left_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPhotoQS_Left_Panel)
CTRL_LIST_ITEM(UIMenuWndPhotoQS_Left_Menu)
CTRL_LIST_END

//----------------------UIMenuWndPhotoQS_Left_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPhotoQS_Left_Panel)
EVENT_END

//----------------------UIMenuWndPhotoQS_Left_MenuCtrl Event---------------------------
INT32 UIMenuWndPhotoQS_Left_Menu_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoQS_Left_Menu_OnKeyDown(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPhotoQS_Left_Menu)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndPhotoQS_Left_Menu_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndPhotoQS_Left_Menu_OnKeyDown)
EVENT_END

INT32 UIMenuWndPhotoQS_Left_Menu_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoQS_Left_Menu_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndPhotoQS_Help_StatusCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPhotoQS_Help_Status)
EVENT_END

