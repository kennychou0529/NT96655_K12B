/**
    Header file of movie interface

    movie interface

    @file       MovieInterface.h
    @ingroup    mIAPPMOVIEINTERFACE

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MOVIEINTERFACEDEF_H
#define _MOVIEINTERFACEDEF_H

#include "type.h"
#include "FilesysTsk.h"
#include "mediawritelib.h"
#include "SysKer.h"

//#define MODULE_DBGLVL           1 //0=OFF, 1=ERROR, 2=TRACE


/**
    Stop type for MovRec_Stop()
*/
typedef enum
{
    MOVREC_NO_WAIT  = 0x0,  ///< stop recording and no wait
    MOVREC_WAIT_END = 0x1,   ///< stop recording and wait this action finished
    ENUM_DUMMY4WORD(MOVREC_STOP_TYPE)
} MOVREC_STOP_TYPE;

#define MOVIEREC_AVIREC     0
#define MOVIEREC_MEDIAREC   1

#define MOVIEREC_JPGFMT_444  1
#define MOVIEREC_JPGFMT_422  2
#define MOVIEREC_JPGFMT_420  3

#define MOVIEREC_RECFORMAT_VIDEO_ONLY       1
#define MOVIEREC_RECFORMAT_AUD_VID_BOTH     2

#define MOVIEREC_1STQP_BEST     1280
#define MOVIEREC_1STQP_NORMAL   2560
#define MOVIEREC_1STQP_DRAFT    6400

#define MOVIEREC_ENDTYPE_CUT    1
#define MOVIEREC_ENDTYPE_NORMAL 2

typedef void (MovieRecCallback)(UINT32 event_id, UINT32 e_value);

typedef enum
{
    MOVREC_STATUS_NOT_OPENED        = 0x1,  ///< media recorder task is not open
    MOVREC_STATUS_OPENED_NOT_RECORD = 0x2,  ///< media recorder task is open, not recording
    MOVREC_STATUS_RECORDING         = 0x3,  ///< media recorder task is recording
    ENUM_DUMMY4WORD(MOVREC_STATUS_TYPE)
} MOVREC_STATUS_TYPE;

/**
    Video ready buffer information for movie recording.

*/
typedef struct
{
    UINT32      y;              ///<[in]: y address
    UINT32      cb;             ///<[in]: cb address
    UINT32      cr;             ///<[in]: cr address
    UINT32      y_lot;          ///<[in]: y line offset
    UINT32      uv_lot;         ///<[in]: uv line offset
    UINT32      uiBufID;        ///<[in]: IME ready buffer ID
} MEDIAREC_READYBUF_INFO;

/**
    check type for MediaRec_GetLastV_BS()
*/
typedef enum
{
    MEDIAREC_VIDBS_PATH1  = 0x0,  ///< video path1 bitstream
    MEDIAREC_VIDBS_PATH2  = 0x1,  ///< video path2 bitstream
    ENUM_DUMMY4WORD(MEDIAREC_VIDBS_TYPE)
} MEDIAREC_VIDBS_TYPE;

/**
    check type for MediaRec_SetDisk2SecRestriction()
*/
typedef enum
{
    MEDIAREC_RESTR_INDEXBUF = 0x0,  ///< check index buffer to calculate max recording time
    MEDIAREC_RESTR_FSLIMIT  = 0x1,  ///< check fs limitation, 4GB now
    ENUM_DUMMY4WORD(MEDIAREC_RESTR_TYPE)
} MEDIAREC_RESTR_TYPE;
//#NT#2010/05/17#Meg Lin -end

/**
    @name IPL mode change callback event

    Event ID for changing IPL mode callback function.
*/
//@{
#define MOVREC_IPLCHG_PREVIEW     1   ///< Set IPL to preview mode
#define MOVREC_IPLCHG_VIDEO       2   ///< Set IPL to video mode
//@}

/**
    @name Media ImmediatelyProc callback ID

    Callback ID of media ImmediatelyProc.
*/
//@{
#define     MOVREC_IMMPROC_CUTFILE_END        1  ///< end of one cut file, no parameter
#define     MOVREC_IMMPROC_DELETE_FILE        2  ///< end of deleting file, p1: directory ID, p2: file ID
#define     MOVREC_IMMPROC_ENC_SETTING        3  ///< special encoding parameters setting, such as H.264 3DNR
//@}

#define MOVREC_IPLGET_READYBUF    1   ///< Get IPL ready buffer

