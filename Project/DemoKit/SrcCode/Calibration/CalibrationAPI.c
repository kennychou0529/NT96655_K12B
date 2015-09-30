/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTsk.c
    @ingroup    mIPRJAPTest

    @brief      Calibration task API
                Calibration task API

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Kernel.h"
#include "SysCfg.h"
#include "CalibrationInt.h"
#include "FileSysTsk.h"
#include "UIFlow.h"

#include "UIFramework.h"
#include "UIGraphics.h"
#include "CalibrationTsk.h"

static DC**     pDCList;

//1. 中間圓對焦OK時的 "對比值"(Param_01)
//2. 週圍圓對焦OK時的 "對比值"(Param_02)
//3. 中間圓 "對比值" 的 tolerance(Param_03)
//4. 週圍圓 "對比值" 的 tolerance(Param_04)
//5. 需要粗調時, "對比值" 的顯示顏色(Param_05)
//6. 需要微調時, "對比值" 的顯示顏色(Param_06)
//7. 調焦OK時, "對比值" 顯示的顏色(Param_07)
typedef enum
{
    FocusCenter,                //Param_01
    FocusEdge,                  //Param_02
    FocusCenterTolerance,    //Param_03
    FocusEdgeTolerance,      //Param_04
    FocusRoughColor,         //Param_05
    FocusFineColor,        //Param_06
    FocusOkColor,           //Param_07
    FocusTotal
}CAL_PARAM_SET;


UINT32 g_CalParam[FocusTotal];

/*
    0       1
        2
    3       4
*/
// 2 is center point,0,1,3,4 are edge point
void Cal_ShowFocusResult(UINT32 *pFocusPoint)
{
    char str[64];
    UINT32 leftX = 50 ;
    UINT32 rightX = 240;
    UINT32 upY = 40;
    UINT32 downY =180;

    if(!pFocusPoint)
        return;
    debug_err(("pFocusPoint = %d,%d,%d,%d,%d\r\n",pFocusPoint[0],pFocusPoint[1],pFocusPoint[2],pFocusPoint[3],pFocusPoint[4]));
    sprintf(str,"%ld",pFocusPoint[0]);
    if(pFocusPoint[0]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,leftX,upY,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[0]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,leftX,upY,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,leftX,upY,g_CalParam[FocusRoughColor]);

    sprintf(str,"%ld",pFocusPoint[1]);
    if(pFocusPoint[1]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,rightX,upY,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[1]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,rightX,upY,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,rightX,upY,g_CalParam[FocusRoughColor]);

    //center point
    sprintf(str,"%ld",pFocusPoint[2]);
    if(pFocusPoint[2]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,(leftX+rightX)/2 ,(upY+downY)/2,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[2]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,(leftX+rightX)/2 ,(upY+downY)/2,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,(leftX+rightX)/2 ,(upY+downY)/2,g_CalParam[FocusRoughColor]);


    sprintf(str,"%ld",pFocusPoint[3]);
    if(pFocusPoint[3]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,leftX,downY,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[3]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,leftX,downY,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,leftX,downY,g_CalParam[FocusRoughColor]);

    sprintf(str,"%ld",pFocusPoint[4]);
    if(pFocusPoint[4]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,rightX,downY,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[4]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,rightX,downY,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,rightX,downY,g_CalParam[FocusRoughColor]);
}

void Cal_ShowStringWithColor(char *pStr, UINT16 X, UINT16 Y,UINT32 color)
{
    URECT rect;

    rect.x = X;
    rect.y = Y;
    rect.w = 320;//OSD_W;
    rect.h = 30;//OSD_H;

    pDCList = (DC**)UI_BeginScreen();

    GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD],rect.x,rect.y,rect.x+rect.w,rect.y+rect.h);
    //debug_err(("c %d %s \r\n",color ,pStr));
    GxGfx_SetTextColor(color, color, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], rect.x, rect.y, pStr);
    UI_EndScreen((UINT32)pDCList);
}

void Cal_ClearOSD(UINT32 uiColorIdx)
{
    DC** pDCList;

    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(0, uiColorIdx, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD], 0, 0, pDCList[GxGfx_OSD]->size.w, pDCList[GxGfx_OSD]->size.h);
    UI_EndScreen((UINT32)pDCList);
}

void Cal_ShowRectStringWithColor(CHAR *pStr, PURECT pRect, BOOL bClear,UINT32 color)
{
    pDCList = (DC**)UI_BeginScreen();
    if(bClear)
    {
        GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
        GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->x,pRect->y,pRect->x+pRect->w,pRect->y+pRect->h);
    }
    GxGfx_SetTextColor(color, color, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], pRect->x, pRect->y, pStr);
    UI_EndScreen((UINT32)pDCList);
}

void Cal_ShowStringByColor(CHAR *pStr, PURECT pRect, UINT32 uiFrColor, UINT32 uiBgColor)
{
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(uiBgColor, uiBgColor, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->x,pRect->y,pRect->x+pRect->w,pRect->y+pRect->h);
    GxGfx_SetTextColor(uiFrColor, 0, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], pRect->x, pRect->y, pStr);
    UI_EndScreen((UINT32)pDCList);
}


void Cal_FillRect(PURECT pRect, UINT32 uiBgColor)
{
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(uiBgColor, uiBgColor, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->x,pRect->y,pRect->x+pRect->w,pRect->y+pRect->h);
    UI_EndScreen((UINT32)pDCList);
}

//@}
