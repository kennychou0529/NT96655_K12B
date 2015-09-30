#ifndef _RTSPNVTAPI_H
#define _RTSPNVTAPI_H

#include "Type.h"

#define RTSPNVT_API_VERSION 0x14012412U ///< API version

/**
   Error Code.

   Any error code occured will display on uart or return.
*/
typedef enum _RTSPNVT_ER{
    RTSPNVT_ER_OK                =   0, ///< no error
    RTSPNVT_ER_MEDIA_PALY        =  -1, ///< there is error occured to use media play
    RTSPNVT_ER_TYPE              =  -2, ///< user's assigned OPEN::Type is not support
    ENUM_DUMMY4WORD(RTSPNVT_ER)
}RTSPNVT_ER;

/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum _RTSPNVT_WR{
    RTSPNVT_WR_OK                =   0, ///< no warning
    RTSPNVT_WR_ALREADY_OPEN      =  -1, ///< xRtspNvt_OnOpen called twice
    RTSPNVT_WR_NOT_OPEN          =  -2, ///< xRtspNvt_OnOpen not to call before.
    ENUM_DUMMY4WORD(RTSPNVT_WR)
}RTSPNVT_WR;

/**
   RTSP Type.

   RTSP uses 2 modules being a source input. One is MediaPlay playing the mov files.
   The other is LviewNvt sending live-view data.
*/
typedef enum _RTSPNVT_TYPE{
    RTSPNVT_TYPE_PLAYBACK        =   0, ///< playback via MediaPlay
    RTSPNVT_TYPE_LIVE_VIEW,             ///< live-view via LviewNvt
    RTSPNVT_TYPE_MEDIA_REC,             ///< live-view or recording via MediaRec without audio
    RTSPNVT_TYPE_MEDIA_REC_WITH_AUD,    ///< live-view or recording via MediaRec with audio
    ENUM_DUMMY4WORD(RTSPNVT_TYPE)
}RTSPNVT_TYPE;

/**
    AUDIO Sampling Rate
*/
typedef enum
{
    RTSPNVT_AUDIO_SR_8000   = 8000,     ///< 8 KHz
    RTSPNVT_AUDIO_SR_11025  = 11025,    ///< 11.025 KHz
    RTSPNVT_AUDIO_SR_12000  = 12000,    ///< 12 KHz
    RTSPNVT_AUDIO_SR_16000  = 16000,    ///< 16 KHz
    RTSPNVT_AUDIO_SR_22050  = 22050,    ///< 22.05 KHz
    RTSPNVT_AUDIO_SR_24000  = 24000,    ///< 24 KHz
    RTSPNVT_AUDIO_SR_32000  = 32000,    ///< 32 KHz
    RTSPNVT_AUDIO_SR_44100  = 44100,    ///< 44.1 KHz
    RTSPNVT_AUDIO_SR_48000  = 48000,    ///< 48 KHz

    ENUM_DUMMY4WORD(RTSPNVT_AUDIO_SR)
} RTSPNVT_AUDIO_SR;

/**
    AUDIO Channel
*/
typedef enum
{
    RTSPNVT_AUDIO_CH_LEFT,              ///< Left
    RTSPNVT_AUDIO_CH_RIGHT,             ///< Right
    RTSPNVT_AUDIO_CH_STEREO,            ///< Stereo
    RTSPNVT_AUDIO_CH_MONO,              ///< Mono two channel

    ENUM_DUMMY4WORD(RTSPNVT_AUDIO_CH)
} RTSPNVT_AUDIO_CH;

/**
    Video frame rate.
*/
typedef enum _RTSPNVT_FRAMERATE
{
    RTSPNVT_FRAMERATE_30 = 30,             ///< Video frame rate 30
    RTSPNVT_FRAMERATE_24 = 24,             ///< Video frame rate 24
    RTSPNVT_FRAMERATE_15 = 15,             ///< Video frame rate 15
    RTSPNVT_FRAMERATE_12 = 12,             ///< Video frame rate 12
    ENUM_DUMMY4WORD(RTSPNVT_FRAMERATE)
}RTSPNVT_FRAMERATE;

