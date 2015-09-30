#ifndef _LIVE555API_H
#define _LIVE555API_H

#include "Live555Common.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern INT32 Live555_Open(void);
    extern INT32 Live555_Close(void);
    extern INT32 Live555_SetInterface(LIVE555_INTERFACE_CB* pInterface);
    extern INT32 Live555_SetVideoInfo(LIVE555_VIDEO_INFO* pInfo);
    extern INT32 Live555_SetAudioInfo(LIVE555_AUDIO_INFO* pInfo);
    extern INT32 Live555_SetServerEventCb(LIVE555_FP_SERVER_EVENT fpCb);

    //for RTSP-Client
    extern INT32 Live555_Client_Open(LIVE555_CLIENT_INFO* pInfo);
    extern INT32 Live555_Client_Close(void);

#ifdef __cplusplus
} //extern "C"
#endif

#endif