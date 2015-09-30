#ifndef _MOVIESTAMP_H
#define _MOVIESTAMP_H

#include "draw_lib.h"

// bit [0]
#define STAMP_ON                0x0000
#define STAMP_OFF               0x0001
#define STAMP_SWITCH_MASK       0x0001

// bit [1]
#define STAMP_AUTO              0x0000
#define STAMP_MANUAL            0x0002
#define STAMP_OPERATION_MASK    0x0002

// bit [3:2]
#define STAMP_DATE_TIME         0x0000
#define STAMP_DATE_TIME_AMPM    0x0004
#define STAMP_DATE              0x0008
#define STAMP_TIME              0x000C
#define STAMP_DATE_TIME_MASK    0x000C

// bit [5:4]
#define STAMP_BOTTOM_LEFT       0x0000
#define STAMP_BOTTOM_RIGHT      0x0010
#define STAMP_TOP_LEFT          0x0020
#define STAMP_TOP_RIGHT         0x0030
#define STAMP_POSITION_MASK     0x0030

// bit [7:6]
#define STAMP_YY_MM_DD          0x0000
#define STAMP_DD_MM_YY          0x0040
#define STAMP_MM_DD_YY          0x0080
#define STAMP_DATE_FORMAT_MASK  0x00C0

// bit [8]
#define STAMP_BG_TRANSPARENT    0x0000  // default
#define STAMP_BG_FILL_COLOR     0x0100
#define STAMP_BG_FORMAT_MASK    0x0100

// bit [9]
#define STAMP_POS_NORMAL        0x0000  // default
#define STAMP_POS_END           0x0200
#define STAMP_POS_END_MASK      0x0200

#define STAMP_FORMAT_422            1   // YUV422
#define STAMP_FORMAT_420_UVPACK     2   // YUV420 UV pack

#define cst_prom0   21
#define cst_prom1   79
#define cst_prom2   29
#define cst_prom3   43
#define RGB_GET_Y(r,g,b)    (((INT32)g) + ((cst_prom1 * (((INT32)r)-((INT32)g))) >> 8) + ((cst_prom2 * (((INT32)b)-((INT32)g))) >> 8) )
#define RGB_GET_U(r,g,b)    (128 + ((cst_prom3 * (((INT32)g)-((INT32)r))) >> 8) + ((((INT32)b)-((INT32)g)) >> 1) )
#define RGB_GET_V(r,g,b)    (128 + ((cst_prom0 * (((INT32)g)-((INT32)b))) >> 8) + ((((INT32)r)-((INT32)g)) >> 1) )

typedef struct
{
    UINT32    uiX;              ///< the X offset
    UINT32    uiY;              ///< the Y offset
} STAMP_POS;

typedef struct
{
    UINT8     ucY;              ///< the Y value
    UINT8     ucU;              ///< the U value
    UINT8     ucV;              ///< the V value
    UINT8     rev;              ///< reserved
} STAMP_COLOR, *PSTAMP_COLOR;

typedef struct
{
    char    *pi8Str;            ///< The string of date.
    ICON_DB const *pDataBase;   ///< The font database for date.
    UINT32  ui32FontWidth;      ///< The max width of font
    UINT32  ui32FontHeight;     ///< The max height of font
    UINT32  ui32DstHeight;      ///< The destination height of font
    STAMP_COLOR Color[3];       ///< The stamp color (0: background, 1: frame, 2: foreground)
} STAMP_INFO, *PSTAMP_INFO;

typedef struct
{
    UINT32  uiYAddr;            ///< Stamp buffer Y address
    UINT32  uiUAddr;            ///< Stamp buffer U address
    UINT32  uiVAddr;            ///< Stamp buffer V address
    UINT32  uiYLineOffset;      ///< Stamp buffer Y line offset
    UINT32  uiUVLineOffset;     ///< Stamp buffer UV line offset
    UINT32  uiFormat;           ///< Stamp YUV format
} STAMP_BUFFER, *PSTAMP_BUFFER;

// Date stamp for movie
extern void     MovieStamp_Setup(UINT32 uiVidEncId, UINT32 uiFlag, UINT32 uiImageWidth, UINT32 uiImageHeight);
extern void     MovieStamp_SetPos(UINT32 uiVidEncId, UINT32 x, UINT32 y);
extern void     MovieStamp_SetColor(UINT32 uiVidEncId, PSTAMP_COLOR pStampColorBg, PSTAMP_COLOR pStampColorFr, PSTAMP_COLOR pStampColorFg);
extern void     MovieStamp_SetDataAddr(UINT32 uiVidEncId, UINT32 uiAddr);
//extern UINT32   MovieStamp_GetMaxFontHeight(void);
extern void     MovieStamp_CopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth);

#endif
