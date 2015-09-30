#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "SysCfg.h"
#include "Utility.h"
#include "HwClock.h"
#include "GxImage.h"
#include "MovieStamp.h"
#include "DateStampFont10x16.h"
#include "DateStampFont12x20.h"
#include "DateStampFont18x30.h"
#include "DateStampFont20x44.h"
#include "DateStampFont26x44.h"
#include "IQS_Utility.h"
#include "UIInfo.h"

#define __MODULE__          MovieStamp
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define CHECK_STAMP_TIME    DISABLE
#define CHECK_ENC_INFO      DISABLE

#define COLOR_BG_Y          0x00
#define COLOR_BG_U          0x80
#define COLOR_BG_V          0x80
#define COLOR_FR_Y          0x00
#define COLOR_FR_U          0x80
#define COLOR_FR_V          0x80
#define COLOR_FG_Y          RGB_GET_Y(250, 160, 10)
#define COLOR_FG_U          RGB_GET_U(250, 160, 10)
#define COLOR_FG_V          RGB_GET_V(250, 160, 10)
#define COLOR_ID_BG         0
#define COLOR_ID_FR         1
#define COLOR_ID_FG         2
#define COLORKEY_BG_YUV     0x00000000  // stamp BG data for color key operation
#define COLORKEY_YUV        0x00010101  // color key for transparent background

#define STAMP_WIDTH_TOLERANCE   8   // total font width error tolerance
#define VIDEO_IN_MAX        2   // max 2 video paths


//variable declare
static STAMP_POS    g_MovieStampPos[VIDEO_IN_MAX] =
{
    {0, 0}, // stamp position for primary image 1
    {0, 0}  // stamp position for primary image 2
};
static char         g_cMovieStampStr[VIDEO_IN_MAX][64];
static UINT32       g_uiMovieStampSetup[VIDEO_IN_MAX] = {STAMP_OFF, STAMP_OFF};
static STAMP_INFO   g_MovieStampInfo[VIDEO_IN_MAX];
static struct tm    g_CurDateTime[VIDEO_IN_MAX];
static UINT32       g_uiMovieStampYAddr[VIDEO_IN_MAX] = {0, 0};
static UINT32       g_uiMovieStampFormat = STAMP_FORMAT_420_UVPACK;
//static UINT32       g_uiMovieStampHeightMax = 44; // maximum height of date stamp font (gDateStampFont26x44)
static BOOL         g_bStampColorSetup[VIDEO_IN_MAX] = {FALSE, FALSE};
#define CAR_NUMBER_STAMP_LEN	32
char MOVIE_STAMP_STRING[10]="abcd";//ÓÅµ¼ÊË
static char carnumber_stamp_str[CAR_NUMBER_STAMP_LEN] = {0};

