/**
    Header file of video codec H264 library

    Exported header file of video codec library.

    @file       VideoCodecH264.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _VIDEOCODECH264_H
#define _VIDEOCODECH264_H

#include "type.h"
#include "videoencode.h"
#include "videodecode.h"

/*
    Public funtions to get video codec object
*/
//@{
// new API, to support multi-path video encoding
extern PVIDEO_ENCODER MP_H264Enc_getVideoObject(VIDENC_ID VidEncId);
// for backward compatible
extern PVIDEO_ENCODER MP_H264Enc_getVideoEncodeObject(void);
extern PVIDEO_DECODER MP_H264Dec_getVideoDecodeObject(void);

/**
     Install H.264 plug-in semaphore id.
*/
extern void MP_H264Enc_InstallID(void) _SECTION(".kercfg_text");

//@}
#endif

