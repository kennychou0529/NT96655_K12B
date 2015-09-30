//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDPLAYMAGNIFYRES_H
#define UIFLOWWNDPLAYMAGNIFYRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_ITEM_RECT(CMD_Rectangle,Skin1Rect,0,0,319,239,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_BODY(Skin1Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

SHOW_ITEM_RECT(CMD_Rectangle,Skin3Rect,0,0,59,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin3)
SHOW_MAP_BODY(Skin3Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin3,0,0,59,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text5,0,0,59,27,STRID_NULL,1,1280,0,0,2,1,1,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal2)
SHOW_MAP_BODY(Skin3)
SHOW_MAP_BODY(Text5)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal2,41,207,100,234)

SHOW_MAP_BEGIN(UIFlowWndPlayMagnify_StaticTXT_DZoom)
SHOW_MAP_BODY(Normal2)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlayMagnify_StaticTXT_DZoom)
CREATE_STATIC_DATA(UIFlowWndPlayMagnify_StaticTXT_DZoom,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPlayMagnify_StaticTXT_DZoom,0,41,207,100,234)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin8)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin8,0,0,16,10)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon9, 0, 0, 16,10, ICONID_NULL,0,0,0,4,0,0,0,0)
SHOW_MAP_BEGIN(Normal7)
SHOW_MAP_BODY(Skin8)
SHOW_MAP_BODY(Icon9)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal7,259,7,275,17)

SHOW_MAP_BEGIN(UIFlowWndPlayMagnify_StaticIcon_DirUp)
SHOW_MAP_BODY(Normal7)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlayMagnify_StaticIcon_DirUp)
CREATE_STATIC_DATA(UIFlowWndPlayMagnify_StaticIcon_DirUp,ICON_DIR_UP)
CREATE_STATIC_CTRL(UIFlowWndPlayMagnify_StaticIcon_DirUp,0,259,7,275,17)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin12)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin12,0,0,16,10)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon13, 0, 0, 16,10, ICONID_NULL,0,0,0,4,0,0,0,0)
SHOW_MAP_BEGIN(Normal11)
SHOW_MAP_BODY(Skin12)
SHOW_MAP_BODY(Icon13)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal11,259,70,275,80)

SHOW_MAP_BEGIN(UIFlowWndPlayMagnify_StaticIcon_DirDown)
SHOW_MAP_BODY(Normal11)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlayMagnify_StaticIcon_DirDown)
CREATE_STATIC_DATA(UIFlowWndPlayMagnify_StaticIcon_DirDown,ICON_DIR_DOWN)
CREATE_STATIC_CTRL(UIFlowWndPlayMagnify_StaticIcon_DirDown,0,259,70,275,80)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin16)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin16,0,0,11,18)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon17, 0, 0, 11,18, ICONID_NULL,0,0,0,4,0,0,0,0)
SHOW_MAP_BEGIN(Normal15)
SHOW_MAP_BODY(Skin16)
SHOW_MAP_BODY(Icon17)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal15,302,34,313,52)

SHOW_MAP_BEGIN(UIFlowWndPlayMagnify_StaticIcon_DirRight)
SHOW_MAP_BODY(Normal15)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlayMagnify_StaticIcon_DirRight)
CREATE_STATIC_DATA(UIFlowWndPlayMagnify_StaticIcon_DirRight,ICON_DIR_RIGHT)
CREATE_STATIC_CTRL(UIFlowWndPlayMagnify_StaticIcon_DirRight,0,302,34,313,52)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin20)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin20,0,0,11,18)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon21, 0, 0, 11,18, ICONID_NULL,0,0,0,4,0,0,0,0)
SHOW_MAP_BEGIN(Normal19)
SHOW_MAP_BODY(Skin20)
SHOW_MAP_BODY(Icon21)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal19,222,34,233,52)

SHOW_MAP_BEGIN(UIFlowWndPlayMagnify_StaticIcon_DirLeft)
SHOW_MAP_BODY(Normal19)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlayMagnify_StaticIcon_DirLeft)
CREATE_STATIC_DATA(UIFlowWndPlayMagnify_StaticIcon_DirLeft,ICON_DIR_LEFT)
CREATE_STATIC_CTRL(UIFlowWndPlayMagnify_StaticIcon_DirLeft,0,222,34,233,52)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIFlowWndPlayMagnify)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlayMagnify)
DECLARE_CTRL_LIST(UIFlowWndPlayMagnify)
CREATE_CTRL(UIFlowWndPlayMagnify,UIFlowWndPlayMagnify,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
