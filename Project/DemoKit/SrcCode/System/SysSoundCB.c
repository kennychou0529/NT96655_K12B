/*
    System Sound Callback

    System Callback for Sound Module.

    @file       SysSoundCB.c
    @ingroup    mIPRJSYS

    @note       這個檔案負責一件事

                1.接收Sound callback的介面
                  Sound_CB()
                  可能的event有:
                     SYSTEM_CB_CONFIG
                     SOUND_CB_START
                       會呼叫GxFlash_SuspendCharge();
                     SOUND_CB_STOP
                       會呼叫GxFlash_ResumeCharge();
                     SOUND_CB_OUTDEVCHG
                       會呼叫GxSound_SetOutput()


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

//#NT#2010/01/14#Lincy -begin
// Need to suspend charge when sound play
#define  _SOUND_CB_CHARGE_                  0
//#NT#2010/01/14#Lincy -end

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#if 0//_SOUND_CB_CHARGE_
#include "GxFlash.h"
#endif
#include "GxSound.h"
#include "Debug.h"
#include "GxSound.h"



//---------------------SysSoundCB Debug Definition -----------------------------
#define _SOUND_ERROR_MSG        1
#define _SOUND_TRACE_MSG        0

#if (_SOUND_ERROR_MSG && (PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define SoundErrMsg(...)            debug_msg ("^R SOUND: "__VA_ARGS__)
#else
#define SoundErrMsg(...)
#endif

#if (_SOUND_TRACE_MSG && (PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define SoundTraceMsg(...)          debug_msg ("^B SOUND: "__VA_ARGS__)
#else
#define SoundTraceMsg(...)
#endif

//---------------------SysSoundCB Global Variables -----------------------------
//---------------------SysSoundCB Prototype Declaration  -----------------------
//---------------------SysSoundCB Public API  ----------------------------------
//---------------------SysSoundCB Private API ----------------------------------

//#NT#2010/07/14#Jeah Yen -begin
int SX_TIMER_DET_AUDIODEV_ID = -1;
//#NT#2010/07/14#Jeah Yen -end

//#NT#2010/07/14#Jeah Yen -begin
extern BOOL g_bIsInitSystemFinish;
//#NT#2010/07/14#Jeah Yen -end

//#NT#2012/05/07#Lily Kao -begin
//#NT# update task-id and flag-id
#if 0
//#NT#2011/1/21#Jeah Yen -begin
SOUND_CFG sound_cfg = {0};
//#NT#2011/1/21#Jeah Yen -end
#endif
//#NT#2012/05/07#Lily Kao -end


//#NT#2011/02/10#Lily Kao -begin
UINT32 guiSndRepeat = FALSE;

void Sound_SetRepeat(UINT32 isTrue)
{
    guiSndRepeat = isTrue;
}
//#NT#2011/02/10#Lily Kao -end

//#NT#2011/02/18#Lily Kao -begin
UINT32 Sound_GetRepeat(void)
{
    return guiSndRepeat;
}
//#NT#2011/02/18#Lily Kao -end

void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
    //#NT#2012/11/07#Lily Kao -begin
    //#NT#
    //AUDIO_OUTPUT audOutDev = param1;
    UINT32 audOutDev = param1;
    //#NT#2012/11/07#Lily Kao -end

    SoundTraceMsg("++Sound_CB:event=%d,p1=%d,p2=%d\r\n",event,param1,param2);
    switch(event)
    {
    //#NT#2010/07/14#Jeah Yen -begin
    case SYSTEM_CB_CONFIG:
        //#NT#2012/05/07#Lily Kao -begin
        //#NT# update task-id and flag-id
        #if 0
        //1.設定init值
        //#NT#2011/1/21#Jeah Yen -begin
        sound_cfg.uiTskID = PLAYSOUNDTSK_ID;
        sound_cfg.uiFlagID = FLG_ID_SOUND;
        GxSound_SetConfig(&sound_cfg);
        //#NT#2011/1/21#Jeah Yen -end
        #endif
        //#NT#2012/05/07#Lily Kao -end
        //2.設定CB值,
        //3.註冊SxJob服務 ---------> System Job
        //4.註冊SxTimer服務 ---------> Detect Job
        //SX_TIMER_DET_AUDIODEV_ID = System_AddSxTimer(GxSoud_DetSetAudioOutDevice, 25, "GxSoud_DetAudioOutDevice");
        //5.註冊SxCmd服務 ---------> Cmd Function
        //System_AddSxCmd(Sound_OnCommand); //GxSound
        break;
    //#NT#2010/07/14#Jeah Yen -end
    case SOUND_CB_START:
        #if _SOUND_CB_CHARGE_
        SoundTraceMsg("Sound start\r\n");
        //#NT#2010/10/08#Lincy Lin -begin
        //#NT#This will cause suspend , resume not match
        //#NT#2010/07/14#Jeah Yen -begin
        //if(g_bIsInitSystemFinish) //avoid call GxFlash when playing power-on sound
        {
        //#NT#2010/07/14#Jeah Yen -end
           // GxFlash_SuspendCharge();
        }
        //#NT#2010/10/08#Lincy Lin -end
        #else
        SoundTraceMsg("Sound_CB:event=%d Not suspend charge\r\n",event);
        #endif
        break;
    case SOUND_CB_STOP:
        #if _SOUND_CB_CHARGE_
        SoundTraceMsg("Sound stop\r\n");
        //#NT#2010/10/08#Lincy Lin -begin
        //#NT#This will cause suspend , resume not match
        //#NT#2010/07/14#Jeah Yen -begin
        //if(g_bIsInitSystemFinish) //avoid call GxFlash when playing power-on sound
        {
        //#NT#2010/07/14#Jeah Yen -end
           // GxFlash_ResumeCharge();
        }
        //#NT#2010/10/08#Lincy Lin -end
        #else
        SoundTraceMsg("Sound_CB:event=%d Not resume charge\r\n",event);
        #endif

#if SLIDESHOW_MUSIC_OPTION
        //#NT#2011/02/10#Lily Kao -begin
        if (guiSndRepeat)
        {
            //#NT#2011/02/18#Lily Kao -begin
            FlowPlaySlideShow_PlaySpecificBGMusic(UI_SLIDEMUSIC_MENU_SETTING);
            //#NT#2011/02/18#Lily Kao -end
        }
        //#NT#2011/02/10#Lily Kao -end
#endif
        break;

    case SOUND_CB_OUTDEVCHG:
        //#NT#2012/11/07#Lily Kao -begin
        //#NT#
        //GxSound_SetOutput(audOutDev);
        //GxSound_SetAudOutDev(audOutDev);
        GxSound_SetOutDevConfigIdx(audOutDev);
        //#NT#2012/11/07#Lily Kao -end
        break;
    default:
        debug_err(("Not Support SOUND_CB event=%d\r\n",event));
        break;
    }
    SoundTraceMsg("--Sound_CB:event=%d\r\n",event);
}

