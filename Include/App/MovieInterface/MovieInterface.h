/**
    Header file of movie interface

    movie interface

    @file       MovieInterface.h
    @ingroup    mIAPPMOVIEINTERFACE

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MOVIEINTERFACE_H
#define _MOVIEINTERFACE_H

#include "type.h"
#include "FilesysTsk.h"
#include "SysKer.h"

#include "IPL_Cmd.h"
#include "IPL_Utility.h"
#include "IPL_AlgInfor.h"
#include "ImgCaptureAPI.h"
#include "PhotoTask.h"
#include "GxImage.h" //for GX_IMAGE_PIXEL_FMT_YUV420_PACKED

#include "Sensor.h" //for SENSOR_MODE_1, SENSOR_MODE_2
#include "Movieinterface_def.h"

#include "MediaRecAPI.h"
#include "MediaRecApi.h"
#include "NameRule_DCFFull.h"
#include "IPL_Display.h"
#include "FileDB.h"
#include "NameRule_FileDB.h"
//#NT#2013/04/19#Calvin Chang#Add customer's Maker/Model name to MOV/MP4 file format by Nvtsystem -begin
#include "MOVLib.h"
#include "rtc.h"
#include "dcf.h"
#include "SMediaRecAPI.h"



/**
    Open movrec recorder task.

    Open movrec recorder task to prepare for recording.

    @param[in] param    the media recorder object

    @return
        - @b E_OK:  open Smedia recorder task successfully
        - @b E_SYS: Smedia recorder task is already opened
*/
extern ER   MovRec_Open(MEDIAREC_OBJ *pObj);

/**
    Close movrec recorder task.

    Close movrec recorder related tasks and stop recording if necessary.

    @return
        - @b E_OK:  terminate media recorder task successfully
        - @b E_SYS: Smedia recorder task is not opened
*/
extern ER   MovRec_Close(void);

/**
    Change movrec recorder parameters.

    Change movrec recorder parameters, such as width, height, frame rate, target bit rate, ..., and so on.

    @note

    @param[in] type parameter type
    @param[in] p1   parameter1
    @param[in] p2   parameter2
    @param[in] p3   parameter3

    @return
        - @b E_OK:  change successfully
        - @b E_PAR: parameter value error
*/
extern ER   MovRec_ChangeParameter(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Start movrec recording.

    @return
        - @b E_OK:  start recording successfully
        - @b E_SYS: Smedia recorder is recording or not ready
        - @b E_PAR: recorder setting error (such as recording format, video codec type, ...)
*/
extern ER   MovRec_Record(void);

/**
    Stop movrec recording.

    @param[in] waitType waiting type as media recording stop, MOVREC_NO_WAIT or MOVREC_WAIT_END

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER   MovRec_Stop(MOVREC_STOP_TYPE ucWait);
extern ER   MovRec_ResetMemoryValue(UINT32 addr, UINT32 size);//2011/06/15 Meg Lin

/**
    Re-allocate buffer to movrec recorder.

    Re-allocate buffer for movrec video and audio recording usage.

    @note The buffer should be re-allocated before media recording.

    @param[in] memStart buffer starting address
    @param[in] memSize  buffer size

    @return
        - @b E_OK:  re-allocate buffer successfully
        - @b E_SYS: re-allocate buffer failed (memory is not enough)
*/
extern ER   MovRec_ReAllocBuf2VA(UINT32 addr, UINT32 size);
extern void MovRec_CB(UINT32 uiEventID, UINT32 uiSeconds);

extern UINT32   MovRec_Disk2Second(void);
extern void     MovRec_MinusFilesize(UINT32 size);

/**
    Get movrec recorder parameter settings.

    Get movrec recorder parameter settings, only support getting width and height information currently.

    @note

    @param[in]  type    parameter type, only support MOVREC_GETINFO_WIDTH_HEIGHT currently
    @param[out] p1      parameter1 value
    @param[out] p2      parameter2 value
    @param[out] p3      parameter3 value

    @return
        - @b E_OK:  get parameter successfully
        - @b E_SYS: parameter address error
        - @b E_PAR: parameter type error
*/
extern ER       MovRec_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);

/**
    Set user data.

    Set user data for media container.

    @note The user data is in 'JUNK' chunk for AVI file, and in 'udta' atom for MOV/MP4 file.

    @param[in] addr user data starting address
    @param[in] size user data size

    @return void
*/
extern void     MovRec_SetUserData(UINT32 addr, UINT32 size);

/**
    Estimate remaining time for media recording.

    Estimate remaining time in seconds according to storage device space with different counting type.

    @note This function should be called before Smedia recording.

    @param[in] type counting type, MOVREC_COUNTTYPE_FS (according to storage free space),
                    MOVREC_COUNTTYPE_4G (FAT file size limitation), or MOVREC_COUNTTYPE_USER
                    (user defined space size, for special purpose)
    @param[in] size user defined space size, just for MOVREC_COUNTTYPE_USER type

    @return Estimated remaining time in seconds.
*/
extern UINT32   MovRec_Disk2SecondWithType(UINT32 type, UINT32 size);

