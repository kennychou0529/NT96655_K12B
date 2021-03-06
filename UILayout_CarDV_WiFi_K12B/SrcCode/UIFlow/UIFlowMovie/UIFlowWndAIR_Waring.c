//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIFlowWndAIR_WaringRes.c"
#include "UIFlowWndAIR_Waring.h"
#include "PrjCfg.h"

//---------------------UIFlowWndAIR_WaringCtrl Debug Definition -----------------------------
#define _UIFLOWWNDAIR_WARING_ERROR_MSG        1
#define _UIFLOWWNDAIR_WARING_TRACE_MSG        0

#if (_UIFLOWWNDAIR_WARING_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIFlowWndAIR_WaringErrMsg(...)            debug_msg ("^R UIFlowWndAIR_Waring: "__VA_ARGS__)
#else
#define UIFlowWndAIR_WaringErrMsg(...)
#endif

#if (_UIFLOWWNDAIR_WARING_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIFlowWndAIR_WaringTraceMsg(...)          debug_msg ("^B UIFlowWndAIR_Waring: "__VA_ARGS__)
#else
#define UIFlowWndAIR_WaringTraceMsg(...)
#endif

//---------------------UIFlowWndAIR_WaringCtrl Global Variables -----------------------------

//---------------------UIFlowWndAIR_WaringCtrl Prototype Declaration  -----------------------

//---------------------UIFlowWndAIR_WaringCtrl Public API  ----------------------------------

//---------------------UIFlowWndAIR_WaringCtrl Private API  ---------------------------------
//---------------------UIFlowWndAIR_WaringCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndAIR_Waring)
CTRL_LIST_ITEM(UIFlowWndAIR_Waring_panel)
CTRL_LIST_ITEM(UIFlowWndAIR_Quility_title)
CTRL_LIST_ITEM(UIFlowWndAIR_Quility_value)
CTRL_LIST_ITEM(UIFlowWndAIR_Fresh_title)
CTRL_LIST_ITEM(UIFlowWndAIR_Fresh_value)
CTRL_LIST_ITEM(UIFlowWndAIR_PW25)
CTRL_LIST_ITEM(UIFlowWndAIR_PW25_value)
CTRL_LIST_END

//----------------------UIFlowWndAIR_WaringCtrl Event---------------------------
INT32 UIFlowWndAIR_Waring_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndAIR_Waring_OnClose(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndAIR_Waring)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndAIR_Waring_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndAIR_Waring_OnClose)
EVENT_END

INT32 UIFlowWndAIR_Waring_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndAIR_Waring_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
//---------------------UIFlowWndAIR_Waring_panelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndAIR_Waring_panel)
CTRL_LIST_END

//----------------------UIFlowWndAIR_Waring_panelCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndAIR_Waring_panel)
EVENT_END

//----------------------UIFlowWndAIR_Quility_titleCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndAIR_Quility_title)
EVENT_END

//----------------------UIFlowWndAIR_Quility_valueCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndAIR_Quility_value)
EVENT_END

//----------------------UIFlowWndAIR_Fresh_titleCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndAIR_Fresh_title)
EVENT_END

//----------------------UIFlowWndAIR_Fresh_valueCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndAIR_Fresh_value)
EVENT_END

//----------------------UIFlowWndAIR_PW25Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndAIR_PW25)
EVENT_END

//----------------------UIFlowWndAIR_PW25_valueCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndAIR_PW25_value)
EVENT_END

