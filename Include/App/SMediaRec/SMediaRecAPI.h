/**
    Header file of SMedia Recorder Task

    Exported header file of media recorder task.

    @file       SMediaRecAPI.h
    @ingroup    mIAPPSMEDIAREC
    @note       add gSMediaRecPwrOffPT

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _SMEDIARECAPI_H
#define _SMEDIARECAPI_H

#include "MediaWriteLib.h"
#include "AVFile_MakerMov.h"
#include "AVFile_MakerAvi.h"
#include "MediaReadLib.h"
#include "AVFile_ParserMov.h"
#include "AVFile_ParserAvi.h"
#include "VideoEncode.h"
#include "movieinterface.h"

/**
    @addtogroup mIAPPSMEDIAREC
*/
//@{

/**
    Stop type for SMediaRec_Stop()
*/
typedef enum
{
    SMEDIAREC_NO_WAIT  = 0x0,  ///< stop recording and no wait
    SMEDIAREC_WAIT_END = 0x1,   ///< stop recording and wait this action finished
    ENUM_DUMMY4WORD(SMEDIAREC_STOP_TYPE)
} SMEDIAREC_STOP_TYPE;

//#NT#2010/05/17#Meg Lin -begin
//#NT#add restriction type

/**
    check type for SMediaRec_SetWriteCard()
*/
typedef enum
{
    SMEDIAREC_WRITE_PATH1  = 0x0,  ///< write path 1 to card
    SMEDIAREC_WRITE_PATH2  = 0x1,  ///< write path 2 to card
    ENUM_DUMMY4WORD(SMEDIAREC_WRITECARD_TYPE)
} SMEDIAREC_WRITECARD_TYPE;

/**
    @name SMedia record event ID

    Callback ID of Smedia recorder.
*/
//@{
#define     SMEDIAREC_EVENT_RESULT_NORMAL        1   ///< Finish normally
#define     SMEDIAREC_EVENT_RESULT_HW_ERR        2   ///< Hardware error
#define     SMEDIAREC_EVENT_RESULT_FULL          3   ///< Card full
#define     SMEDIAREC_EVENT_RESULT_SLOW          4   ///< Write card too slowly
#define     SMEDIAREC_EVENT_RESULT_NOTREADY      5   ///< Not ready
#define     SMEDIAREC_EVENT_THRESHOLD            6   ///< Threshold meets
#define     SMEDIAREC_EVENT_RESULT_OVERTIME      7   ///< Overtime
#define     SMEDIAREC_EVENT_RESULT_WRITE_ERR     10  ///< Filesystem write fails
#define     SMEDIAREC_EVENT_STOPPED              11  ///< Callback when stops totally
#define     SMEDIAREC_EVENT_FILECOMPLETE         13  ///< Write file completely
#define     SMEDIAREC_EVENT_RESULT_CUT_FAIL      19  ///< Cut, overlap fail (write slowly)
#define     SMEDIAREC_EVENT_CUSTOM_UDATA         21  ///< Set customized user data
#define     SMEDIAREC_EVENT_ENCONE_OK            22  ///< enc one frame

#define     SMEDIAREC_EVENT_H264BSCB_P0          23  ///< path1 video enc ok
#define     SMEDIAREC_EVENT_H264BSCB_P1          24  ///< path2 video enc ok
#define     SMEDIAREC_EVENT_RESULT_FLASHSTOP     25  ///< 30 sec after flash stop
#define     SMEDIAREC_EVENT_AUDBSCB              26  ///< audio enc ok
#define     SMEDIAREC_EVENT_START                0x80 ///< start recording
#define     SMEDIAREC_EVENT_TIMELAPSE_ONESHOT    0x81 ///< timelapse oneshot
#define     SMEDIAREC_EVENT_SLOWSTART            0x82 ///< slow stop begins
//@}

//#NT#2012/09/12#Meg Lin -begin
/**
    @name SMedia ImmediatelyProc callback ID

    Callback ID of Smedia ImmediatelyProc.
*/
//@{
#define     SMEDIAREC_IMMPROC_CUTFILE_END        1  ///< end of one cut file, no parameter
#define     SMEDIAREC_IMMPROC_DELETE_FILE        2  ///< end of deleting file, p1: directory ID, p2: file ID
#define     SMEDIAREC_IMMPROC_ENC_SETTING        3  ///< special encoding parameters setting, such as H.264 3DNR
//@}
//#NT#2012/09/12#Meg Lin -end

