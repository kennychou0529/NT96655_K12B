
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "JpgEnc.h"
#include "Exif.h"
#include "Utility.h"
#include "audio.h"
#include "RTC.h"
//#include "PhotoTsk.h"

static CHAR    g_RecMaxTimeStr[20] = {0};
static UINT32  g_RecMaxTime=0;
static CHAR    g_RecCurrTimeStr[20] = {0};
static UINT32  g_RecCurrTime=0;
static CHAR    date_str[20] = {0};
static CHAR    time_str[20] = {0};

static CHAR    resolution_Buf[MOVIE_SIZE_ID_MAX][10]=
{
    "1080FHD",
    //"1080P",
    "720P",
    "WVGA",
    "VGA"
};

void FlowMovie_IconDrawDscMode(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideDscMode(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawMaxRecTime(VControl *pCtrl)
{
    memset((void *)g_RecMaxTimeStr, 0, sizeof(g_RecMaxTimeStr));
    g_RecMaxTime = UIMovRecObj_GetData(RECMOVIE_MAXSECOND);

    if (g_RecMaxTime <= 2)
    {
        FlowMovie_SetRecMaxTime(0);
    }
    else if (g_RecMaxTime > 86399)
    {   ///23:59:59
        FlowMovie_SetRecMaxTime(86399);
    }
    else
    {
        FlowMovie_SetRecMaxTime(g_RecMaxTime);
    }

    if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
    {
        sprintf(g_RecMaxTimeStr,"%02d:%02d:%02d", 0, 0, 0);
    }
    else
    {
        sprintf(g_RecMaxTimeStr,"%02d:%02d:%02d", g_RecMaxTime/3600,(g_RecMaxTime%3600)/60, (g_RecMaxTime%3600)%60);
    }

    UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(g_RecMaxTimeStr));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideMaxRecTime(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawRecTime(VControl *pCtrl)
{
    switch (gMovData.State)
    {
    case MOV_ST_REC:
    case MOV_ST_REC | MOV_ST_ZOOM:
        g_RecCurrTime = FlowMovie_GetRecCurrTime();
        memset((void *)g_RecCurrTimeStr, 0, sizeof(g_RecCurrTimeStr));
        sprintf(g_RecCurrTimeStr,"%02d:%02d:%02d", g_RecCurrTime/3600,(g_RecCurrTime%3600)/60, (g_RecCurrTime%3600)%60);
        UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(g_RecCurrTimeStr));
        UxCtrl_SetShow(pCtrl, TRUE);
        break;
    }
}

void FlowMovie_IconDrawDateTime(void)
{
 RTC_DATE    Date;
 RTC_TIME    Time;

    // display Date/Time string in movie mode
    Date = rtc_getDate();
    Time = rtc_getTime();
    //sprintf(date_str,"%04d/%02d/%02d",Date.s.year,Date.s.month,Date.s.day);
    sprintf(date_str,"%02d/%02d/%04d",Date.s.day,Date.s.month,Date.s.year);
    sprintf(time_str,"%02d:%02d:%02d",Time.s.hour,Time.s.minute,Time.s.second);
    UxStatic_SetData(&UIFlowWndMovie_YMD_StaticCtrl,STATIC_VALUE,Txt_Pointer(date_str));
    UxStatic_SetData(&UIFlowWndMovie_HMS_StaticCtrl,STATIC_VALUE,Txt_Pointer(time_str));
    UxCtrl_SetShow(&UIFlowWndMovie_YMD_StaticCtrl,TRUE);
    UxCtrl_SetShow(&UIFlowWndMovie_HMS_StaticCtrl,TRUE);
}

void FlowMovie_IconHideDateTime(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_YMD_StaticCtrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndMovie_HMS_StaticCtrl,FALSE);
}

void FlowMovie_IconDrawRec(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,TRUE);
}

void FlowMovie_IconHideRec(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}

void FlowMovie_IconDrawSize(VControl *pCtrl)
{
    UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(resolution_Buf[SysGetFlag(FL_MOVIE_SIZE)]));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideSize(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawEV(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_EV));
    UxCtrl_SetShow(pCtrl,TRUE);
}

void FlowMovie_IconHideEV(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}

void FlowMovie_IconDrawStorage(VControl *pCtrl)
{
    /* Update status item data */
    if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
    {
        UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Storage_ICON_INTERNAL_FLASH);
    } else if (UI_GetData(FL_CardStatus) == CARD_LOCKED) {
        UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Storage_ICON_SD_LOCK);
    } else {
        UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Storage_ICON_SD_CARD);
    }

    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideStorage(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawCyclicRec(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_CYCLIC_REC));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideCyclicRec(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawHDR(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_HDR));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideHDR(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}


