#ifndef _IPL_CALIBRATION_H_
#define _IPL_CALIBRATION_H_
/**
    IPL calibration data structure

    Describe calibration items which are supported.
    including working buffer control mech., calibration environment, calibration algorithm, calibration UI control.

    @file       ipl_calibration.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#include "IPL_Calibration_3A.h"
#include "IPL_Calibration_ALG.h"
#include "IPL_Calibration_BUF.h"
#include "IPL_Ctrl.h"
/** \addtogroup mILibCal */
//@{

#if 0
    - UI
#endif

/**
    calibration UI event
*/
typedef enum _CalUICB
{
    UICB_NONE       = 0,    ///< -
    UICB_START      = 1,    ///< prepare to initiate calibration item.
    UICB_WAITKEY    = 2,    ///
    UICB_PROCESSING = 3,    ///< -
    UICB_END        = 4,    ///< -
    ENUM_DUMMY4WORD(CalUICB)
}CalUICB;


/**
    calibration String Color
*/
typedef enum _CalSTRCOLOR
{
    UICOL_RED       = 0,    ///< -
    UICOL_GREEN     = 1,    ///< -
    UICOL_BLUE      = 2,    ///< -
    UICOL_WHITE     = 3,    ///< -
    UICOL_BLACK     = 4,    ///< -
    UICOL_GRAY      = 5,    ///< -
    UICOL_YELLOW    = 6,    ///< -
    ENUM_DUMMY4WORD(CalSTRCOLOR)
}CalSTRCOLOR;

/**
    function prototype of calibration UI control
    CalUICB: event
    CHAR*:   string
*/
typedef void (*fpUI_cb)(CalUICB,CHAR*,UINT32);

/**
    calibration item. UI structure
    @note plz refer to CAL_UICB()
*/
typedef struct _CAL_UICB_OBJ
{
    fpUI_cb     AWB_UI;         ///< plz refer to CAL_UICB()
    fpUI_cb     ISO_UI;         ///< plz refer to CAL_UICB()
    fpUI_cb     ECS_UI;         ///< plz refer to CAL_UICB()
    fpUI_cb     MSHUT_UI;       ///< plz refer to CAL_UICB()
    fpUI_cb     DP_W_UI;        ///< plz refer to CAL_UICB()
    fpUI_cb     DP_D_UI;        ///< plz refer to CAL_UICB()
}CAL_UICB_OBJ,*pCAL_UICB_OBJ;

/**
     Calibration item UI control

     control calibration item UI, (like wait key, show string ... etc.)

     @return Description of data returned.
         - @b AWB_UI:   Item: AWB UI control
         - @b ISO_UI:   Item: ISO UI control
         - @b ECS_UI:   Item: ECS UI control
         - @b MSHUT_UI: Item: MSHUT UI control
         - @b DP_W_UI:  Item: DP(bight pixel) UI control
         - @b DP_D_UI:  Item: DP(dark pixel) UI control
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern pCAL_UICB_OBJ CAL_UICB(void);

#if 0
    - ITEM
#endif


typedef struct _CAL_STATUS_CHK
{
    UINT32 ISO_STATUS;
    UINT32 AWB_STATUS;
    UINT32 DP_STATUS;
    UINT32 ECS_STATUS;
    UINT32 AECHK_STATUS;
}CAL_STATUS_CHK;

typedef struct _CalIQInfo
{
    CAL_STATUS_CHK Cal_Status;
    UINT32 Cal_AWB_Rg;
    UINT32 Cal_AWB_Bg;
    UINT32 Cal_ISO_Base;
    UINT32 Cal_AE_ChkRst;    //Check Capture Luminance at LV 15
}CalIQInfo;

/**
    function prototype of Calibratoin Item
*/
typedef ER (*fpItem)(fpUI_cb,UINT32 DmaAddr,UINT32 DmaSize);

/**
    calibration Item structure
*/
typedef struct _CAL_ITEM_OBJ
{
    fpItem  fpISOAdj;           ///< -
    fpItem  fpAWBAdj;           ///< -
    fpItem  fpAWBGSAdj;         ///< -
    fpItem  fpMSHUTAdj;         ///< -
    fpItem  fpECSAdj;           ///< -
    fpItem  fpDP_Adj;           ///< -
    fpItem  fpDP_Adj_Bright;    ///< -
    fpItem  fpDP_Adj_Dark;      ///< -
    fpItem  fpAE_Check;         ///< -
    BOOL    (*fpGetAdjustRst)(fpUI_cb fpUIcb, CalIQInfo *CalData);
}CAL_ITEM_OBJ,*pCAL_ITEM_OBJ;

//@}
#endif
