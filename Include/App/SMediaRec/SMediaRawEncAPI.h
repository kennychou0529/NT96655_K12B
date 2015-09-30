/**
    SRaw encoder applications.

    To encode YUV raw data to JPEG during movie recording.

    @file       RawEncAPI.h
    @ingroup    mIAPPSMEDIAREC
    @note

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _SMRAWRECAPI_H
#define _SMRAWRECAPI_H

/**
    @addtogroup mIAPPSMEDIAREC
*/
//@{


/**
    @name raw encoding EVENT ID
    @note callback ID of raw encoder
*/
//@{
#define SMRAWENC_EVENT_RESULT_OK          1    ///< finish normally
#define SMRAWENC_EVENT_RESULT_ERR         2    ///< encode error
#define SMRAWENC_EVENT_RESULT_WRITE_ERR   3    ///< filesystem write fails
#define SMRAWENC_EVENT_RESULT_DCF_FULL    4    ///< DCF ID full
//@}

/**
    @name raw encoding results
*/
//@{
#define SMRAWENC_OK                       0   ///< encoding successfully
#define SMRAWENC_ERR_NOT_OPEN             1   ///< raw encode task is not opened
#define SMRAWENC_ERR_BUSY                 2   ///< raw encode task is busy
#define SMRAWENC_ERR_IMG_NOT_READY        3   ///< YUV raw image is not ready
#define SMRAWENC_ERR_UNSAFE_TO_WRITE      4   ///< memory may be not enough for writing JPEG file
//@}

/**
    Changeable parameters of raw encoder.

    Changeable parameters of raw encoder.
    @note
*/
typedef enum
{
    SMRAWENC_JPEG_WIDTH   = 0x0,  ///< output JPEG width, param1: width(in)
    SMRAWENC_JPEG_HEIGHT  = 0x1,  ///< output JPEG height, param1: height(in)
    SMRAWENC_Q            = 0x2,  ///< input encoding quality, param1: Q(in, 0~100)
    ENUM_DUMMY4WORD(SMRAW_CHANGE_TYPE)
} SMRAW_CHANGE_TYPE;

/**
    Export parameters of raw encoder.

    Export parameters of raw encoder.
    @note
*/
typedef struct
{
    //BOOL                bEnable;            ///< enable raw encode or not
    //UINT32              uiMemAddr;          ///< starting address for recording
    //UINT32              uiMemSize;          ///< size for recording
    //UINT32              uiMaxWidth;         ///< max encode pic width
    //UINT32              uiMaxHeight;        ///< max encode pic height
    //UINT32              uiJpegWidth;        ///< output JPEG width
    //UINT32              uiJpegHeight;       ///< output JPEG height
    UINT32              uiEncQ;             ///< input encoding Q
    //RAW_FORMAT_TYPE     uiRawFormat;        ///< raw format, RAW_FORMAT_444 or others
    //RawEncCallbackType  *CallBackFunc;      ///< event inform callback, remove 2012/08/30
} SMRAW_ENC_DATA;

//
//  Export APIs
//

/**
    Change raw encoding parameters.

    Change raw encoding parameters, such as width, height, encoding Q value.
    @note

    @param[in] type parameter type
    @param[in] p1 parameter1
    @param[in] p2 parameter2
    @param[in] p3 parameter3
    @return
        - @b E_OK:  change successfully.
*/
extern ER SMRawEnc_ChangeParameter(SMRAW_CHANGE_TYPE type, UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Trigger raw encoding.

    Trigger YUV raw encoding to output JPEG file.
    @note

    @return
        - @b SMRAWENC_OK:                     encoding successfully
        - @b SMRAWENC_ERR_NOT_OPEN:           raw encode task is not opened
        - @b SMRAWENC_ERR_BUSY:               raw encode task is busy
        - @b SMRAWENC_ERR_IMG_NOT_READY:      YUV raw image is not ready
        - @b SMRAWENC_ERR_UNSAFE_TO_WRITE:    memory may be not enough for writing JPEG file

    Example:
    @code
    {
        // Example for applying raw encoding function
        SMRawEnc_ChangeParameter(RAWENC_JPEG_WIDTH,  1920, 0, 0);
        SMRawEnc_ChangeParameter(RAWENC_JPEG_HEIGHT, 1080, 0, 0);
        SMRawEnc_TriggerEncode();
    }
    @endcode
*/
extern INT32    SMRawEnc_TriggerEncode(void);


//@}
#endif //_SMRAWRECAPI_H
