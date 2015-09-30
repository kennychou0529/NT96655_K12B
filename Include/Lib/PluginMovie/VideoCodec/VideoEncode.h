/**
    Header file of video encoding codec library

    Exported header file of video encoding codec library.

    @file       VideoEncode.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _VIDEOENCODE_H
#define _VIDEOENCODE_H

#include <stdio.h>
#include <string.h>
#include "type.h"
#include "KERNEL.h"

/**
    @addtogroup mIAVCODEC
*/
//@{

/**
    Each bitstream buffer size for N frames encoding simultaneously by H.264 encoder
*/
#define MEDIAREC_H264ENC_BSBUFSIZE  (1024*1024) ///< 1MB for BS temp buffer as simultaneously encode

/**
    @name H.264 3DNR settings
    @note
*/
//@{
//New H264 3DNR Levels
#define H264_3DNR_LV0           0   ///< H.264 3DNR: disable
#define H264_3DNR_LV1           1   ///< H.264 3DNR: level 1
#define H264_3DNR_LV2           2   ///< H.264 3DNR: level 2
#define H264_3DNR_LV3           3   ///< H.264 3DNR: level 3
#define H264_3DNR_LV4           4   ///< H.264 3DNR: level 4
#define H264_3DNR_LV5           5   ///< H.264 3DNR: level 5
#define H264_3DNR_LV6           6   ///< H.264 3DNR: level 6
#define H264_3DNR_LV7           7   ///< H.264 3DNR: level 7
#define H264_3DNR_LV8           8   ///< H.264 3DNR: level 8
#define H264_3DNR_LV9           9   ///< H.264 3DNR: level 9
#define H264_3DNR_LV10          10  ///< H.264 3DNR: level 10

//Old H264 3DNR Levels for compatibility
#define H264_3DNR_DISABLE           H264_3DNR_LV0   ///< H.264 3DNR: disable
#define H264_3DNR_WEAKEST           H264_3DNR_LV1   ///< H.264 3DNR: weakest
#define H264_3DNR_WEAK              H264_3DNR_LV2   ///< H.264 3DNR: weak
#define H264_3DNR_NORMAL            H264_3DNR_LV3   ///< H.264 3DNR: normal
#define H264_3DNR_STRONG            H264_3DNR_LV4   ///< H.264 3DNR: strong
#define H264_3DNR_STRONGEST         H264_3DNR_LV5   ///< H.264 3DNR: strongest1
#define H264_3DNR_STRONGEST2        H264_3DNR_LV6   ///< H.264 3DNR: strongest2
#define H264_3DNR_STRONGEST3        H264_3DNR_LV7   ///< H.264 3DNR: strongest3
#define H264_3DNR_STRONGEST4        H264_3DNR_LV8   ///< H.264 3DNR: strongest4
#define H264_3DNR_STRONGEST5        H264_3DNR_LV9   ///< H.264 3DNR: strongest5
#define H264_3DNR_STRONGEST6        H264_3DNR_LV10   ///< H.264 3DNR: strongest6
//@}

/**
    @name H.264 QP initial level
    @note
*/
//@{
#define H264_QP_LEVEL_1             0   ///< H.264 QP initial level 1 (low compression)
#define H264_QP_LEVEL_2             1   ///< H.264 QP initial level 2
#define H264_QP_LEVEL_3             2   ///< H.264 QP initial level 3
#define H264_QP_LEVEL_4             3   ///< H.264 QP initial level 4
#define H264_QP_LEVEL_5             4   ///< H.264 QP initial level 5
#define H264_QP_LEVEL_6             5   ///< H.264 QP initial level 6 (high compression)
//@}

/**
    Video Encoder ID
*/
//@{
typedef enum
{
    VIDENC_ID_1,                ///< video encoder ID 1
    VIDENC_ID_2,                ///< video encoder ID 2
    VIDENC_ID_MAX,              ///< video encoder ID maximum
    ENUM_DUMMY4WORD(VIDENC_ID)
} VIDENC_ID;
//@}

/**
    Video Display Aspect Ratio
*/
//@{
typedef enum
{
    VIDENC_DAR_DEFAULT,         ///< default video display aspect ratio (the same as encoded image)
    //VIDENC_DAR_4_3,             ///< 4:3 video display aspect ratio
    //VIDENC_DAR_3_2,             ///< 3:2 video display aspect ratio
    VIDENC_DAR_16_9,            ///< 16:9 video display aspect ratio
    VIDENC_DAR_MAX,             ///< maximum video display aspect ratio
    ENUM_DUMMY4WORD(VIDENC_DAR)
} VIDENC_DAR;
//@}