/**
    @name SMedia record parameters

    Media recording changeable parameters.
*/
//@{
#define SMEDIAREC_RECPARAM_WIDTH             0x01 ///< width, p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_HEIGHT            0x02 ///< height, p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_FRAMERATE         0x03 ///< frameRate, p1 valid
#define SMEDIAREC_RECPARAM_QUALITY           0x04 ///< quality, p1 valid
#define SMEDIAREC_RECPARAM_TARGETRATE        0x05 ///< target rate (bytes per second), p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_GOLFSHOT_ON       0x08 ///< change recFormat to golfshot, p1 0 or 1
#define SMEDIAREC_RECPARAM_CUTSEC            0x09 ///< seamless cut sec
#define SMEDIAREC_RECPARAM_CUTSIZE           0x0A ///< seamless cut size
#define SMEDIAREC_RECPARAM_ENDTYPE           0x0B ///< end type
#define SMEDIAREC_RECPARAM_FLASH_ON          0x0C ///< flash recording on/off, p1 0 or 1
#define SMEDIAREC_RECPARAM_PWROFFPT          0x0D ///< enable/disable poweroff_protection, default: on
#define SMEDIAREC_RECPARAM_GPSON             0x0E ///< enable/disable gps info on, default: off
#define SMEDIAREC_RECPARAM_TIMELAPSE         0x0F ///< set time lapse recording mode (ON/OFF)
#define SMEDIAREC_RECPARAM_TIMELAPSE_TIME    0x10 ///< set time lapse recording time interval (in unit of ms, should be larger than 1/30 sec)
#define SMEDIAREC_RECPARAM_THUMB_DATA        0x11 ///< movie thumbnail settings, p1:on/off, p2:width, p3:height
#define SMEDIAREC_RECPARAM_AUD_SAMPLERATE    0x12 ///< audio sample rate, AUDIO_SR_8000 or others
#define SMEDIAREC_RECPARAM_AUD_CHS           0x13 ///< audio channels, 1 or 2
#define SMEDIAREC_RECPARAM_AUD_SRC           0x14 ///< audio source, MEDIAREC_AUDTYPE_LEFT or others
#define SMEDIAREC_RECPARAM_AUD_CODEC         0x15 ///< audio codec type, MEDIAREC_ENC_PCM or others
#define SMEDIAREC_RECPARAM_RECFORMAT         0x16 ///< recording format, MEDIAREC_AUD_VID_BOTH or others
#define SMEDIAREC_RECPARAM_EN_CUSTOMUDATA    0x17 ///< enable custom user data or not
#define SMEDIAREC_RECPARAM_VIDEOCODEC        0x18 ///< video codec type, MEDIAREC_ENC_H264 or others
#define SMEDIAREC_RECPARAM_H264GOPTYPE       0x19 ///< h264 GOP type, MEDIAREC_H264GOP_IPONLY or others
#define SMEDIAREC_RECPARAM_H2643DNRLEVEL     0x1A ///< h264 3DNR level
#define SMEDIAREC_RECPARAM_RAWENC_Q          0x1B ///< Q value for raw-encoding
#define SMEDIAREC_RECPARAM_EN_AUDFILTER      0x1C ///< enable audio filter or not
#define SMEDIAREC_RECPARAM_DROP_FRAME_NUM    0x1D ///< drop frame number as start recording (for AV sync)
#define SMEDIAREC_RECPARAM_H264QPLEVEL       0x1E ///< h264 QP initial level
//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
#define SMEDIAREC_RECPARAM_MOV_ROTATE        0x1F ///< MOV/MP4 file rotation, MEDIAREC_MOV_ROTATE_0
//#NT#2013/04/17#Calvin Chang -end
#define SMEDIAREC_RECPARAM_FILETYPE_2        0x20 ///< file1 type, MEDIAREC_AVI as default
#define SMEDIAREC_RECPARAM_MERGEYUV          0x21
#define SMEDIAREC_RECPARAM_FILETYPE          0x87 ///< file type, MEDIAREC_MOV, MEDIAREC_AVI, MEDIAREC_MP4
#define SMEDIAREC_RECPARAM_JPG_YUVFORMAT     0x22 ///< set jpeg yuv format
#define SMEDIAREC_RECPARAM_MULTIREC_ON       0x23
#define SMEDIAREC_RECPARAM_LIVEVIEW          0x24 ///< set MEDIAREC_LIVEVIEW recformat 2013/10/02 Meg
#define SMEDIAREC_RECPARAM_DAR               0x25 ///< video display aspect ratio
#define SMEDIAREC_RECPARAM_PRE_ENCODE        0x26 ///< video pre-encode (for applying 3DNR to 1st frame)
#define SMEDIAREC_RECPARAM_SENSOR_FPS        0x27 ///< video sensor fps