void FlowMovie_IconDrawMotionDet(VControl *pCtrl)
{
    
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_MOTION_DET));
    UxCtrl_SetShow(pCtrl, TRUE);
     
}

void FlowMovie_IconHideMotionDet(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawTimeLapes(VControl *pCtrl)
{
  
	UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_TimeLapes_ICON_TIMELAPSE_REC_STATUS);
       UxCtrl_SetShow(pCtrl, TRUE);
}
void FlowMovie_IconHideTimeLapes(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}
void FlowMovie_IconDrawBattery(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,GetBatteryLevel());
    UxCtrl_SetShow(pCtrl,TRUE);
}

void FlowMovie_IconHideBattery(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}

void FlowMovie_IconDrawDZoom(VControl *pCtrl)
{
 BOOL   bShow;

    UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(Get_DZoomRatioString()));
    bShow = (DZOOM_IDX_GET() > DZOOM_IDX_MIN())? TRUE : FALSE;
    UxCtrl_SetShow(pCtrl,bShow);
}

void FlowMovie_IconHideDZoom(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}

void FlowMovie_DrawPIM(BOOL bDraw)
{
    UxCtrl_SetShow(&UIFlowWndMovie_StaticIcon_PIMCCtrl, bDraw);
}

void FlowMovie_IconDrawAudioOnOff(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_AUDIO));
    UxCtrl_SetShow(pCtrl, TRUE);
}

    
void FlowMovie_IconDrawLock(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Static_LockCtrl, TRUE);
}

void FlowMovie_IconHideLock(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Static_LockCtrl, FALSE);
}

void FlowMovie_IconDrawGsensor(void)
{

    if(SysGetFlag(FL_GSENSOR)==GSENSOR_OFF)
	    UxState_SetData(&UIFlowWndMovie_Status_GsensorCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Gsensor_ICON_GSENSOR_OFF);
    else  if(SysGetFlag(FL_GSENSOR)==GSENSOR_LOW)
    	    UxState_SetData(&UIFlowWndMovie_Status_GsensorCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Gsensor_ICON_GSENSOR_L);
    else  if(SysGetFlag(FL_GSENSOR)==GSENSOR_MED)
    	    UxState_SetData(&UIFlowWndMovie_Status_GsensorCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Gsensor_ICON_GSENSOR_M);
    else  if(SysGetFlag(FL_GSENSOR)==GSENSOR_HIGH)
    	    UxState_SetData(&UIFlowWndMovie_Status_GsensorCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Gsensor_ICON_GSENSOR_H);
    UxCtrl_SetShow(&UIFlowWndMovie_Status_GsensorCtrl, TRUE);
}

void FlowMovie_IconHideGsensor(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Status_GsensorCtrl, FALSE);
}
void FlowMovie_IconDrawPlateNumber(void)
{

	static char CarNoBuf[16] = {0};
	if (SysGetFlag(FL_CAR_NUM) == CAR_NUM_ON) {			
		if((SysGetFlag(FL_LANGUAGE) == LANG_SC) ||(SysGetFlag(FL_LANGUAGE) == LANG_TC))
		{ 
			strncpy(CarNoBuf, SysGetZHCarNoStamp(), 16);
			if (CarNoBuf[0]<1 || CarNoBuf[0]>32)
			{
				CarNoBuf[0] = 2;
				strcpy(CarNoBuf+1, SysGetNZHCarNoStamp());
				SysSetZHCarNoStamp(CarNoBuf);
			}
			UxState_SetData(&UIFlowWndMovie_Status_ZHCarNoCtrl,STATE_CURITEM,CarNoBuf[0]);
			UxCtrl_SetShow(&UIFlowWndMovie_Status_ZHCarNoCtrl,TRUE);

                    UxStatic_SetData(&UIFlowWndMovie_CarNo_StaticCtrl,STATIC_VALUE,Txt_Pointer(&CarNoBuf[1]));
                    UxCtrl_SetShow(&UIFlowWndMovie_CarNo_StaticCtrl,TRUE);
		}
		else
		{
              UxStatic_SetData(&UIFlowWndMovie_CarNo_StaticCtrl,STATIC_VALUE,Txt_Pointer(SysGetNZHCarNoStamp()));
    		UxCtrl_SetShow(&UIFlowWndMovie_CarNo_StaticCtrl,TRUE);
              UxCtrl_SetShow(&UIFlowWndMovie_Status_ZHCarNoCtrl,FALSE);
		}
	}
	else
	{
        UxCtrl_SetShow(&UIFlowWndMovie_Status_ZHCarNoCtrl,FALSE);
        UxCtrl_SetShow(&UIFlowWndMovie_CarNo_StaticCtrl,FALSE);
	}
	
}