#define MOVREC_RECPARAM_WIDTH             0x01 ///< width, p1 valid, p2 fileid
#define MOVREC_RECPARAM_HEIGHT            0x02 ///< height, p1 valid, p2 fileid
#define MOVREC_RECPARAM_FRAMERATE         0x03 ///< frameRate, p1 valid
#define MOVREC_RECPARAM_QUALITY           0x04 ///< quality, p1 valid
#define MOVREC_RECPARAM_TARGETRATE        0x05 ///< target rate (bytes per second), p1 valid, p2 fileid
#define MOVREC_RECPARAM_GOLFSHOT_ON       0x08 ///< change recFormat to golfshot, p1 0 or 1
#define MOVREC_RECPARAM_CUTSEC            0x09 ///< seamless cut sec
#define MOVREC_RECPARAM_CUTSIZE           0x0A ///< seamless cut size
#define MOVREC_RECPARAM_ENDTYPE           0x0B ///< end type
#define MOVREC_RECPARAM_FLASH_ON          0x0C ///< flash recording on/off, p1 0 or 1
#define MOVREC_RECPARAM_PWROFFPT          0x0D ///< enable/disable poweroff_protection, default: on
#define MOVREC_RECPARAM_GPSON             0x0E ///< enable/disable gps info on, default: off
#define MOVREC_RECPARAM_TIMELAPSE         0x0F ///< set time lapse recording mode (ON/OFF)
#define MOVREC_RECPARAM_TIMELAPSE_TIME    0x10 ///< set time lapse recording time interval (in unit of ms, should be larger than 1/30 sec)
#define MOVREC_RECPARAM_THUMB_DATA        0x11 ///< movie thumbnail settings, p1:on/off, p2:width, p3:height
#define MOVREC_RECPARAM_AUD_SAMPLERATE    0x12 ///< audio sample rate, AUDIO_SR_8000 or others
#define MOVREC_RECPARAM_AUD_CHS           0x13 ///< audio channels, 1 or 2
#define MOVREC_RECPARAM_AUD_SRC           0x14 ///< audio source, MEDIAREC_AUDTYPE_LEFT or others
#define MOVREC_RECPARAM_AUD_CODEC         0x15 ///< audio codec type, MEDIAREC_ENC_PCM or others
#define MOVREC_RECPARAM_RECFORMAT         0x16 ///< recording format, MEDIAREC_AUD_VID_BOTH or others
#define MOVREC_RECPARAM_EN_CUSTOMUDATA    0x17 ///< enable custom user data or not
#define MOVREC_RECPARAM_VIDEOCODEC        0x18 ///< video codec type, MEDIAREC_ENC_H264 or others
#define MOVREC_RECPARAM_H264GOPTYPE       0x19 ///< h264 GOP type, MEDIAREC_H264GOP_IPONLY or others
#define MOVREC_RECPARAM_H2643DNRLEVEL     0x1A ///< h264 3DNR level
#define MOVREC_RECPARAM_RAWENC_Q          0x1B ///< Q value for raw-encoding
#define MOVREC_RECPARAM_EN_AUDFILTER      0x1C ///< enable audio filter or not
#define MOVREC_RECPARAM_DROP_FRAME_NUM    0x1D ///< drop frame number as start recording (for AV sync)
#define MOVREC_RECPARAM_H264QPLEVEL       0x1E ///< h264 QP initial level
#define MOVREC_RECPARAM_MOV_ROTATE        0x1F ///< MOV/MP4 file rotation, MEDIAREC_MOV_ROTATE_0
#define MOVREC_RECPARAM_FILETYPE_2        0x20 ///< file1 type, MEDIAREC_AVI as default
#define MOVREC_RECPARAM_MERGEYUV          0x21
#define MOVREC_RECPARAM_FILETYPE          0x87 ///< file type, MEDIAREC_MOV, MEDIAREC_AVI, MEDIAREC_MP4
#define MOVREC_RECPARAM_JPG_YUVFORMAT     0x22 ///< set jpeg yuv format
#define MOVREC_RECPARAM_MULTIREC_ON       0x23
#define MOVREC_RECPARAM_LIVEVIEW          0x24 ///< set MEDIAREC_LIVEVIEW recformat 2013/10/02 Meg
#define MOVREC_RECPARAM_DAR               0x25 ///< video display aspect ratio
#define MOVREC_RECPARAM_PRE_ENCODE        0x26 ///< video pre-encode (for applying 3DNR to 1st frame)
#define MOVREC_RECPARAM_SENSOR_FPS        0x27 ///< video sensor fps

