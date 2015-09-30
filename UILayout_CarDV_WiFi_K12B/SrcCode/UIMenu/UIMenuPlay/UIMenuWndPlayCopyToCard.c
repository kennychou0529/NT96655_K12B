//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndPlayCopyToCardRes.c"
#include "UIMenuWndPlayCopyToCard.h"
#include "PrjCfg.h"

//---------------------UIMenuWndPlayCopyToCardCtrl Debug Definition -----------------------------
#define _UIMENUWNDPLAYCOPYTOCARD_ERROR_MSG        1
#define _UIMENUWNDPLAYCOPYTOCARD_TRACE_MSG        0

#if (_UIMENUWNDPLAYCOPYTOCARD_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIMenuWndPlayCopyToCardErrMsg(...)            debug_msg ("^R UIMenuWndPlayCopyToCard: "__VA_ARGS__)
#else
#define UIMenuWndPlayCopyToCardErrMsg(...)
#endif

#if (_UIMENUWNDPLAYCOPYTOCARD_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIMenuWndPlayCopyToCardTraceMsg(...)          debug_msg ("^B UIMenuWndPlayCopyToCard: "__VA_ARGS__)
#else
#define UIMenuWndPlayCopyToCardTraceMsg(...)
#endif

//---------------------UIMenuWndPlayCopyToCardCtrl Global Variables -----------------------------

//---------------------UIMenuWndPlayCopyToCardCtrl Prototype Declaration  -----------------------

//---------------------UIMenuWndPlayCopyToCardCtrl Public API  ----------------------------------

//---------------------UIMenuWndPlayCopyToCardCtrl Private API  ---------------------------------
//---------------------UIMenuWndPlayCopyToCardCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPlayCopyToCard)
CTRL_LIST_ITEM(UIMenuWndPlayCopyToCard_Panel)
CTRL_LIST_END

//----------------------UIMenuWndPlayCopyToCardCtrl Event---------------------------
INT32 UIMenuWndPlayCopyToCard_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayCopyToCard_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayCopyToCard_OnChildClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayCopyToCard_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayCopyToCard_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayCopyToCard_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayCopyToCard_OnKeyShutter2(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPlayCopyToCard)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndPlayCopyToCard_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndPlayCopyToCard_OnClose)
EVENT_ITEM(NVTEVT_CHILD_CLOSE,UIMenuWndPlayCopyToCard_OnChildClose)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndPlayCopyToCard_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndPlayCopyToCard_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndPlayCopyToCard_OnKeyMode)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIMenuWndPlayCopyToCard_OnKeyShutter2)
EVENT_END

INT32 UIMenuWndPlayCopyToCard_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayCopyToCard_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayCopyToCard_OnChildClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayCopyToCard_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayCopyToCard_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayCopyToCard_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayCopyToCard_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndPlayCopyToCard_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPlayCopyToCard_Panel)
CTRL_LIST_ITEM(UIMenuWndPlayCopyToCard_Menu)
CTRL_LIST_END

//----------------------UIMenuWndPlayCopyToCard_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPlayCopyToCard_Panel)
EVENT_END

//----------------------UIMenuWndPlayCopyToCard_MenuCtrl Event---------------------------
INT32 UIMenuWndPlayCopyToCard_Menu_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayCopyToCard_Menu_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayCopyToCard_Menu_OnKeyEnter(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPlayCopyToCard_Menu)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndPlayCopyToCard_Menu_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndPlayCopyToCard_Menu_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndPlayCopyToCard_Menu_OnKeyEnter)
EVENT_END

INT32 UIMenuWndPlayCopyToCard_Menu_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayCopyToCard_Menu_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayCopyToCard_Menu_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}