//@}

/**
    @name Valid video frame rate

    Valid video frame rate.
*/
//@{
#define SMEDIAREC_VRATE_2            2  ///< 2 fps, only for MEDIAREC_FAST_FWD
#define SMEDIAREC_VRATE_10           10 ///< 10 fps
#define SMEDIAREC_VRATE_15           15 ///< 15 fps
#define SMEDIAREC_VRATE_20           20 ///< 20 fps
#define SMEDIAREC_VRATE_24           24 ///< 24 fps
#define SMEDIAREC_VRATE_25           25 ///< 25 fps
#define SMEDIAREC_VRATE_30           30 ///< 30 fps
#define SMEDIAREC_VRATE_48           48  ///< 48 fps, only for 960p/720p/VGA/QVGA
#define SMEDIAREC_VRATE_50           50  ///< 50 fps, only for 960p/720p/VGA/QVGA
#define SMEDIAREC_VRATE_60           60  ///< 60 fps, only for 720p/VGA/QVGA
#define SMEDIAREC_VRATE_120          120 ///< 120 fps, only for VGA/QVGA
#define SMEDIAREC_VRATE_240          240 ///< 240 fps, only for QVGA
//@}

/**
    @name Valid file type

    Valid file type.
*/
//@{
#define SMEDIAREC_AVI    MEDIA_FILEFORMAT_AVI    ///< .avi file
#define SMEDIAREC_MOV    MEDIA_FILEFORMAT_MOV    ///< .mov file
#define SMEDIAREC_MP4    MEDIA_FILEFORMAT_MP4    ///< .mp4 file
//@}

/**
    @name Valid video encode codec

    Valid video encode codec.
*/
//@{
#define SMEDIAREC_ENC_JPG    MEDIAVIDENC_MJPG    ///< motion JPG
#define SMEDIAREC_ENC_H264   MEDIAVIDENC_H264    ///< H.264
//@}

/**
    @name Valid audio encode codec

    Valid audio encode codec.
*/
//@{
#define SMEDIAREC_ENC_PCM    MOVAUDENC_PCM   ///< PCM
#define SMEDIAREC_ENC_AAC    MOVAUDENC_AAC   ///< AAC
//@}

/**
    @name Valid video input YCbCr format

    Valid video input YCbCr format.
*/
//@{
#define SMEDIAREC_JPEG_FORMAT_420    0  ///< Y:Cb:Cr = 4:1:1
#define SMEDIAREC_JPEG_FORMAT_422    1  ///< Y:Cb:Cr = 2:1:1
#define SMEDIAREC_JPEG_FORMAT_444    2  ///< Y:Cb:Cr = 1:1:1
#define SMEDIAREC_JPEG_FORMAT_100    3  ///< Y:Cb:Cr = 4:1:1
//@}

/**
    @name Valid counting type

    Valid counting type for MediaRec_Disk2SecondWithType.
*/
//@{
#define SMEDIAREC_COUNTTYPE_FS    1  ///< get freespace in storage
#define SMEDIAREC_COUNTTYPE_4G    2  ///< using 4G to calculate
#define SMEDIAREC_COUNTTYPE_USER  3  ///< using given size to calculate
//@}

