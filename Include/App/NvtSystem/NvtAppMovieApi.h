/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIDef.h
    @ingroup    mIPRJAPUIFlow

    @brief      UIDef Functions
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef NVT_MOVIE_API_H
#define NVT_MOVIE_API_H

#include "GxLib.h"
#include "MediaWriteLib.h"
#include "AudFilterAPI.h"


typedef void (MediaRecGetReadyCb2)(UINT32 mode, UINT32 param);
typedef UINT32 (MediaRecRTSPSetBS)(UINT32 p1, UINT32 p2, UINT32 p3);

typedef struct
{
    MediaRecGetReadyCb2    *GetReadyCb2;      ///< callback to get ready buffer
} NVT_MEDIAOBJ;

extern NVT_MEDIAOBJ gNvtMediaObj;
/**
    @name Media record event ID

    Callback ID of media recorder.
*/
//@{
#define MOVIE_CFG_POOL          1           //memory address for recording
#define MOVIE_CFG_CACHESIZE     2           //cache size, now 0
#define MOVIE_CFG_PIMSIZE       3           //external buffer for PIM, now 88M
#define MOVIE_CFG_RATIO         4           //crop ratio, IPL_SENCROPRATIO_4_3 or others
#define MOVIE_CFG_RSC           5           //RSC on/off, SEL_RSC_OFF or others
#define MOVIE_CFG_FUNCINFO      9           //PhotoFuncInfo_dummy
#define MOVIE_CFG_USE_FILEDB      10        //use fileDB or not, 0 or 1
#define MOVIE_CFG_FILEDB_MEM      11        //memory address for fileDB
#define MOVIE_CFG_MULTIREC        12        //multi-recording
#define MOVIE_CFG_MULTIREC_YUVCB  13        //multi-recording, getYUV CB
#define MOVIE_CFG_IMGPATH         14        //config different image size,refer to
//@}

typedef enum _MOVIE_IMGPATH_CFG
{
    MOVIE_NORMAL = 0,           //normal movie preview,movie record
    MOVIE_DUAL_SENSOR,          //dual sensor,2 image source from IPL_ID_1,IPL_ID_2
    MOVIE_WIFI_PREVIEW,         //wifi preview
    MOVIE_WIFI_RECORD,          //wifi record
    MOVIE_DUAL_DISPLAY, //dual display
}MOVIE_IMGPAEH_CFG;

extern VIEW_SRC gMovie_src;

/**
    Config settings for movie recording.

    Config settings for movie recording, such as MOVIE_CFG_POOL or others.

    @param[in] config_id MOVIE_CFG_POOL or others.
    @param[in] value configuration value.

    @return void
*/
extern void DscMovie_Config(UINT32 config_id, UINT32 value);

/**
    Get config settings for movie recording.

    Get config settings for movie recording, such as MOVIE_CFG_IMGPATH or others.

    @param[in] config_id MOVIE_CFG_IMGPATH or others.

    @return configuration value
*/

extern UINT32 DscMovie_GetConfig(UINT32 config_id);

/**
    Open movie recorder.

    Open movie recorder.

    @param[in] void

    @return void
*/
extern void DscMovie_Open(void);

/**
    Close movie recorder.

    Close movie recorder.

    @param[in] void

    @return void
*/
extern void DscMovie_Close(void);

/**
    Start to record a movie file.

    Start to record a movie file.

    @param[in] void

    @return void
*/
extern void DscMovie_RecStart(void);

/**
    Stop recording and save a movie file.

    Stop recording and save a movie file.

    @param[in] void

    @return void
*/
extern void DscMovie_RecStop(void);

/**
    Open IPL.

    Open IPL.

    @param[in] void

    @return void
*/
extern void DscMovie_OpenIPL(void);

/**
    Close IPL.

    Close IPL.

    @param[in] void

    @return void
*/
extern void DscMovie_CloseIPL(void);

/**
    Set user maker data and model data for MOV files.

    Set user maker data and model data for MOV files.

    @param[in] p8Buf temp buffer to put user data
    @param[in] pcMakername address of MakerName
    @param[in] uMakersize size of MakerName
    @param[in] pcModelname address of ModelName
    @param[in] uModelsize size of ModelName

    @return void
*/
extern UINT32 DscMovie_MakeUserMakerModelData(UINT8 *p8Buf,
                                              unsigned char *pcMakername, UINT32 uMakersize,
                                              unsigned char *pcModelname, UINT32 uModelsize);
/**
    Register draw function for movie recording.

    Register callback function for data drawing (such as date stamp) as movie recording.

    @param[in] pDrawCB callback function to be registered

    @return void
*/
extern void DscMovie_RegDrawCB(MediaRecDrawStr *pDrawCB);

/**
    Register audio filter function for movie recording.

    Register callback function for audio filtering as movie recording.

    @param[in] pAudFilterCB callback function to be registered

    @return void
*/
extern void DscMovie_RegAudFilterCB(AUDFILTER_FILTER_CB pAudFilterCB);

/**
    Register callback function for processing events immediately.

    Register callback function for the events which should be processed immediately
    as movie recording (cannot use post event).

    @param[in] pImmProcCB callback function to be registered

    @return void
*/
extern void DscMovie_RegImmProcCB(MediaRecImmediatelyProc *pImmProcCB);

/**
    Register callback function for MediaPlay_Open().

    Register callback function for the events from Media Play Object

    @param[in] event_id  define Media Play Event ID

    @return void
*/
extern void Play_MovieCB(UINT32 event_id);
extern void DscMovie_SetGetReadyCb2(void *pobj);

/**
    Register callback function for RTSP to set video BS.

    Register callback function for RTSP to set video BS.

    @param[in] pImmProcCB callback function to be registered

    @return void
*/
extern void DscMovie_RegRTSPSetBSCB(MediaRecRTSPSetBS *pRTSPCB);

/**
    Register FileDB folder name.

    Register FileDB folder name.

    @param[in] pStr folder name, default: CARDV

    @return void
*/
extern void DscMovie_SetFDBFolder(char *pStr);

/**
    Register FileDB readonly folder name.

    Register FileDB readonly folder name.

    @param[in] pStr folder name, default: MOVIE\RO

    @return void
*/
extern void DscMovie_SetFDB_ROFolder(char *pStr);

/**
    Register FileDB photo folder name. (for rawEnc)

    Register FileDB photo folder name. (for rawEnc)

    @param[in] pStr folder name, default: PHOTO

    @return void
*/
extern void DscMovie_SetFDB_PhotoFolder(char *pStr);

/**
    Register FileDB movie folder name.

    Register FileDB movie folder name.

    @param[in] pStr folder name, default: MOVIE

    @return void
*/
extern void DscMovie_SetFDB_MovieFolder(char *pStr);

#endif//NVT_MOVIE_API_H