void MovieStamp_Setup(UINT32 uiVidEncId, UINT32 uiFlag, UINT32 uiImageWidth, UINT32 uiImageHeight)
{
    PSTAMP_INFO     pStampInfo;
    ICON_DB const   *pDB;
    UINT32          uiIconID;
    UINT32          uiStrOffset;

    g_uiMovieStampSetup[uiVidEncId] = uiFlag;

    if ((uiFlag & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }
	#if 1
	memset(carnumber_stamp_str, 0, CAR_NUMBER_STAMP_LEN);
	if (SysGetFlag(FL_CAR_NUM) == CAR_NUM_ON)
	{	  	
	        if((SysGetFlag(FL_LANGUAGE) == LANG_SC) ||(SysGetFlag(FL_LANGUAGE) == LANG_TC))			
		{
			strcpy(carnumber_stamp_str, SysGetZHCarNoStamp());
		}
		else
		{
			strcpy(carnumber_stamp_str, SysGetNZHCarNoStamp());
		}
	}
	else
	{
		sprintf(carnumber_stamp_str," ");
	}
	#endif

    pStampInfo = &g_MovieStampInfo[uiVidEncId];
    pStampInfo->pi8Str = &g_cMovieStampStr[uiVidEncId][0];

    // set date stamp font data base
    switch (uiImageWidth)
    {
    case 1920:  // 1920x1080
    case 1536:  // 1536x1536
        //pStampInfo->pDataBase = &gDateStampFont26x44;
		pStampInfo->pDataBase = &gDateStampFont18x30;
        break;

    case 1728:  // 1728x1296 (DAR 16:9)
    case 1440:  // 1440x1080 (DAR 16:9)
        //pStampInfo->pDataBase = &gDateStampFont20x44;
		pStampInfo->pDataBase = &gDateStampFont18x30;
        break;

    case 1280:  // 1280x720
        pStampInfo->pDataBase = &gDateStampFont18x30;
        break;

    case 320:   // QVGA
        pStampInfo->pDataBase = &gDateStampFont10x16;
        break;

    default:    // VGA & others
        pStampInfo->pDataBase = &gDateStampFont12x20;
        break;
    }

    // set stamp string (for calculating stamp position)
    switch (uiFlag & STAMP_DATE_TIME_MASK)
    {
    case STAMP_DATE_TIME_AMPM:
        sprintf(pStampInfo->pi8Str, "0000/00/00 00:00:00 AM");
        break;

    case STAMP_DATE: // date only is not suitable for movie stamp (it's suitable for still image stamp)
    case STAMP_TIME:
        sprintf(pStampInfo->pi8Str, "00:00:00");
        break;

    case STAMP_DATE_TIME:
    default:
        //sprintf(pStampInfo->pi8Str, "0000/00/00 00:00:00");
	#if 0
	  sprintf(pStampInfo->pi8Str, "%s 0000/00/00 00:00:00",MOVIE_STAMP_STRING);
	#else
	  sprintf(pStampInfo->pi8Str, "%s AAAAA  AAAAAAA  0000/00/00 00:00:00",MOVIE_STAMP_STRING);
	  //sprintf(pStampInfo->pi8Str, "%s A AAAA  0000/00/00 00:00:00",MOVIE_STAMP_STRING);
	  //sprintf(pStampInfo->pi8Str, "ABCD AAAA  0000/00/00 00:00:00");
	#endif
	  
        break;
    }

    // set font width and height (use the width of 1st font, so the total width may have some error)
    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;
    uiIconID = pStampInfo->pi8Str[0] - uiStrOffset; // 1st font
    pStampInfo->ui32FontWidth  = pDB->pIconHeader[uiIconID].uiWidth;
    pStampInfo->ui32FontHeight = pDB->pIconHeader[uiIconID].uiHeight;
    pStampInfo->ui32DstHeight  = pStampInfo->ui32FontHeight; // no scaling

    // Set date stamp position
    if ((uiFlag & STAMP_OPERATION_MASK) == STAMP_AUTO)
    {
        //UINT32  uiImageWidth = GetMovieSizeWidth(uiMovieIndex);
        //UINT32  uiImageHeight = GetMovieSizeHeight(uiMovieIndex);
        UINT32  uiStampWidth = (pStampInfo->ui32DstHeight * pStampInfo->ui32FontWidth) / pStampInfo->ui32FontHeight;

        switch (uiFlag & STAMP_POSITION_MASK)
        {
        case STAMP_TOP_LEFT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = 0;
                g_MovieStampPos[uiVidEncId].uiY = 0;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth * 2; // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2; // 1/2 font height gap
                #endif
            }
            break;

        case STAMP_TOP_RIGHT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * strlen(pStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
                g_MovieStampPos[uiVidEncId].uiY = 0;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 2); // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 1); // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2; // 1/2 font height gap
                #endif
            }
            break;

        case STAMP_BOTTOM_LEFT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = 0;
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth * 2; // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight * 2; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
                #endif
            }
            break;

        case STAMP_BOTTOM_RIGHT:
        default:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * strlen(pStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight;
            }
            else
            {
                #if 1
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 2); // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight * 2; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 1); // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
                #endif
            }
            break;
        }
    }

    g_MovieStampPos[uiVidEncId].uiX = ALIGN_FLOOR_4(g_MovieStampPos[uiVidEncId].uiX);
    g_MovieStampPos[uiVidEncId].uiY = ALIGN_FLOOR_4(g_MovieStampPos[uiVidEncId].uiY);

    // set default stamp color if necessary
    if (g_bStampColorSetup[uiVidEncId] == FALSE)
    {
        // Stamp background color
        pStampInfo->Color[COLOR_ID_BG].ucY = COLOR_BG_Y;
        pStampInfo->Color[COLOR_ID_BG].ucU = COLOR_BG_U;
        pStampInfo->Color[COLOR_ID_BG].ucV = COLOR_BG_V;

        // Stamp frame color
        pStampInfo->Color[COLOR_ID_FR].ucY = COLOR_FR_Y;
        pStampInfo->Color[COLOR_ID_FR].ucU = COLOR_FR_U;
        pStampInfo->Color[COLOR_ID_FR].ucV = COLOR_FR_V;

        // Stamp foreground color (text body)
        pStampInfo->Color[COLOR_ID_FG].ucY = COLOR_FG_Y;
        pStampInfo->Color[COLOR_ID_FG].ucU = COLOR_FG_U;
        pStampInfo->Color[COLOR_ID_FG].ucV = COLOR_FG_V;
    }

    // Reset reference time
    g_CurDateTime[uiVidEncId].tm_sec = 61;
}