/**
    Set restriction for recording time.

    Set restriction to calculate remaining time for Smedia recording.

    @note

    @param[in] type     restriction type, MOVREC_RESTR_INDEXBUF or MOVREC_RESTR_FSLIMIT
    @param[in] value    set FALSE to disable, and others to enable the related restriction

    @return void
*/
extern void     MovRec_SetDisk2SecRestriction(MEDIAREC_RESTR_TYPE type, UINT32 value);

/**
    Set free user data.

    Set free user data for special purpose.

    @note It's only valid for MOV/MP4 file. The data will be put in 'fre1' atom.

    @param[in] addr free data starting address
    @param[in] size free data size

    @return void
*/
extern void     MovRec_SetFre1Data(UINT32 addr, UINT32 size);

/**
    Stop movrec recording and set read-only.

    Stop movrec recording and set previous file, current file and next file to be read-only.

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       MovRec_StopAndReadOnly(void);

/**
    To get movrec recorder status.

    @return
        - @b MOVREC_STATUS_NOT_OPENED:        movrec recorder is not opened
        - @b MOVREC_STATUS_OPENED_NOT_RECORD: movrec recorder is opened but not recording
        - @b MOVREC_STATUS_RECORDING:         movrec recorder is recording
*/
extern MOVREC_STATUS_TYPE   MovRec_GetStatus(void);

/**
    Set audio volume for movrec Recorder.

    @param[in] percent  the percentage of audio gain level (0% - 100%)
    @return
        - @b E_OK:  set successfully
*/
extern ER       MovRec_SetAudioVolume(UINT32 percent);

/**
    Set ready buffer for movrec recorder.

    Set ready buffer information, such as Y, Cb, Cr starting address and line offset for media recorder.

    @note This function is usually used in IPLGetReadyCB (get IPL ready buffer callback) of media recorder object.
          In the callback, get IPL ready buffer from IPL driver, then use this API to set to media recorder.

    @param[in] pInfo    ready buffer information for media recorder

    @return void
*/
extern void     MovRec_PutReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern void     MovRec_GetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern void     MovRec_SetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern ER       MovRec_PutReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     MovRec_GetReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     MovRec_SetReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     MovRec_Pause(void);
extern void     MovRec_Resume(void);

/**
    Set GPS data.

    Set GPS data for movrec container.

    @note It's only valid for AVI file currently. The GPS data is put in 'JUNK' chunk per second.

    @param[in] addr GPS data starting address
    @param[in] size GPS data size

    @return void
*/
extern void     MovRec_SetGPSData(UINT32 addr, UINT32 size);

/**
    Set crash event.

    Set current recording file to be renamed and read-only.

    @note

    @return void
*/
extern void     MovRec_SetCrash(void);

/**
    Cancel crash event.

    Cancel crash event.

    @note

    @return void
*/
extern void     MovRec_CancelCrash(void);

/**
    To use same crash DID or not.

    To use same or different DCF directory ID for crash event.

    @note Set TRUE to use same directory ID, FALSE to use different directory ID.

    @param[in] on   turn on/off same crash DID function

    @return void
*/
extern void     MovRec_SetTheSameCrashDid(UINT8 on);

/**
    Set crash folder name.

    Set DCF folder name for crash event.

    @note It should follow the DCF naming rule, that is, 5 characters in 'A'~'Z', 'a'~'z', '0'~'9', and '_'.

    @param[in] pChar    pointer to the folder name

    @return void
*/
extern void     MovRec_SetCrashFolderName(char *pChar);

/**
    Set naming rule for movrec Recorder.

    @param[in] pHdl    pointer to the naming rule
    @return
        - @b E_OK:  set successfully
*/
extern ER       MovRec_SetFSNamingHandler(MEDIANAMINGRULE *pHdl);
extern void     MovRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode);
extern void     MovRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB);
extern void     MovRec_VideoDoEnc(void);
extern void     MovRec_SetFileHandle(FST_FILE fhdl, UINT32 fileid);
extern UINT32   MovRec_GetNowVideoFrames(void);

/**
    Open movrec recording error msg.

    @param[in] on  1 to open error msg
    @return void
*/
extern void     MovRec_OpenErrMsg(UINT8 on);

/**
    Reset movrec recorder containerMaker.

    @return void
*/
extern void     MovRec_ResetConMaker(void);

/**
    Open movrec recording warning msg.

    @param[in] on  1 to open warning msg
    @return void
*/
extern void     MovRec_OpenWrnMsg(UINT8 on);

/**
    Open movrec recording seamless recording msg.

    @param[in] on  1 to open seamless recording msg
    @return void
*/
extern void     MovRec_OpenCutMsg(UINT8 on);

/**
    Check movrec recorder if recording.

    @param[in] void
    @return
        - @b TRUE:  recording
        - @b FALSE: not recording
*/
extern BOOL     MovRec_IsRecording(void);