#define     MOVREC_EVENT_RESULT_NORMAL        1   ///< Finish normally
#define     MOVREC_EVENT_RESULT_HW_ERR        2   ///< Hardware error
#define     MOVREC_EVENT_RESULT_FULL          3   ///< Card full
#define     MOVREC_EVENT_RESULT_SLOW          4   ///< Write card too slowly
#define     MOVREC_EVENT_RESULT_NOTREADY      5   ///< Not ready
#define     MOVREC_EVENT_THRESHOLD            6   ///< Threshold meets
#define     MOVREC_EVENT_RESULT_OVERTIME      7   ///< Overtime
#define     MOVREC_EVENT_RESULT_WRITE_ERR     10  ///< Filesystem write fails
#define     MOVREC_EVENT_STOPPED              11  ///< Callback when stops totally
#define     MOVREC_EVENT_FILECOMPLETE         13  ///< Write file completely
#define     MOVREC_EVENT_RESULT_CUT_FAIL      19  ///< Cut, overlap fail (write slowly)
#define     MOVREC_EVENT_CUSTOM_UDATA         21  ///< Set customized user data
#define     MOVREC_EVENT_ENCONE_OK            22  ///< enc one frame
#define     MOVREC_EVENT_H264BSCB_P0          23  ///< path1 video enc ok
#define     MOVREC_EVENT_H264BSCB_P1          24  ///< path2 video enc ok
#define     MOVREC_EVENT_RESULT_FLASHSTOP     25  ///< 30 sec after flash stop
#define     MOVREC_EVENT_AUDBSCB              26  ///< audio bitstream callback
#define     MOVREC_EVENT_LOOPREC_FULL         27  ///< no space for seamless recording
#define     MOVREC_EVENT_START                0x80 ///< start recording
#define     MOVREC_EVENT_TIMELAPSE_ONESHOT    0x81 ///< timelapse oneshot
#define     MOVREC_EVENT_SLOWSTART            0x82 ///< slow stop begins

typedef struct
{
    UINT32              uiMemAddr;          ///< starting address for recording
    UINT32              uiMemSize;          ///< size for recording
    MovieRecCallback    *CallBackFunc;      ///< event inform callback
    UINT32              uiRecFormat;        ///< recording type, MEDIAREC_AUD_VID_BOTH
    UINT32              uiVidFrameRate;     ///< recording video frame rate, MEDIAREC_VRATE_30
    UINT32              ui1stFrameQp;       ///< recording video quality
    UINT32              uiCBthreshold;      ///< video frames for callback, usually equals to frame rate
    UINT32              uiMaxWidth;         ///< recording max video width, 1440x1088 or 1280x720
    UINT32              uiMaxHeight;        ///< recording max video height
    UINT32              uiWidth;            ///< now recording video width, 1440x1088/1280x720/640x480/320x240
    UINT32              uiHeight;           ///< now recording video height
    UINT32              uiJpegFormat;
    UINT32              uiAudSampleRate;    ///< recording audio sample rate, AUDIO_SR_8000
    UINT32              uiAudBits;          ///< audio setting, bits per sample, 8 or 16
    UINT32              uiAudChannels;      ///< audio setting, channels, 1 or 2
    UINT32              audioCh;            ///< audio recording channel, AUDIO_CH_LEFT or others//2009/12/22 Meg
    UINT16              ucSeamlessSec;      ///< seamless cut second,
    UINT8               ucEngType;          ///< engine, MOVIEREC_AVIREC, MOVIEREC_MEDIAREC
    UINT8               ucEndType;          ///< end type, CUTOVERLAP, NORMAL
    UINT32              uiNoiseFilterLevel;       // 0: close noise filter
    UINT32              uiNoiseFilterNoiseDecay;  //0: noise*(1/2), 1: noise*(1/4), 2: noise*(1/8)
    UINT8               ucSaveFrameNum; //if memory enough, can be frame number. else, 3,5,6,10,15,30
    UINT8               rev;
    UINT32              uiUserDataAddr;
    UINT32              uiUserDataSize;
    UINT32              uiTargetFramesize;
    UINT32              uiAudioBufferAddr;
    UINT32              uiAudioBufferSize;
    UINT32              uiMaxRecSecs;
    UINT32              uiPanelWidth;       ///< input: panel size, 2011/06/10 Meg
    UINT32              uiPanelHeight;      ///< input: panel size, 2011/06/10 Meg

} MOVIEREC_OBJ;

typedef struct
{
    UINT32 Addr;    ///< Memory buffer starting address
    UINT32 Size;    ///< Memory buffer size
    UINT32 RawYAddr;///< thisBS Y address
} MEDIAREC_MEM_RANGE, *PMEDIAREC_MEM_RANGE;


typedef struct
{
    UINT64     nowFSFreeSpace;      //in: free space
    UINT32     vidfps;              //in: video frame per second
    UINT32     vidTBR;              //in: now video target bit rate
    UINT32     audSampleRate;       //in: audio sample rate
    UINT32     audCodec;            //in: MEDIAREC_ENC_PCM or MEDIAREC_ENC_AAC
    UINT32     audChs;              //in: ahdio channels
    UINT32     clustersize;         //in: now cluster size
    UINT32     gpson;               //in: if gps on
    UINT32     nidxon;              //in: if nidx on
} MEDIAREC_CALCSEC_SETTING;

typedef struct
{
    UINT32     bsOutAddr;           //in: bitstream address
    UINT32     bsOutSize;           //in: bitstream size
    UINT32     bsKey;               //in: if key-frame or not

} MEDIAREC_VIDEO_BS_INFO;

// Audio Filter export API
extern BOOL     AudFilter_Open(UINT32 uiChannels, BOOL bSmooth);
extern BOOL     AudFilter_Close(void);
extern BOOL     AudNoiseReduce_Open(void);
extern BOOL     AudNoiseReduce_Close(void);

#endif