void MovieStamp_SetPos(UINT32 uiVidEncId, UINT32 x, UINT32 y)
{
    g_MovieStampPos[uiVidEncId].uiX = x;
    g_MovieStampPos[uiVidEncId].uiY = y;
}

void MovieStamp_SetColor(UINT32 uiVidEncId, PSTAMP_COLOR pStampColorBg, PSTAMP_COLOR pStampColorFr, PSTAMP_COLOR pStampColorFg)
{
    g_bStampColorSetup[uiVidEncId] = TRUE;

    // Stamp background color
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucY = pStampColorBg->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucU = pStampColorBg->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucV = pStampColorBg->ucV;

    // Stamp frame color
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucY = pStampColorFr->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucU = pStampColorFr->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucV = pStampColorFr->ucV;

    // Stamp foreground color (text body)
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucY = pStampColorFg->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucU = pStampColorFg->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucV = pStampColorFg->ucV;
}

#if 0
UINT32 MovieStamp_GetMaxFontHeight(void)
{
    return g_uiMovieStampHeightMax;
}
#endif

static BOOL MovieStamp_DrawIcon420UV(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    ICON_HEADER pIconHeader;
    ICON_DB const *pDB;
    UINT8   const *uiIconData;
    UINT8   *pChar, *pCharU, *pCharV;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    pDB = pStampInfo->pDataBase;
    pIconHeader = pDB->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

    uiPixelCount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCb+1;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1) // frame color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
                }
                else // foreground color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
                }

                if (((j&1)==0)&&((i&1)==0))
                {
                    if (uiIndex == 1) // frame color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
                    }
                    else // foreground color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
                    }
                }
            }
            pChar++;
            if ((j&1)==0)
            {
                pCharU+=2;
                pCharV+=2;
            }
            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        if  ((i&1) == 0)
        {
            uiFBCb += uiLineOffset;
            uiFBCr += uiLineOffset;
        }
    }
    return TRUE;
}