/**
    @name Valid end type

    Valid recording end type. Normal end, cut overlap, or cut till card full.
*/
//@{
#define SMEDIAREC_ENDTYPE_NORMAL             1 ///< normal end, such as press key to stop recording
#define SMEDIAREC_ENDTYPE_CUTOVERLAP         2 ///< cut file and continue recording automatically; overlap the old files (cyclic recording)
#define SMEDIAREC_ENDTYPE_CUT_TILLCARDFULL   3 ///< cut file and continue recording automatically till card full
#define SMEDIAREC_ENDTYPE_MAX                3 ///< maximum number of recording end type
//@}

//#NT#2012/01/09#Hideo Lin -begin
//#NT#Add media recording source type (for test mode)
/**
    @name Media recording source type

    Media recording video source type: normal, fixed YUV pattern, bitstream;
    Media recording audio source type: normal, fixed PCM pattern.
*/
//@{
#define SMEDIAREC_SRC_NORMAL         0   ///< normal source (video: sensor, audio: mic-in)
#define SMEDIAREC_SRC_FIXED_YUV      1   ///< fixed YUV pattern (for video)
#define SMEDIAREC_SRC_BS_ONLY        2   ///< bitstream only (for video)
#define SMEDIAREC_SRC_FIXED_PCM      1   ///< fixed PCM pattern (for audio)
//@}
//#NT#2012/01/09#Hideo Lin -end

/**
    @name IPL mode change callback event

    Event ID for changing IPL mode callback function.
*/
//@{
#define SMEDIAREC_IPLCHG_PREVIEW     1   ///< Set IPL to preview mode
#define SMEDIAREC_IPLCHG_VIDEO       2   ///< Set IPL to video mode
//@}

/**
    @name Get IPL ready buffer callback event

    Event ID for getting IPL ready buffer callback function.
*/
//@{
#define SMEDIAREC_IPLGET_READYBUF    1   ///< Get IPL ready buffer
//@}

/**
    @name Smedia record information

    SMedia recording getting information.
*/
//@{
#define     SMEDIAREC_GETINFO_WIDTH_HEIGHT   1   ///< p1: width (out), p2: height (out)
#define     SMEDIAREC_GETINFO_VIDEO_CODEC    2   ///< p1: videocodec type(out), MEDIAREC_ENC_JPG or others//2012/10/15 Meg
#define     SMEDIAREC_GETINFO_AUDIO_SR       3   ///< p1: audio samplerate(out), AUDIO_SR_32000 or others//2013/01/11 Meg
#define     SMEDIAREC_GETINFO_AUDIO_CHS      4   ///< p1: audio channels(out), 1 or 2//2013/01/11 Meg
#define     SMEDIAREC_GETINFO_AUDIO_BITS     5   ///< p1: audio bits per sample, 16
#define     SMEDIAREC_GETINFO_H264_SPS       6   ///< p1: addr (out), p2: size (out), p3: num(0 or 1)
#define     SMEDIAREC_GETINFO_VIDEO_FPS      7   ///< p1: v fps (out)
#define     SMEDIAREC_GETINFO_WIDTH_HEIGHT_P2 8  ///< p1: width of Path2 (out), p2: height of Path2(out)
#define     SMEDIAREC_GETINFO_AUDIO_CODEC    9   ///< p1: audio codec type of 1stpath(out), MEDIAREC_ENC_PCM or others
//@}

/**
    @name Smedia audio type information

    SMedia recording audio type information.
*/
//@{
#define     SMEDIAREC_AUDTYPE_LEFT   1   ///< left
#define     SMEDIAREC_AUDTYPE_RIGHT  2   ///< right
#define     SMEDIAREC_AUDTYPE_STEREO 3   ///< stereo
#define     SMEDIAREC_AUDTYPE_MONO   4   ///< mono
//@}

//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
/**
    @name Smedia rotation angle information

    SMedia recording roation angle information.
*/
//@{
#define     SMEDIAREC_MOV_ROTATE_0       0   ///< angle 0
#define     SMEDIAREC_MOV_ROTATE_90      1   ///< angle 90
#define     SMEDIAREC_MOV_ROTATE_180     2   ///< angle 180
#define     SMEDIAREC_MOV_ROTATE_270     3   ///< angle 270
//@}
//#NT#2013/04/17#Calvin Chang -end