void FlowMovie_IconHidePlateNumber(void)
{

    UxCtrl_SetShow(&UIFlowWndMovie_Status_ZHCarNoCtrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndMovie_CarNo_StaticCtrl,FALSE);
    
}
void FlowMovie_IconDrawParkingMonitor(void)
{
    UxState_SetData(&UIFlowWndMovie_Status_ParkingMonitorCtrl,STATE_CURITEM,UI_GetData(FL_MOVIE_PARKING));
    UxCtrl_SetShow(&UIFlowWndMovie_Status_ParkingMonitorCtrl, TRUE);
}

void FlowMovie_IconHideParkingMonitor(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Status_ParkingMonitorCtrl, FALSE);
}

void FlowMovie_UpdateIcons(BOOL bShow)
{

    if (bShow == FALSE || SysGetFlag(FL_LCD_DISPLAY) == DISPLAY_OFF)
    {
        FlowMovie_IconHideDscMode(&UIFlowWndMovie_Static_cameraCtrl);
        FlowMovie_IconHideSize(&UIFlowWndMovie_Static_resolutionCtrl);
        FlowMovie_IconHideMaxRecTime(&UIFlowWndMovie_Static_maxtimeCtrl);
        FlowMovie_IconHideRec(&UIFlowWndMovie_PanelCtrl);
        FlowMovie_IconHideStorage(&UIFlowWndMovie_Status_StorageCtrl);
        FlowMovie_IconHideCyclicRec(&UIFlowWndMovie_Status_CyclicRecCtrl);
        FlowMovie_IconHideMotionDet(&UIFlowWndMovie_Status_MotionDetCtrl);
        FlowMovie_IconHideDZoom(&UIFlowWndMovie_Zoom_StaticCtrl);
        FlowMovie_IconHideBattery(&UIFlowWndMovie_Status_batteryCtrl);
        FlowMovie_IconHideEV(&UIFlowWndMovie_StatusICN_EVCtrl);
        FlowMovie_IconHideDateTime();
        FlowMovie_DrawPIM(FALSE);
        FlowMovie_IconDrawAudioOnOff(&UIFlowWndMovie_StatusICN_AudioCtrl);
	 FlowMovie_IconHideParkingMonitor();	
	 FlowMovie_IconHideLock();  //eric 文件加锁
	 FlowMovie_IconHideTimeLapes(&UIFlowWndMovie_Status_TimeLapesCtrl);	
		
    }
    else
    {
        FlowMovie_IconDrawDscMode(&UIFlowWndMovie_Static_cameraCtrl);
        FlowMovie_IconDrawSize(&UIFlowWndMovie_Static_resolutionCtrl);
        FlowMovie_IconDrawMaxRecTime(&UIFlowWndMovie_Static_maxtimeCtrl);
        FlowMovie_IconHideRec(&UIFlowWndMovie_PanelCtrl);
        FlowMovie_IconDrawStorage(&UIFlowWndMovie_Status_StorageCtrl);
        FlowMovie_IconDrawCyclicRec(&UIFlowWndMovie_Status_CyclicRecCtrl);
        FlowMovie_IconDrawHDR(&UIFlowWndMovie_Status_HDRCtrl);
        FlowMovie_IconDrawMotionDet(&UIFlowWndMovie_Status_MotionDetCtrl);
        FlowMovie_IconDrawDZoom(&UIFlowWndMovie_Zoom_StaticCtrl);
        FlowMovie_IconDrawBattery(&UIFlowWndMovie_Status_batteryCtrl);
        FlowMovie_IconDrawEV(&UIFlowWndMovie_StatusICN_EVCtrl);
        FlowMovie_IconDrawAudioOnOff(&UIFlowWndMovie_StatusICN_AudioCtrl);
        FlowMovie_IconDrawDateTime();
        FlowMovie_DrawPIM(FALSE);
        FlowMovie_IconDrawGsensor();
        //FlowMovie_IconHideLock();
         if(UIFlowWndMovie_IsGsensorTrig()==TRUE) ////eric 文件加锁
	  FlowMovie_IconDrawLock();			
	  else	
	  FlowMovie_IconHideLock();//eric 文件加锁
	 FlowMovie_IconDrawParkingMonitor();	
	 if(UI_GetData(FL_MOVIE_TIMELAPSE_REC)!=MOVIE_TIMELAPSEREC_OFF)
	 {
          FlowMovie_IconDrawTimeLapes(&UIFlowWndMovie_Status_TimeLapesCtrl);
	  }
	 else
	  FlowMovie_IconHideTimeLapes(&UIFlowWndMovie_Status_TimeLapesCtrl);	
	 
	 if(SysGetFlag(FL_CAR_NUM)==CAR_NUM_ON)
            FlowMovie_IconDrawPlateNumber();
        else
            FlowMovie_IconHidePlateNumber();
    }
}