/**
    type of getting information from video encoding codec library.
    format: ER (*GetInfo)(VIDCOENC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
*/
typedef enum
{
    MEDIAENC_GETINFO_VIDENCPARAM = 0x01,    ///< get video encoding parmater (p1: MEDIAREC_VIDENC_PARAM pointer)
    MEDIAENC_GETINFO_H264DESC    = 0x90,    ///< get H.264 description (p1: addr, p2: len)
    MEDIAENC_GETINFO_ISIFRAME    = 0x91,    ///< i frame or not (p1: true or false)
    MEDIAENC_GETINFO_YUVADDR     = 0x92,    ///< get H.264 encoding address (p1: y addr, p2: uv addr)
    ENUM_DUMMY4WORD(VIDCOENC_GETINFO_TYPE)
} VIDCOENC_GETINFO_TYPE;

/**
    type of setting information to video encoding codec library.
    format: ER (*SetInfo)(VIDCOENC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum
{
    MEDIAENC_SETINFO_YCBCRADDR      = 0x01, ///< set YCbCr addr, (p1: y addr, p2: cb addr, p3: cr addr)
    MEDIAENC_SETINFO_WID_HEI        = 0x02, ///< set encoding image width/height (p1: width, p2: height)
    MEDIAENC_SETINFO_QUALITY        = 0x03, ///< set JPEG quality (old setting, no need now)
    MEDIAENC_SETINFO_ENCODEBUF      = 0x04, ///< set codec buffer (p1: addr, p2: size)
    MEDIAENC_SETINFO_RATECONTROL    = 0x05, ///< set target data rate (p1: target data rate in bytes)
    MEDIAENC_SETINFO_VIDRATE        = 0x06, ///< set video frame rate (p1: frame rate)
    MEDIAENC_SETINFO_YUVOFFSET      = 0x07, ///< set Y, UV line offset (p1: Y line offset, p2: UV line offset)
    //MEDIAENC_SETINFO_DISADDR        = 0x08, ///< set Y, U, V addr for DIS (no need now)
    MEDIAENC_SETINFO_JPG_MODE       = 0x80, ///< set JPEG format (no need now)
    MEDIAENC_SETINFO_JPG_DCOUTPUT   = 0x81, ///< set JPEG DC out on/off (no need now)
    MEDIAENC_SETINFO_JPG_YUVFORMAT  = 0x82, ///< set JPEG YUV format
    MEDIAENC_SETINFO_H264_SETPARAM  = 0x90, ///< set H.264 parameters (no param)
    MEDIAENC_SETINFO_H264_ENDENC    = 0x91, ///< stop H.264 encoding (no param)
    MEDIAENC_SETINFO_H264_IPRATIO   = 0x92, ///< set I-P ratio (p1: I+n*P)
    MEDIAENC_SETINFO_H264_3DNR      = 0x93, ///< set H.264 3DNR level (p1: 3DNR level)
    MEDIAENC_SETINFO_PRELOAD        = 0x94, ///< pre-load image for H.264 encoding with B-frame (no param)
    MEDIAENC_SETINFO_H264_QP        = 0x95, ///< set H.264 QP initial level (p1: QP initial level)
    MEDIAENC_SETINFO_DAR            = 0x96, ///< set video display aspect ratio
    MEDIAENC_SETINFO_BSTR_INFO      = 0xA0, ///< set bitstream pattern info for test scheme
    ENUM_DUMMY4WORD(VIDCOENC_SETINFO_TYPE)
} VIDCOENC_SETINFO_TYPE;

/**
    Video encoding parameters structure for EncodeOne()
*/
typedef struct
{
    UINT32 encAddr;     ///< codec needed memory
    UINT32 encSize;     ///< size for codec memory
    UINT32 yAddr;       ///< encoding y address
    UINT32 cbAddr;      ///< encoding cb address
    UINT32 crAddr;      ///< encoding cr address
    UINT32 mode;        ///< codec encoding mode
    UINT32 thisSize;    ///< [out] size of output frame
    UINT32 targetRate;  ///< target rate (in bytes)
    UINT32 DisY_Addr;   ///< encoding DIS y addr //2010/01/25 Meg, add for DIS
    UINT32 DisU_Addr;   ///< encoding DIS u addr
    UINT32 DisV_Addr;   ///< encoding DIS v addr
    UINT16 width;       ///< frame width
    UINT16 height;      ///< frame height
    UINT16 frameRate;   ///< frame rate
    UINT16 rev16;       ///< reserved 16-bit data
    UINT32 ip_ratio;    ///< p frame number to make a GOP
    UINT32 ipb_bnum;    ///< b frame between p
    UINT32 DC_output;   ///< (JPEG) DC output enable/disable
    UINT32 qValue;      ///< initilizing q value
    //#NT#2010/01/14#Meg Lin -begin
    //#NT#add for DIS
    UINT32 uiYLineOffset;       ///< Y line offset
    UINT32 uiUVLineOffset;      ///< UV line offset
    //#NT#2010/01/14#Meg Lin -end
    //#NT#2012/03/14#Hideo Lin -begin
    //#NT#For 2 B-frame encode simulation test
    UINT32 uiBsAddr[2];         ///< output bit-stream address (for max 2 frames)
    UINT32 uiBsSize[2];         ///< output bit-stream size (for max 2 frames)
    UINT32 uiBsEndAddr;         ///< output bit-stream end address
    UINT32 uiEncFrameNum;       ///< total frame number for simultaneously encoding
    UINT32 uiEncFrameCount;     ///< frame count for simultaneously encoding
    //#NT#2012/03/14#Hideo Lin -end
    UINT32 uiAddImgCount;       ///< adding image count
    UINT32 uiFrameCount;        ///< current frame count
    UINT32 ui3DNRLevel;         ///< H.264 3DNR level
    UINT32 uiQPLevel;           ///< H.264 QP initial level
    UINT32 uiYuvFormat;         ///< YUV format
    UINT32 uiEncYaddr ;         ///< output this BS from RAW Yaddr
    VIDENC_DAR  uiDAR;          ///< video display aspect ratio

} MEDIAREC_VIDENC_PARAM;