/**
    Status type for SMediaRec_GetStatus()
*/
typedef enum
{
    SMEDIAREC_STATUS_NOT_OPENED        = 0x1,  ///< media recorder task is not open
    SMEDIAREC_STATUS_OPENED_NOT_RECORD = 0x2,  ///< media recorder task is open, not recording
    SMEDIAREC_STATUS_RECORDING         = 0x3,  ///< media recorder task is recording
    ENUM_DUMMY4WORD(SMEDIAREC_STATUS_TYPE)
} SMEDIAREC_STATUS_TYPE;

//#NT#2012/02/23#Meg Lin -begin
typedef struct
{
    UINT32      uiPCMAddr;          //in: address to put pcm
    UINT32      uiPCMSize;          //in: size to put pcm
    UINT32      uiAACBSAddr;        //in: address to put aac bitstream
    UINT32      uiAACBSSize;        //in: size to put aac bitstream
    UINT32      uiAudSampleRate;    //in: AUDIO_SR_8000
    UINT32      uiAudBitRate;       //in: target bitrate
    UINT32      uiAudRawBlock;      //out: bytes to encode one audio BS (1024 samples for AAC)
    UINT32      uiAudChannels;      //in: channels, 1 or 2
    BOOL        bTestByFile;        //in: test by file (TRUE) or by microphone (FALSE)
    char        *pPCMFileName;      //in: the file name for writing PCM data
    char        *pAACFileName;      //in: the file name for writing AAC data
} SMEDIAREC_AACETEST_OBJ;
//#NT#2012/02/23#Meg Lin -end

typedef struct
{
    UINT32      uiInAddr;           //in: address for AAC bs input
    UINT32      uiOutAddr;          //in: address for PCM output
    UINT32      uiAudSampleRate;    //in: AUDIO_SR_8000
    UINT32      uiAudChannels;      //in: channels, 1 or 2
    char        *pPCMFileName;      //in: the file name for writing PCM data
    char        *pAACFileName;      //in: the file name for reading AAC data
} SMEDIAREC_AACDTEST_OBJ;

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
} SMEDIAREC_READYBUF_INFO;

/**
     Install Task, Sem, Flag ID for SMediaRec.

*/
extern void SMediaRec_InstallID(void) _SECTION(".kercfg_text");

/*
    Public Smedia recording functions
*/
//MediaRecTsk.c
/**
    Open Smedia recorder task.

    Open Smedia recorder task to prepare for recording.

    @param[in] param    the media recorder object

    @return
        - @b E_OK:  open Smedia recorder task successfully
        - @b E_SYS: Smedia recorder task is already opened
*/
extern ER       SMediaRec_Open(MEDIAREC_OBJ *param);

/**
    Close Smedia recorder task.

    Close Smedia recorder related tasks and stop recording if necessary.

    @return
        - @b E_OK:  terminate media recorder task successfully
        - @b E_SYS: Smedia recorder task is not opened
*/
extern ER       SMediaRec_Close(void);

/**
    Start Smedia recording.

    @return
        - @b E_OK:  start recording successfully
        - @b E_SYS: Smedia recorder is recording or not ready
        - @b E_PAR: recorder setting error (such as recording format, video codec type, ...)
*/
extern ER       SMediaRec_Record(void);

/**
    Stop Smedia recording.

    @param[in] waitType waiting type as media recording stop, MEDIAREC_NO_WAIT or MEDIAREC_WAIT_END

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       SMediaRec_Stop(SMEDIAREC_STOP_TYPE waitType);

/**
    Change Smedia recorder parameters.

    Change Smedia recorder parameters, such as width, height, frame rate, target bit rate, ..., and so on.

    @note

    @param[in] type parameter type
    @param[in] p1   parameter1
    @param[in] p2   parameter2
    @param[in] p3   parameter3

    @return
        - @b E_OK:  change successfully
        - @b E_PAR: parameter value error
*/
extern ER       SMediaRec_ChangeParameter(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Get Smedia recorder parameter settings.

    Get Smedia recorder parameter settings, only support getting width and height information currently.

    @note

    @param[in]  type    parameter type, only support SMEDIAREC_GETINFO_WIDTH_HEIGHT currently
    @param[out] p1      parameter1 value
    @param[out] p2      parameter2 value
    @param[out] p3      parameter3 value

    @return
        - @b E_OK:  get parameter successfully
        - @b E_SYS: parameter address error
        - @b E_PAR: parameter type error
*/
extern ER       SMediaRec_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);

