//This source code is generated by UI Designer Studio.

#ifndef UIMENUWNDPRINT_CROPRES_H
#define UIMENUWNDPRINT_CROPRES_H

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

SHOW_MAP_BEGIN(Skin3)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin3,0,0,59,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text5,0,0,59,29,STRID_NULL,1,1280,0,0,2,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal2)
SHOW_MAP_BODY(Skin3)
SHOW_MAP_BODY(Text5)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal2,253,8,312,37)

CREATE_STATE_ITEM_DATA(STRID_CROP6,STRID_CROP,ICONID_NULL,NULL) 
SHOW_MAP_BEGIN(UIMenuWndPrint_Crop_Title_Text_Status)
SHOW_MAP_BODY(Normal2)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPrint_Crop_Title_Text_Status)
STATE_LIST_BEGIN(UIMenuWndPrint_Crop_Title_Text_Status)
STATE_LIST_ITEM(STRID_CROP6)
STATE_LIST_END

CREATE_STATE_DATA(UIMenuWndPrint_Crop_Title_Text_Status,0)
CREATE_STATE_CTRL(UIMenuWndPrint_Crop_Title_Text_Status,0,253,8,312,37)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_IMAGE(CMD_Image,Image8, 0, 0, 59,23, ICON_MENU_RETURN,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image9, 177, 0, 196,23, ICON_DIR_LEFT,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image10, 197, 0, 216,23, ICON_DIR_UP,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image11, 217, 0, 236,23, ICON_DIR_DOWN,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image12, 237, 0, 256,23, ICON_DIR_RIGHT,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image13, 257, 0, 296,23, ICON_OK,83886080,0,0,0,0,0,17,0)
SHOW_MAP_BEGIN(Skin7)
SHOW_MAP_BODY(Image8)
SHOW_MAP_BODY(Image9)
SHOW_MAP_BODY(Image10)
SHOW_MAP_BODY(Image11)
SHOW_MAP_BODY(Image12)
SHOW_MAP_BODY(Image13)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin7,0,0,296,23)

SHOW_MAP_BEGIN(UIMenuWndPrint_Crop_End_Dir_Menu_Panel)
SHOW_MAP_BODY(Skin7)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPrint_Crop_End_Dir_Menu_Panel)
DECLARE_CTRL_LIST(UIMenuWndPrint_Crop_End_Dir_Menu_Panel)
CREATE_CTRL(UIMenuWndPrint_Crop_End_Dir_Menu_Panel,UIMenuWndPrint_Crop_End_Dir_Menu_Panel,CTRL_WND,NULL,0 ,12,213,308,236)
SHOW_MAP_BEGIN(UIMenuWndPrint_Crop)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPrint_Crop)
DECLARE_CTRL_LIST(UIMenuWndPrint_Crop)
CREATE_CTRL(UIMenuWndPrint_Crop,UIMenuWndPrint_Crop,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