//#NT#2011/12/27#JustinLee -begin
//#NT#Add bitstream pattern test scheme
/**
    Video bitstream test pattern number
*/
//#define VIDENC_BSTR_VF_NUM_MAX  15  ///< input video bitstream test pattern max frame numbers
//#define VIDENC_BSTR_VF_NUM_MAX  30  ///< input video bitstream test pattern max frame numbers
#define VIDENC_BSTR_VF_NUM_MAX  60  ///< input video bitstream test pattern max frame numbers

/**
    Video bitstream test pattern data
*/
typedef struct
{
    UINT32 inBStrAddr;          ///< [in] input bitstream addr
    UINT32 inBStrSize;          ///< [in] input bitstream size
} MEDIAREC_VIDENC_BSTR_ENTRY;

/**
    H.264 description for video bitstream test pattern
*/
typedef struct
{
    UINT32 inBStrH264DescAddr;  ///< [in] input H264 description addr (if bitstream codec using H264)
    UINT32 inBStrH264DescSize;  ///< [in] input H264 description size (if bitstream codec using H264)
} MEDIAREC_VIDENC_BSTR_H264DESC;

/**
    Video bitstream test pattern information
*/
typedef struct
{
    UINT32                          totalBStrVFN;                   ///< [in] total input video bitstream frame numbers
    MEDIAREC_VIDENC_BSTR_H264DESC   bstrH264Desc;                   ///< [in] H264 description data (necessary if bitstream codec using H264)
    MEDIAREC_VIDENC_BSTR_ENTRY      bstrVF[VIDENC_BSTR_VF_NUM_MAX]; ///< [in] bitstream frames
} MEDIAREC_VIDENC_BSTR_INFO;
//#NT#2011/12/27#JustinLee -end

//#NT#2012/01/09#Hideo Lin -begin
//#NT#To assign specific YUV pattern for video recording
/**
    Fixed YUV test pattern number
*/
#define VIDENC_FIXED_YUV_NUM_MAX    8   ///< max number of fixed YUV test patterns

/**
    Fixed YUV test pattern source data
*/
typedef struct
{
    UINT32  uiYAddr;            ///< Y address
    UINT32  uiCbAddr;           ///< Cb address
    UINT32  uiCrAddr;           ///< Cr address
    UINT32  uiYLineOffset;      ///< Y line offset
    UINT32  uiUVLineOffset;     ///< UV line offset
} MEDIAREC_VIDENC_YUV_SRC;

/**
    Fixed YUV test pattern information
*/
typedef struct
{
    UINT32                      uiTotalNum;                         ///< [in] total YUV pattern number
    MEDIAREC_VIDENC_YUV_SRC     YUVSrc[VIDENC_FIXED_YUV_NUM_MAX];   ///< [in] YUV source information
} MEDIAREC_VIDENC_YUV_INFO;
//#NT#2012/01/09#Hideo Lin -end

/**
    Structure of video encoder
*/
typedef struct
{
    ER (*Initialize)(void);  ///< initializing codec
    ER (*GetInfo)(VIDCOENC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);  ///< get information from codec library
    ER (*SetInfo)(VIDCOENC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3); ///< set parameters to codec library
    ER (*EncodeOne)(UINT32 type, UINT32 outputAddr, UINT32 *pSize, MEDIAREC_VIDENC_PARAM *ptr); ///< encoding one video frame
    ER (*AdjustBPS)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3); ///< adjusting BPS
    ER (*CustomizeFunc)(UINT32 type, void *pobj); ///< codec customize function
    UINT32 checkID;     ///< check ID
} VIDEO_ENCODER, *PVIDEO_ENCODER;



//@}
#endif