/**
    Set user data.

    Set user data for media container.

    @note The user data is in 'JUNK' chunk for AVI file, and in 'udta' atom for MOV/MP4 file.

    @param[in] addr user data starting address
    @param[in] size user data size

    @return void
*/
extern void     SMediaRec_SetUserData(UINT32 addr, UINT32 size);

/**
    Estimate remaining time for media recording.

    Estimate remaining time in seconds according to storage device space with different counting type.

    @note This function should be called before Smedia recording.

    @param[in] type counting type, SMEDIAREC_COUNTTYPE_FS (according to storage free space),
                    SMEDIAREC_COUNTTYPE_4G (FAT file size limitation), or MEDIAREC_COUNTTYPE_USER
                    (user defined space size, for special purpose)
    @param[in] size user defined space size, just for SMEDIAREC_COUNTTYPE_USER type

    @return Estimated remaining time in seconds.
*/
extern UINT32   SMediaRec_Disk2SecondWithType(UINT32 type, UINT32 size);//2010/01/25 Meg

/**
    Set restriction for recording time.

    Set restriction to calculate remaining time for Smedia recording.

    @note

    @param[in] type     restriction type, SMEDIAREC_RESTR_INDEXBUF or SMEDIAREC_RESTR_FSLIMIT
    @param[in] value    set FALSE to disable, and others to enable the related restriction

    @return void
*/
extern void     SMediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_TYPE type, UINT32 value);//2010/05/17 Meg Lin

/**
    Set free user data.

    Set free user data for special purpose.

    @note It's only valid for MOV/MP4 file. The data will be put in 'fre1' atom.

    @param[in] addr free data starting address
    @param[in] size free data size

    @return void
*/
extern void     SMediaRec_SetFre1Data(UINT32 addr, UINT32 size);

/**
    Re-allocate buffer to Smedia recorder.

    Re-allocate buffer for Smedia video and audio recording usage.

    @note The buffer should be re-allocated before media recording.

    @param[in] memStart buffer starting address
    @param[in] memSize  buffer size

    @return
        - @b E_OK:  re-allocate buffer successfully
        - @b E_SYS: re-allocate buffer failed (memory is not enough)
*/
extern ER       SMediaRec_ReAllocBuf2VA(UINT32 memStart, UINT32 memSize);

/**
    Stop Smedia recording and set read-only.

    Stop Smedia recording and set previous file, current file and next file to be read-only.

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       SMediaRec_StopAndReadOnly(void);//2012/02/17 Meg

/**
    To get Smedia recorder status.

    @return
        - @b SMEDIAREC_STATUS_NOT_OPENED:        Smedia recorder is not opened
        - @b SMEDIAREC_STATUS_OPENED_NOT_RECORD: Smedia recorder is opened but not recording
        - @b SMEDIAREC_STATUS_RECORDING:         Smedia recorder is recording
*/
extern SMEDIAREC_STATUS_TYPE   SMediaRec_GetStatus(void);

/**
    Set audio volume for SMedia Recorder.

    @param[in] percent  the percentage of audio gain level (0% - 100%)
    @return
        - @b E_OK:  set successfully
*/
extern ER SMediaRec_SetAudioVolume(UINT32 percent);


extern void     SMediaRec_Sleep(void);
extern void     SMediaRec_Wakeup(void);
extern void     SMediaRec_PauseDMA(void);
extern void     SMediaRec_ResumeDMA(void);
extern void     SMediaRec_ChangeDisplaySize(UINT32 uiDisp_w, UINT32 uiDisp_h);


//SMediaVideoRecTsk.c
/**
    Set ready buffer for Smedia recorder.

    Set ready buffer information, such as Y, Cb, Cr starting address and line offset for media recorder.

    @note This function is usually used in IPLGetReadyCB (get IPL ready buffer callback) of media recorder object.
          In the callback, get IPL ready buffer from IPL driver, then use this API to set to media recorder.

    @param[in] pInfo    ready buffer information for media recorder

    @return void
*/
extern void     SMediaRec_ResetReadyBuf(void);
extern void     SMediaRec_PutReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern void     SMediaRec_GetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern void     SMediaRec_SetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);//2012/08/30 Meg
extern ER       SMediaRec_PutReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     SMediaRec_GetReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     SMediaRec_SetReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);//2012/08/30 Meg


