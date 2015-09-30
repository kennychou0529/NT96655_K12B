//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndPlaySetDPOFRes.c"
#include "UIMenuWndPlaySetDPOF.h"
#include "PrjCfg.h"

//---------------------UIMenuWndPlaySetDPOFCtrl Debug Definition -----------------------------
#define _UIMENUWNDPLAYSETDPOF_ERROR_MSG        1
#define _UIMENUWNDPLAYSETDPOF_TRACE_MSG        0

#if (_UIMENUWNDPLAYSETDPOF_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIMenuWndPlaySetDPOFErrMsg(...)            debug_msg ("^R UIMenuWndPlaySetDPOF: "__VA_ARGS__)
#else
#define UIMenuWndPlaySetDPOFErrMsg(...)
#endif

#if (_UIMENUWNDPLAYSETDPOF_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIMenuWndPlaySetDPOFTraceMsg(...)          debug_msg ("^B UIMenuWndPlaySetDPOF: "__VA_ARGS__)
#else
#define UIMenuWndPlaySetDPOFTraceMsg(...)
#endif

//---------------------UIMenuWndPlaySetDPOFCtrl Global Variables -----------------------------

//---------------------UIMenuWndPlaySetDPOFCtrl Prototype Declaration  -----------------------

//---------------------UIMenuWndPlaySetDPOFCtrl Public API  ----------------------------------

//---------------------UIMenuWndPlaySetDPOFCtrl Private API  ---------------------------------
//---------------------UIMenuWndPlaySetDPOFCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPlaySetDPOF)
CTRL_LIST_ITEM(UIMenuWndPlaySetDPOF_Panel)
CTRL_LIST_END

//----------------------UIMenuWndPlaySetDPOFCtrl Event---------------------------
INT32 UIMenuWndPlaySetDPOF_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_OnChildClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_OnKeyShutter2(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPlaySetDPOF)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndPlaySetDPOF_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndPlaySetDPOF_OnClose)
EVENT_ITEM(NVTEVT_CHILD_CLOSE,UIMenuWndPlaySetDPOF_OnChildClose)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndPlaySetDPOF_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndPlaySetDPOF_OnKeyMode)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIMenuWndPlaySetDPOF_OnKeyShutter2)
EVENT_END

INT32 UIMenuWndPlaySetDPOF_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_OnChildClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndPlaySetDPOF_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPlaySetDPOF_Panel)
CTRL_LIST_ITEM(UIMenuWndPlaySetDPOF_Button)
CTRL_LIST_ITEM(UIMenuWndPlaySetDPOF_Static)
CTRL_LIST_ITEM(UIMenuWndPlaySetDPOF_Static2)
CTRL_LIST_END

//----------------------UIMenuWndPlaySetDPOF_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPlaySetDPOF_Panel)
EVENT_END

//----------------------UIMenuWndPlaySetDPOF_ButtonCtrl Event---------------------------
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyRight(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyEnter(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPlaySetDPOF_Button)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndPlaySetDPOF_Button_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndPlaySetDPOF_Button_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndPlaySetDPOF_Button_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIMenuWndPlaySetDPOF_Button_OnKeyRight)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndPlaySetDPOF_Button_OnKeyEnter)
EVENT_END

INT32 UIMenuWndPlaySetDPOF_Button_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlaySetDPOF_Button_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndPlaySetDPOF_StaticCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPlaySetDPOF_Static)
EVENT_END

//----------------------UIMenuWndPlaySetDPOF_Static2Ctrl Event---------------------------
EVENT_BEGIN(UIMenuWndPlaySetDPOF_Static2)
EVENT_END