/**
    Set memory addr and size for MergeYUV.

    @param[in] addr memory address
    @param[in] size memory size
    @return void
*/
extern void     MovRec_SetGiveYUVBuf(UINT32 addr, UINT32 size);

/**
    Set readyYUV address for MergeYUV.

    @param[in] ptr    pointer to YUV info
    @return void
*/
extern void     MovRec_GiveYUV(MEDIAREC_READYBUF_INFO *ptr);

/**
    Set Extend crash.
    If seamless recording, this will set Crash after 30 sec.
    Max 3 times. If busy, return E_SYS.

    @param[in] void
    @return
        - @b E_OK: ok
        - @b E_SYS: busy
*/
extern ER       MovRec_SetExtendCrash(void);

/**
    Dump MediaStatus.

    @param[in] pData    pointer to data,p1=width,p2=height,p3=bpp,p4=fps
    @return number of data
*/
extern UINT32   MovRec_DumpDataStatus(UINT32* pData);

/**
    Get lastest video bitstream addr,size.

    @param[in] frameBuf    pointer to video frame
    @return 0
*/
extern UINT32   MovRec_GetLastV_BS(MEDIAREC_VIDBS_TYPE vidnum, MEDIAREC_MEM_RANGE* frameBuf);

/**
    Force movrec to check FS_DiskInfo.

    @param[in] void
    @return void
*/
extern void     MovRec_CheckFSDiskInfo(void);

/**
    Open movrec recording FSObj msg.

    @param[in] on  1 to open FSObj msg
    @return void
*/
extern void     MovRec_OpenFSOBJMsg(UINT8 on);

/**
    Open movrec recording file msg.

    @param[in] on  1 to open file msg
    @return void
*/
extern void     MovRec_OpenFileMsg(UINT8 on);

/**
    Set path2 write card or not.

    @param[in] on  1 to write path2 to card, if path2 enable
    @return void
*/
extern void     MovRec_SetPath2WriteCard(UINT32 on);

/**
    Set path2 IPL get ready buffer callback.

    @param[in] IPLCb  path2 IPL get ready buffer callback
    @return void
*/
extern void     MovRec_RegisterCB2(MediaRecIPLGetReadyCB IPLCb);

/**
    Minus filesize if some files written outside MediaREC.

    @param[in] size filesize, Must be cluster-alignment!!
    @return void
*/
extern void     MovRec_MinusFilesize(UINT32 size);
extern void     MovRec_StopPreRecordStartWriting(void);

/**
    Get H264 sps/pps for each video path.

    @param[in] fileid video path, 0 or 1
    @param[in] pAddr address to put sps/pps
    @param[in] pSize size of sps/pps
    @return void
*/
extern void     MovRec_VidGetDesc(UINT32 fileid, UINT32 *pAddr, UINT32 *pSize);
extern void     MovRec_InstallID(void);
/**
    Set MediaRec version.

    @param[in] ver MEDIAREC_VER_2_0/MEDIAREC_VER_3_0
    @return void
*/
extern void     MovRec_SetVersion(UINT32 ver);

/**
    Get minimum memory size for liveview mode.

    @param[in] void
    @return memory size
*/
extern UINT32   MovRec_GetLiveViewMinMemSize(void);

/**
    Update setting to calculate disk2second.

    @param[in] void
    @return void
*/
extern void     MovRec_UpdateSetting2CaluSec(void);

/**
    Set Customized Data. (Put end of file with specific tag)

    @param[in] tag abcd=0x61626364
    @param[in] addr data address
    @param[in] size data size
    @return void
*/
extern void     MovRec_SetCustomData(UINT32 tag, UINT32 addr, UINT32 size);

/**
    Set read-only percentage.

    @param[in] percent min=30, max=50
    @return void
*/
extern void MovRec_SetROPercent(UINT32 percent);

/**
    Enable end-char for single recording.

    @param[in] value on/off
    @return void
*/
extern void MovRec_EnableEndChar4SingleRec(UINT8 value);

/**
    Enable end-char for multiple recording.

    @param[in] value on/off
    @return void
*/
extern void MovRec_EnableEndChar4MultiRec(UINT8 value);

/**
    Get lastest audio bitstream.

    @param[in] frameBuf output audio bs addr/size
    @return void
*/
extern void MovRec_GetLastA_BS(MEDIAREC_MEM_RANGE* frameBuf);

/**
    Get lastest movie filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void MovRec_GetLastFilePath(char *pPath);

/**
    Enable Check and delete read-only file when seamless recoring.

    @param[in] value on/off, default: on
    @return void
*/
extern void MovRec_EnableCheckReadonly(UINT8 value);

/**
    Set max customdata size. Card capacity will minus this size when start recording.

    @param[in] fileID
    @param[in] maxSize
    @return void
*/
extern void MovRec_SetCustomDataMaxSize(UINT32 fileid, UINT32 maxSize);
extern void MovRec_SetCustomDataWithID(UINT32 id, UINT32 tag, UINT32 addr, UINT32 size);

#endif