//SMediaAudioRecTsk.c
extern void     SMediaRec_Pause(void);
extern void     SMediaRec_Resume(void);

//SMediaRecFSTsk.c
/**
    Set GPS data.

    Set GPS data for Smedia container.

    @note It's only valid for AVI file currently. The GPS data is put in 'JUNK' chunk per second.

    @param[in] addr GPS data starting address
    @param[in] size GPS data size

    @return void
*/
extern void     SMediaRec_SetGPSData(UINT32 addr, UINT32 size);//2011/06/23 Meg

/**
    Set crash event.

    Set current recording file to be renamed and read-only.

    @note

    @return void
*/
extern void     SMediaRec_SetCrash(void);//2011/06/27 Meg

/**
    Cancel crash event.

    Cancel crash event.

    @note

    @return void
*/
extern void     SMediaRec_CancelCrash(void);//2013/11/18 Meg


/**
    To use same crash DID or not.

    To use same or different DCF directory ID for crash event.

    @note Set TRUE to use same directory ID, FALSE to use different directory ID.

    @param[in] on   turn on/off same crash DID function

    @return void
*/
extern void     SMediaRec_SetTheSameCrashDid(UINT8 on);//2011/09/15 Meg

/**
    Set crash folder name.

    Set DCF folder name for crash event.

    @note It should follow the DCF naming rule, that is, 5 characters in 'A'~'Z', 'a'~'z', '0'~'9', and '_'.

    @param[in] pChar    pointer to the folder name

    @return void
*/
extern void     SMediaRec_SetCrashFolderName(char *pChar);//2011/09/15 Meg

/**
    Set naming rule for SMedia Recorder.

    @param[in] pHdl    pointer to the naming rule
    @return
        - @b E_OK:  set successfully
*/
extern ER SMediaRec_SetFSNamingHandler(MEDIANAMINGRULE *pHdl);


extern void SMediaRecCmdTsk(void);
extern void SMediaRecTsk(void);
extern void SMediaVideoRecTsk(void);
extern void SMRawEncTsk(void);
extern void SMediaAudioEncTsk(void);
extern void SMediaRecFSTsk(void);
extern void SMediaRecVideoStrmTsk(void);
extern void SMediaRecMdatTsk(void);


extern void SMediaRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode);
extern void SMediaRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB);
extern void SMediaRec_VideoDoEnc(void);
extern void SMediaRec_GetEncInfo(UINT32 puiParam[7]);
extern void SMediaRec_SetFileHandle(FST_FILE fhdl, UINT32 fileid);
extern void SMediaRecFS_SetWorkingID(UINT32 id);
extern UINT32   SMediaRec_GetNowVideoFrames(void);

/**
    Open Smedia recording error msg.

    @param[in] on  1 to open error msg
    @return void
*/
extern void SMediaRec_OpenErrMsg(UINT8 on);

/**
    Reset Smedia recorder containerMaker.

    @return void
*/
extern void SMediaRec_ResetConMaker(void);

/**
    Open Smedia recording warning msg.

    @param[in] on  1 to open warning msg
    @return void
*/
extern void SMediaRec_OpenWrnMsg(UINT8 on);

/**
    Open media recording seamless recording msg.

    @param[in] on  1 to open seamless recording msg
    @return void
*/
extern void SMediaRec_OpenCutMsg(UINT8 on);

/**
    Check Smedia recorder if recording.

    @param[in] void
    @return
        - @b TRUE:  recording
        - @b FALSE: not recording
*/
extern BOOL SMediaRec_IsRecording(void);

/**
    Set memory addr and size for MergeYUV.

    @param[in] addr memory address
    @param[in] size memory size
    @return void
*/
extern void SMediaRec_SetGiveYUVBuf(UINT32 addr, UINT32 size);

/**
    Set readyYUV address for MergeYUV.

    @param[in] ptr    pointer to YUV info
    @return void
*/
extern void SMediaRec_GiveYUV(MEDIAREC_READYBUF_INFO *ptr);

