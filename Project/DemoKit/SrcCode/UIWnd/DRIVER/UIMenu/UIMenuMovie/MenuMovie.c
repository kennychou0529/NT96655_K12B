#include <stdio.h>
#include "UIFlow.h"

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// Movie Size Options
#if (UPDATE_CFG == UPDATE_CFG_YES)
TM_OPTION gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_ID_MAX] = {0};
#else
TMDEF_BEGIN_OPTIONS(MOVIE_SIZE)
    TMDEF_OPTION_TEXT(MOVIE_SIZE_1080FHD)
    TMDEF_OPTION_TEXT(MOVIE_SIZE_720P)
    TMDEF_OPTION_TEXT(MOVIE_SIZE_WVGA)
    TMDEF_OPTION_TEXT(MOVIE_SIZE_VGA)
//    TMDEF_OPTION_TEXT(MOVIE_SIZE_1440P)
TMDEF_END_OPTIONS()
#endif

// Cyclic recroding Options
TMDEF_BEGIN_OPTIONS(MOVIE_CYCLIC_REC)
   TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_OFF)
   //TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_1MIN)
   //TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_2MIN)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_3MIN)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_5MIN)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_10MIN)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_MOTION_DET)
    TMDEF_OPTION_TEXT(MOVIE_MOTION_DET_OFF)
    TMDEF_OPTION_TEXT(MOVIE_MOTION_DET_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_HDR)
    TMDEF_OPTION_TEXT(MOVIE_HDR_OFF)
    TMDEF_OPTION_TEXT(MOVIE_HDR_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_AUDIO)
    TMDEF_OPTION_TEXT(MOVIE_AUDIO_OFF)
    TMDEF_OPTION_TEXT(MOVIE_AUDIO_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_DATEIMPRINT)
    TMDEF_OPTION_TEXT(MOVIE_DATEIMPRINT_OFF)
    TMDEF_OPTION_TEXT(MOVIE_DATEIMPRINT_ON)
TMDEF_END_OPTIONS()

// GPS Options
TMDEF_BEGIN_OPTIONS(MOVIE_GPS)
    TMDEF_OPTION_TEXT(MOVIE_GPS_OFF)
    TMDEF_OPTION_TEXT(MOVIE_GPS_ON)
TMDEF_END_OPTIONS()


TMDEF_BEGIN_OPTIONS(AUTO_UPDATE_TIME)
	TMDEF_OPTION_TEXT(AUTO_UPDATE_TIME_OFF)
	TMDEF_OPTION_TEXT(GMTM12)	
	TMDEF_OPTION_TEXT(GMTM11)
	TMDEF_OPTION_TEXT(GMTM10)
	TMDEF_OPTION_TEXT(GMTM9)	
	TMDEF_OPTION_TEXT(GMTM8)
	TMDEF_OPTION_TEXT(GMTM7)
	TMDEF_OPTION_TEXT(GMTM6)
	TMDEF_OPTION_TEXT(GMTM5)	
	TMDEF_OPTION_TEXT(GMTM430)
	TMDEF_OPTION_TEXT(GMTM4)	
	TMDEF_OPTION_TEXT(GMTM330)	
	TMDEF_OPTION_TEXT(GMTM3)	
	TMDEF_OPTION_TEXT(GMTM2)	
	TMDEF_OPTION_TEXT(GMTM1)	
	TMDEF_OPTION_TEXT(GMT0)	
	TMDEF_OPTION_TEXT(GMT1)	
	TMDEF_OPTION_TEXT(GMT2)	
	TMDEF_OPTION_TEXT(GMT3)	
	TMDEF_OPTION_TEXT(GMT330)	
	TMDEF_OPTION_TEXT(GMT4)	
	TMDEF_OPTION_TEXT(GMT430)	
	TMDEF_OPTION_TEXT(GMT5)
	TMDEF_OPTION_TEXT(GMT530)
	TMDEF_OPTION_TEXT(GMT545)
	TMDEF_OPTION_TEXT(GMT6)	
	TMDEF_OPTION_TEXT(GMT630)	
	TMDEF_OPTION_TEXT(GMT7)
	TMDEF_OPTION_TEXT(GMT8)
	TMDEF_OPTION_TEXT(GMT9)
	TMDEF_OPTION_TEXT(GMT930)
	TMDEF_OPTION_TEXT(GMT10)
	TMDEF_OPTION_TEXT(GMT11)
	TMDEF_OPTION_TEXT(GMT12)	
	TMDEF_OPTION_TEXT(GMT13)	
       TMDEF_END_OPTIONS()