static UINT32 UnicodeAsiicVal[35] = 
{
	0x0,
    0x100,      /*ÔÆ*/
    0x101,      /* ¾©  */
    0x106,      /*Äþ  */
    0x107,      /* ´¨*/
    0x10b,      /* »¦ */
    0x10c ,      /*"½ò"  */
     0x10d,      /*Õã*/	
    0x10e,      /*"Óå  */
    0x110,      /* Ïæ*/
    0x115,      /* ÔÁ*/
    0x119,      /* Ô¥*/
    0x11a,      /* ¹ó*/
    0x123,      /* Ç­*/
    0x11c,      /* ÁÉ*/
    0x121,      /* Â³*/
    0x122,      /* ºÚ*/
    0x108,       /* ÐÂ */
    0x116,      /* ËÕ*/  
    0x118,      /* ²Ø*/
    0x11e,      /* Ãö*/
    0x109,      /* ½ú*/
    0x112,      /* Çí*/
    0x102,      /* ¼½*/
    0x105,      /* ¼ª*/
    0x10a,      /* ¹ð*/
    0x113,      /* ¸Ê*/
    0x114,      /* Íî*/
    0x117,      /* ÃÉ  */  
    0x11d,      /* ¶õ*/  
    0x11f,      /* ÉÂ*/
    0x120,      /* Çà*/	
   
};//xin
UINT16 CheackCarNumICONID(INT8 pi8Str)
{
	UINT16  ID;
	
			ID = UnicodeAsiicVal[pi8Str];
		return ID;
}
static UINT32 MovieStamp_GetStampDataWidth(PSTAMP_INFO pStampInfo)
{
    ICON_DB const *pDB;
    UINT32  i, j;
    UINT32  uiStrLen;
    UINT32  uiDataWidth;
    UINT32  uiIconID;
    UINT32  uiStrOffset;

    uiStrLen = strlen(pStampInfo->pi8Str);

    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;

    uiDataWidth = 0;
    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
		#if 1
         if(pStampInfo->pi8Str[i] <= 0x1F)
		uiIconID = CheackCarNumICONID(pStampInfo->pi8Str[i]);
		else
		#endif
        uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
        j = pDB->pIconHeader[uiIconID].uiWidth;
        uiDataWidth += j;

        if (j % 2 != 0)
        {
            j++;
        }
    }

    return uiDataWidth;
}

static UINT32 MovieStamp_DrawString(UINT32 uiVidEncId, PSTAMP_INFO pStampInfo, PSTAMP_BUFFER pStampBuffer)
{
    ICON_DB const *pDB;
    UINT32  i, j;
    UINT32  uiStrLen;
    UINT32  uiDataWidth;
    UINT32  uiIconID;
    UINT32  uiStrOffset;
    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;

    uiStrLen = strlen(pStampInfo->pi8Str);

    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;

    uiFBAddrY  = pStampBuffer->uiYAddr;
    uiFBAddrCb = pStampBuffer->uiUAddr;
    uiFBAddrCr = pStampBuffer->uiVAddr;

    uiDataWidth = 0;
    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
		#if 1
        if(pStampInfo->pi8Str[i] <= 0x1F)
		uiIconID = CheackCarNumICONID(pStampInfo->pi8Str[i]);
		else
		#endif
        uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
        j = pDB->pIconHeader[uiIconID].uiWidth;
        uiDataWidth += j;

        if (j % 2 != 0)
        {
            j++;
        }

        MovieStamp_DrawIcon420UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
        uiFBAddrY  += (j);
        uiFBAddrCb += (j);
        uiFBAddrCr += (j);
    }

    return uiDataWidth;
}

void MovieStamp_SetDataAddr(UINT32 uiVidEncId, UINT32 uiAddr)
{
    g_uiMovieStampYAddr[uiVidEncId] = uiAddr;
}

#if 0 // Don't allow user to set YUV format temporary
void MovieStamp_SetDataType(UINT32 uiType)
{
    g_uiMovieStampFormat = uiType;
}
#endif