/**
    Server Event

    Most of events are mapping to RTSP command that refered to RTSP spec.
*/
typedef enum _RTSPNVT_EVENT
{
    RTSPNVT_EVENT_OPTIONS,      //recevie RTSP OPTION command
    RTSPNVT_EVENT_DESCRIBE,     //recevie RTSP DESCRIBE command
    RTSPNVT_EVENT_SETUP,        //recevie RTSP SETUP command
    RTSPNVT_EVENT_TEARDOWN,     //recevie RTSP TERADOWN command
    RTSPNVT_EVENT_PLAY,         //recevie RTSP PLAY command
    RTSPNVT_EVENT_PAUSE,        //recevie RTSP PAUSE command
    RTSPNVT_EVENT_FRAME_START_I,//start to send client an I-Frame
    RTSPNVT_EVENT_FRAME_START_P,//start to send client an P-Frame
    RTSPNVT_EVENT_FRAME_END,    //finished to send current frame
    ENUM_DUMMY4WORD(RTSPNVT_EVENT)
}RTSPNVT_EVENT;



typedef UINT32 (*RTSPNVT_GETVIDEOBUFCB)(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY); ///< Callback for getting video buffer, same as LviewNvtApi.h
typedef void   (*RTSPNVT_EVENTCB)(RTSPNVT_EVENT event);

/**
    RTSP-Live Data
*/
typedef struct _RTSPNVT_LIVE{
    //H264 Setting
    UINT32                uiBitRate;   ///< stream target size(Byte/Sec).
    RTSPNVT_FRAMERATE     uiFrameRate; ///< streaming frame rate. Normally, the value is 30.
    UINT32                uiImgWidth;  ///< video image width
    UINT32                uiImgHeight; ///< audio image hegiht
    RTSPNVT_GETVIDEOBUFCB fpGetVideo;  ///< a callback getting current preview YUV Data
    //PCM Setting
    BOOL                  bEnableAudio;     ///< indicating if send audio
    RTSPNVT_AUDIO_SR      AudioSampleRate;  ///< audio sample rate
    RTSPNVT_AUDIO_CH      AudioChannel;     ///< Audio channel
}RTSPNVT_LIVE;


/**
    Open Data.
*/
typedef struct _RTSPNVT_OPEN{
    UINT32              uiApiVer;   ///< just assign to RTSPNVT_API_VERSION
    RTSPNVT_TYPE        Type;       ///< Type for playback or live-view
    UINT32              uiPortNum;  ///< what port RTSP used, 554 is used normally.
    UINT32              uiSendBufSize; ///< for set of socket's sending buffer, set 0 to be default value 50k
    UINT32              uiWorkAddr; ///< working address
    UINT32              uiWorkSize; ///< working size, requirement is same with MediaPlay
    RTSPNVT_LIVE        LiveInfo;   ///< live view necessary information (set it Only for Type=RTSPNVT_TYPE_LIVE_VIEW)
    RTSPNVT_EVENTCB     fpEvent;    ///< (Optional) it can be NULL or send the RTSPNVT_EVENT to the callback you assign
    UINT32              uiTypeOfService; ///< (Optional) //IPTOS_LOWDELAY, IPTOS_THROUGHPUT, IPTOS_RELIABILITY, IPTOS_LOWCOST
}RTSPNVT_OPEN,*PRTSPNVT_OPEN;

extern void RtspNvt_InstallID(void) _SECTION(".kercfg_text");

extern RTSPNVT_ER RtspNvt_Open(RTSPNVT_OPEN* pOpen);
extern RTSPNVT_ER RtspNvt_Close(void);


/**
    Notify RtspNvt MediaRec Event

    Only for condition Type=RTSPNVT_TYPE_MEDIA_REC. While the event 'MEDIAREC_EVENT_ENCONE_OK' is coming, call it.
    If user employees NvtSystem, call DscMovie_RegRTSPSetBSCB(RtspNvt_RTSPSetBS); after calling RtspNvt_Open().
    And call DscMovie_RegRTSPSetBSCB(NULL); before calling RtspNvt_Close();

    @param[in] p1  not used currently. set it to 0 is ok.
    @param[in] p2  not used currently. set it to 0 is ok.
    @param[in] p3  not used currently. set it to 0 is ok.

    @return
        - @b E_OK: suspend success.
*/
extern UINT32 RtspNvt_RTSPSetBS(UINT32 p1, UINT32 p2, UINT32 p3); //p1,p2,p3 not used currently. set these to 0 is ok

#endif