/**
    Set Extend crash.
    If seamless recording, this will set Crash after 30 sec.
    Max 3 times. If busy, return E_SYS.

    @param[in] void
    @return
        - @b E_OK: ok
        - @b E_SYS: busy
*/
extern ER SMediaRec_SetExtendCrash(void);//2013/09/04 Meg

/**
    Dump MediaStatus.

    @param[in] pData    pointer to data,p1=width,p2=height,p3=bpp,p4=fps
    @return number of data
*/
extern UINT32 SMediaRec_DumpDataStatus(UINT32* pData);//2013/08/15

/**
    Get lastest video bitstream addr,size.

    @param[in] frameBuf    pointer to video frame
    @return 0
*/
extern UINT32 SMediaRec_GetLastV_BS(MEDIAREC_VIDBS_TYPE vidnum, MEDIAREC_MEM_RANGE* frameBuf);

/**
    Force SMediaRec to check FS_DiskInfo.

    @param[in] void
    @return void
*/
extern void SMediaRec_CheckFSDiskInfo(void);

/**
    Open Smedia recording FSObj msg.

    @param[in] on  1 to open FSObj msg
    @return void
*/
extern void SMediaRec_OpenFSOBJMsg(UINT8 on);

/**
    Open Smedia recording file msg.

    @param[in] on  1 to open file msg
    @return void
*/
extern void SMediaRec_OpenFileMsg(UINT8 on);

/**
    Set path2 write card or not.

    @param[in] on  1 to write path2 to card, if path2 enable
    @return void
*/
extern void SMediaRec_SetPath2WriteCard(UINT32 on);


/**
    Set path2 IPL get ready buffer callback.

    @param[in] IPLCb  path2 IPL get ready buffer callback
    @return void
*/
extern void SMediaRec_RegisterCB2(MediaRecIPLGetReadyCB IPLCb);

/**
    Minus filesize if some files written outside MediaREC.

    @param[in] size filesize, Must be cluster-alignment!!
    @return void
*/
extern void SMediaRec_MinusFilesize(UINT32 size);

extern void SMediaRec_StopPreRecordStartWriting(void);

/**
    Get H264 sps/pps for each video path.

    @param[in] fileid video path, 0 or 1
    @param[in] pAddr address to put sps/pps
    @param[in] pSize size of sps/pps
    @return void
*/
extern void     SMediaRec_VidGetDesc(UINT32 fileid, UINT32 *pAddr, UINT32 *pSize);

/**
    Get minimum memory size for liveview mode.

    @param[in] void
    @return memory size
*/
extern UINT32 SMediaRec_GetLiveViewMinMemSize(void);

/**
    Update setting to calculate disk2second.

    @param[in] void
    @return void
*/
extern void SMediaRec_UpdateSetting2CaluSec(void);

/**
    Set Customized Data. (Put end of file with specific tag)

    @param[in] tag abcd=0x61626364
    @param[in] addr data address
    @param[in] size data size
    @return void
*/
extern void SMediaRec_SetCustomData(UINT32 tag, UINT32 addr, UINT32 size);

/**
    Set read-only percentage.

    @param[in] percent min=30, max=50
    @return void
*/
extern void SMediaRec_SetROPercent(UINT32 percent);

/**
    Enable end-char for single recording.

    @param[in] value on/off
    @return void
*/
extern void SMediaRec_EnableEndChar4SingleRec(UINT8 value);

/**
    Enable end-char for multiple recording.

    @param[in] value on/off
    @return void
*/
extern void SMediaRec_EnableEndChar4MultiRec(UINT8 value);

/**
    Get lastest audio bitstream.

    @param[in] frameBuf output audio bs addr/size
    @return void
*/
extern void SMediaRec_GetLastA_BS(MEDIAREC_MEM_RANGE* frameBuf);

/**
    Get lastest movie filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void SMediaRec_GetLastFilePath(char *pPath);

/**
    Enable Check and delete read-only file when seamless recoring.

    @param[in] value on/off, default: on
    @return void
*/
extern void SMediaRec_EnableCheckReadonly(UINT8 value);

/**
    Input settings to calculate recording seconds.

    @param[in] pSetting input recording setting
    @return recording seconds
*/
extern UINT32 SMediaRec_CalcSecBySetting(MEDIAREC_CALCSEC_SETTING *pSetting);

#endif//_SMEDIARECAPI_H

//@}