// Gsensor Options
TMDEF_BEGIN_OPTIONS(GSENSOR)
    TMDEF_OPTION_TEXT(MOVIE_GSENSOR_OFF)
    TMDEF_OPTION_TEXT(MOVIE_GSENSOR_L)
    TMDEF_OPTION_TEXT(MOVIE_GSENSOR_M)
    TMDEF_OPTION_TEXT(MOVIE_GSENSOR_H)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_PARKING)
    TMDEF_OPTION_TEXT(MOVIE_PARKING_OFF)
    TMDEF_OPTION_TEXT(MOVIE_PARKING_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_LDWS)
    TMDEF_OPTION_TEXT(MOVIE_LDWS_OFF)
    TMDEF_OPTION_TEXT(MOVIE_LDWS_ON)
TMDEF_END_OPTIONS()

// Car num
TMDEF_BEGIN_OPTIONS(CAR_NUM)
    TMDEF_OPTION_TEXT(CAR_NUM_OFF)
    TMDEF_OPTION_TEXT(CAR_NUM_ON)
TMDEF_END_OPTIONS()

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
// Movie Menu Items
    TMDEF_BEGIN_ITEMS(MOVIE)
    TMDEF_ITEM_TEXTID(MOVIE_SIZE)
    TMDEF_ITEM_TEXTID(MOVIE_CYCLIC_REC)
    TMDEF_ITEM_TEXTID(MOVIE_HDR)
    //TMDEF_ITEM_TEXTID(MOVIE_EV)
    TMDEF_ITEM_TEXTID(EV)
    TMDEF_ITEM_TEXTID(MOVIE_MOTION_DET)
    TMDEF_ITEM_TEXTID(MOVIE_AUDIO)
    TMDEF_ITEM_TEXTID(MOVIE_DATEIMPRINT)
    TMDEF_ITEM_TEXTID(CAR_NUM)  
    TMDEF_ITEM_TEXTID(GSENSOR)
    //TMDEF_ITEM_TEXTID(MOVIE_GPS)
    //TMDEF_ITEM_TEXTID(AUTO_UPDATE_TIME)
    //TMDEF_ITEM_TEXTID(MOVIE_LDWS)
    TMDEF_ITEM_TEXTID(MOVIE_PARKING) 
    TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Movie Menu Pages
TMDEF_BEGIN_PAGES(MOVIE)
    TMDEF_PAGE_TEXT_ICON(MOVIE)
    TMDEF_PAGE_TEXT_ICON(SETUP)
TMDEF_END_PAGES()

TMDEF_EMNU(gMovieMenu, MOVIE, Movie_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Movie_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
    UINT16  uwItemId;
    UINT16  uwOption;
    
    if (uiMessage == TMM_CONFIRM_OPTION)
    {
        uwItemId = LO_WORD(uiParam);
        uwOption = HI_WORD(uiParam);
        debug_msg("*********************************ERIC_DEBUG*********************\n\r");//ERIC_DEBUG
 debug_msg("%s,%s,%d\r\n",__FILE__,__func__,__LINE__);
 debug_msg("*********************************ERIC_DEBUG*********************\n\r");//ERIC_DEBUG	
        switch (uwItemId)
        {
        case IDM_MOVIE_SIZE:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIESIZE, 1, uwOption);
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_IMAGE_RATIO, 1, uwOption);
            break;

        case IDM_EV:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_EV, 1, uwOption);
            break;

        case IDM_MOVIE_HDR:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_HDR, 1, uwOption);
            break;

        case IDM_MOVIE_CYCLIC_REC:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_CYCLIC_REC, 1, uwOption);
            break;

        case IDM_MOVIE_MOTION_DET:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOTION_DET, 1, uwOption);
            break;

        case IDM_MOVIE_AUDIO:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_AUDIO, 1, uwOption);
            break;

        case IDM_MOVIE_DATEIMPRINT:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_DATE_IMPRINT, 1, uwOption);
            break;
	 case IDM_CAR_NUM:
	 	
	 	SysSetFlag(FL_CAR_NUM,uwOption);
	  	if(uwOption==1)
	  	{	
	            if ((SysGetFlag(FL_LANGUAGE) == LANG_SC)||(SysGetFlag(FL_LANGUAGE) == LANG_TC)) 
	            {
	            
	                Ux_OpenWindow(&UIMenuWndSetupCarNumberCtrl, 1, 0);
	            }
	            else
	            {
	             
	                Ux_OpenWindow(&UIMenuWndSetupCommonCarNumberCtrl, 1, 0);
	            }		
		}
	  	break;
        case IDM_GSENSOR:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_GSENSOR, 1, uwOption);
            break;
        case IDM_MOVIE_GPS:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIEGPS, 1, uwOption);
            break;
        case IDM_MOVIE_LDWS:
            UI_SetData(FL_MOVIE_LDWS, uwOption);
            break;
        case IDM_AUTO_UPDATE_TIME:
            
            Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_AUTO_UPDATE_TIME, 1, uwOption);
	    break;
	case IDM_MOVIE_PARKING:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIEPARKING, 1, uwOption);
            break;
        default:
            Setup_MenuCallback(uiMessage, uiParam);
            break;
        }
    }

    return TMF_PROCESSED;
}