#if (CHECK_ENC_INFO == ENABLE)
extern void MediaRec_GetEncInfo(UINT32 puiParam[7]);
#endif
void MovieStamp_CopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
    STAMP_BUFFER    StampBuffer;
    struct tm       CurDateTime;
    IMG_BUF         SrcImg, DstImg;
    UINT32          x, y;
    UINT32          uiYAddrOffset, uiUAddrOffset;
    static UINT32   uiStampDataWidth[VIDEO_IN_MAX] = {0, 0};
    UINT32          uiPreWidth, uiWidth;
    UINT32          uiStampDataHeight;
    UINT32          uiBgData;
    UINT32          uiLineOffset[2] = {0};
    UINT32          uiPxlAddr[2] = {0};
    #if (CHECK_STAMP_TIME == ENABLE)
    UINT32          uiTime; // for performance measurement
    #endif

    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

    if (g_uiMovieStampFormat != STAMP_FORMAT_420_UVPACK)
    {
        DBG_ERR("Only support 420 UV pack!\r\n");
        return;
    }

    x = g_MovieStampPos[uiVidEncId].uiX;
    y = g_MovieStampPos[uiVidEncId].uiY;

    CurDateTime = HwClock_GetCurrentTime();

    // Y address offset of destination image to put Y stamp data
    uiYAddrOffset = lineY * y + x;
    // UV address offset of destination image to put UV stamp data
    uiUAddrOffset = lineY * y / 2 + x;

    uiStampDataHeight = g_MovieStampInfo[uiVidEncId].ui32DstHeight;

    StampBuffer.uiYLineOffset   = imgWidth;
    StampBuffer.uiUVLineOffset  = imgWidth;
    StampBuffer.uiYAddr         = g_uiMovieStampYAddr[uiVidEncId];
    StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
    StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
    StampBuffer.uiFormat        = g_uiMovieStampFormat;

    #if (CHECK_STAMP_TIME == ENABLE)
    Perf_Open(); // for performance measurement
    #endif

    #if (CHECK_ENC_INFO == DISABLE)
    if (g_CurDateTime[uiVidEncId].tm_sec != CurDateTime.tm_sec)
    #endif
    {
        g_CurDateTime[uiVidEncId].tm_sec = CurDateTime.tm_sec;

        // Prepare date-time string
        if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME)
        {
            #if 0
                if (SysGetFlag(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON)
                {
                    switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_FORMAT_MASK)
                    {
                    case STAMP_DD_MM_YY:
                        sprintf(&g_cMovieStampStr[uiVidEncId][0], "%s %s %02d/%02d/%04d %02d:%02d:%02d", MOVIE_STAMP_STRING,carnumber_stamp_str,CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    case STAMP_MM_DD_YY:
                        sprintf(&g_cMovieStampStr[uiVidEncId][0], "%s %s %02d/%02d/%04d %02d:%02d:%02d", MOVIE_STAMP_STRING,carnumber_stamp_str,CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    default:
                        sprintf(&g_cMovieStampStr[uiVidEncId][0], "%s %s %04d/%02d/%02d %02d:%02d:%02d", MOVIE_STAMP_STRING,carnumber_stamp_str,CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    }
                }
                else
                {
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%s", carnumber_stamp_str);
                }
            #else
                switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_FORMAT_MASK)
                {
                case STAMP_DD_MM_YY:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%s %s %02d/%02d/%04d %02d:%02d:%02d", MOVIE_STAMP_STRING,carnumber_stamp_str,CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                case STAMP_MM_DD_YY:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%s %s %02d/%02d/%04d %02d:%02d:%02d", MOVIE_STAMP_STRING,carnumber_stamp_str,CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                default:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%s %s %04d/%02d/%02d %02d:%02d:%02d", MOVIE_STAMP_STRING,carnumber_stamp_str,CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                }
            #endif 
        }
        else if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME_AMPM)
        {
            if (CurDateTime.tm_hour >= 12)
            {
                if (CurDateTime.tm_hour > 12)
                    CurDateTime.tm_hour -= 12;

                switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_FORMAT_MASK)
                {
                case STAMP_DD_MM_YY:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                case STAMP_MM_DD_YY:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                default:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%04d/%02d/%02d %02d:%02d:%02d PM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                }
            }
            else
            {
                switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_FORMAT_MASK)
                {
                case STAMP_DD_MM_YY:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                case STAMP_MM_DD_YY:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                default:
                    sprintf(&g_cMovieStampStr[uiVidEncId][0], "%04d/%02d/%02d %02d:%02d:%02d AM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                }
            }
        }
        else
        {
            sprintf(&g_cMovieStampStr[uiVidEncId][0], "%02d:%02d:%02d", CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
        }

        ////<< spark add AE function 
        //IQS_GetLogString(&g_cMovieStampStr[uiVidEncId][0], 32); 

        #if (CHECK_ENC_INFO == ENABLE) 
        {
            UINT32 uiParam[7];

            MediaRec_GetEncInfo(uiParam);
            sprintf(&g_cMovieStampStr[uiVidEncId][0], "%d %d %d %d / %d %d %d", 
                uiParam[0], uiParam[1], uiParam[2], uiParam[3], uiParam[4], uiParam[5], uiParam[6]);
        }
        #endif

        uiPreWidth = uiStampDataWidth[uiVidEncId]; // previous stamp data width
        uiStampDataWidth[uiVidEncId] = MovieStamp_GetStampDataWidth(&g_MovieStampInfo[uiVidEncId]);
        uiWidth = (uiPreWidth > uiStampDataWidth[uiVidEncId]) ? uiPreWidth : uiStampDataWidth[uiVidEncId];

        // Set background data
        #if (CHECK_STAMP_TIME == ENABLE)
        uiTime = Perf_GetCurrent();
        #endif
        // Clear source (stamp) buffer
        uiLineOffset[0] = StampBuffer.uiYLineOffset;
        uiLineOffset[1] = StampBuffer.uiUVLineOffset;
        uiPxlAddr[0]    = StampBuffer.uiYAddr;
        uiPxlAddr[1]    = StampBuffer.uiUAddr;

        GxImg_InitBufEx(&SrcImg,
                        uiWidth,
                        uiStampDataHeight,
                        GX_IMAGE_PIXEL_FMT_YUV420_PACKED,
                        uiLineOffset,
                        uiPxlAddr);

        if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        {
            //uiBgData = COLORKEY_BG_YUV; // for LE op
            uiBgData = COLORKEY_YUV; // for EQ op
        }
        else
        {
            uiBgData =  ((UINT32)g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucV << 16) |
                        ((UINT32)g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucU << 8)  |
                        g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucY;
        }

        GxImg_FillData(&SrcImg, REGION_MATCH_IMG, uiBgData);
        #if (CHECK_STAMP_TIME == ENABLE)
        DBG_DUMP("time1: %d us\r\n", Perf_GetCurrent() - uiTime);
        #endif

        // Draw string by getting data from date stamp font
        #if (CHECK_STAMP_TIME == ENABLE)
        uiTime = Perf_GetCurrent();
        #endif
        MovieStamp_DrawString(uiVidEncId, &g_MovieStampInfo[uiVidEncId], &StampBuffer);
        #if (CHECK_STAMP_TIME == ENABLE)
        DBG_DUMP("time2: %d us\r\n", Perf_GetCurrent() - uiTime);
        #endif
    }

    // copy data from date stamp draw buffer to image buffer
    #if (CHECK_STAMP_TIME == ENABLE)
    uiTime = Perf_GetCurrent();
    #endif
    // Init source (stamp) buffer
    uiLineOffset[0] = StampBuffer.uiYLineOffset;
    uiLineOffset[1] = StampBuffer.uiUVLineOffset;
    uiPxlAddr[0]    = StampBuffer.uiYAddr;
    uiPxlAddr[1]    = StampBuffer.uiUAddr;

    GxImg_InitBufEx(&SrcImg,
                    uiStampDataWidth[uiVidEncId],
                    uiStampDataHeight,
                    GX_IMAGE_PIXEL_FMT_YUV420_PACKED,
                    uiLineOffset,
                    uiPxlAddr);

    // Init destination (image) buffer
    uiLineOffset[0] = lineY;
    uiLineOffset[1] = lineY;
    uiPxlAddr[0]    = yAddr  + uiYAddrOffset;
    uiPxlAddr[1]    = cbAddr + uiUAddrOffset;

    GxImg_InitBufEx(&DstImg,
                    imgWidth,
                    240, // don't care, but should be > 2
                    GX_IMAGE_PIXEL_FMT_YUV420_PACKED,
                    uiLineOffset,
                    uiPxlAddr);

    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
    {
        GxImg_CopyColorkeyData(&DstImg, REGION_MATCH_IMG, &SrcImg, REGION_MATCH_IMG, COLORKEY_YUV, FALSE);
    }
    else
    {
        GxImg_CopyData(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
    }
    #if (CHECK_STAMP_TIME == ENABLE)
    DBG_DUMP("time3: %d us\r\n", Perf_GetCurrent() - uiTime);
    //Perf_Close();
    #endif
}
