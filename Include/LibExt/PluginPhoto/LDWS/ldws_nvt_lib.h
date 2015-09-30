/**
    LDWS detection library.

    @file       ldws_nvt_lib.h
    @ingroup    mILibLDWS_Nvt

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _LDWS_NVT_LIB_H
#define _LDWS_NVT_LIB_H

/**
    @addtogroup mILibLDWS_Nvt
*/

/**
     @name LDWSreturn status
*/
//@{
#define LDWS_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define LDWS_STA_ERROR               (-1)                  ///<  some error occurred
//@}

typedef struct _LDWS_NVT_PARMS
{
    short FocalLen; // unit:mm
    short k; // ratio of focal length
    short eu;
    short ev;
    short euslope;
    short InitRow;
    short L_sensitivity;
    short R_sensitivity;
    short sensitivity;
    short Threshold01;
    short Threshold02;
    short Hs;
    short Fsize_DQ2;
    short Camera2CarDistance_cm;
} LDWS_NVT_PARMS;

typedef enum
{
    LDWS_DEPARTURE_NONE  = 0,
    LDWS_DEPARTURE_LEFT  = 1,
    LDWS_DEPARTURE_RIGHT = 2
} LDWS_NVT_DEPARTURE_DIR;

typedef enum
{
    LDWS_FAILURE_TRUE   = 1,
    LDWS_FAILURE_FALSE = 0
} LDWS_NVT_FAILURE_STATUS;

/*
                 LD_In_Width
 ---------------------------------------------
 - - (ROI_SX, ROI_SY)                        -
 -   -                                       -
 -     ---------------720----------------    -
 -     -                                -    -
 -     -   (Lx1,Ly1)*    * (Rx1,Ry1)    -    -
 -     -                               480   - LD_In_Height
 -     -                                -    -
 -     -(Lx2,Ly2)*           * (Rx2,Ry2)-    -
 -     ----------------------------------    -
 -                                           -
 -                                           -
 ---------------------------------------------
*/
typedef struct _LDWS_NVT_INFO
{
    UINT32 LD_In_Width; // width of ldws input image
    UINT32 LD_In_Height; // height of ldws input image
    UINT32 ROI_SX;       // ROI starting x coordinate of ldws input image
    UINT32 ROI_SY;       // ROI starting y coordinate of ldws input image
    UINT32 ROI_WIDTH;    // width of LDWS ROI
    UINT32 ROI_HEIGHT;   // height of LDWS ROI
    INT32  Lx1;          // pt1's x coordinate of left lane
    INT32  Ly1;          // pt1's y coordinate of left lane
    INT32  Lx2;          // pt2's x coordinate of left lane
    INT32  Ly2;          // pt2's y coordinate of left lane
    INT32  Rx1;          // pt1's x coordinate of right lane
    INT32  Ry1;          // pt1's y coordinate of right lane
    INT32  Rx2;          // pt2's x coordinate of right lane
    INT32  Ry2;          // pt2's y coordinate of right lane
    LDWS_NVT_FAILURE_STATUS Failure;      // Failure mode
    LDWS_NVT_DEPARTURE_DIR Departure_dir;// departure direction

} LDWS_NVT_INFO;


/**
     LDWS detection callback message.

*/
typedef enum _LDWS_NVT_CBMSG
{
    LDWS_NVT_CBMSG_WARNNING = 0,       ///< user will receive this notification when the lane departure warning occur
    LDWS_NVT_CBMSG_MAX_CNT,          ///< total callback message numbers
    ENUM_DUMMY4WORD(LDWS_NVT_CBMSG)
}
LDWS_NVT_CBMSG;


/**
    LDWS callback funcion proto type
*/
typedef void (*LDWS_NVT_CBMSG_FP)(LDWS_NVT_CBMSG uiMsgID, void *Data);

/**
    Get LDWS library version number.

    @return the version number string.
*/
extern CHAR *LDWS_Nvt_GetVerInfo(void);
/**
    Get LDWS library build date information.

    @return the library build date string.
*/
//extern CHAR *SD_GetBuildDate(void);

/**
     Init LDWS Detect engine.

     This function is used to initialize smile detection buffer and set the process rate.

     @param[in] buf: the work buffer for smile detection process. the required buffer size can be get by SD_CalcBuffSize().
     @param[in] ProcessRate: the process rate of SD_Process, if set value as 3, means calling the function SD_Process() 3 times will run the smile dection once.
     @return
         - @b LDWS_STA_OK:       Success.
         - @b LDWS_STA_ERROR:    Some error occurred.

     Example:
     @code
     {
         MEM_RANGE buf;

         buf.Addr = pbuf;
         buf.Size = LDWS_CalcBuffSize();

         LDWS_UnInit();
         // Init LDWS buffer , and set process rate as 5
         LDWS_Init(buf, 5);
     }
     @endcode
*/
extern INT32 LDWS_Nvt_Init(MEM_RANGE *buf, UINT32 ProcessRate);

/**
     Uninit LDWS engine.

     This function is used to release LDWS Detect engine.
     @return
         - @b LDWS_STA_OK:       Success.
         - @b LDWS_STA_ERROR:    Some error occurred.
*/
extern INT32 LDWS_Nvt_UnInit(void);

/**
     Process the LDWS detection.

     This function will find simles in the result of LDWS detection.
     and then detect if any LDW

     @note The process function will really run the LDWS detection when SD is unlocked. It will just returned when SD is locked.\n
           User can lock/unlock the LDWS function by calling LDWS_Lock().\n
           The process rate can be set by LDWS_Init().

     @return
         - @b >=0:    the number of detected LDWS.
         - @b <0:     Some error occurred.

     Example:
     @code
     {

     }
     @endcode
*/
extern INT32 LDWS_Nvt_Process(void);

/**
     Calculate LDWS detection required buffer size.

     @return the required buffer size.
*/
extern UINT32 LDWS_Nvt_CalcBuffSize(void);


/**
     Register callback function.

     @note User must register callback funcion then can receive the callback message notification when detected smiles.

     @param[in] CB: the callback function pointer.

     Example: Please see ModeFlow_PhotoCB.c
     @code
     {
        void Photo_LdwsCB(LDWS_CBMSG uiMsgID, void *Data)
        {
            switch (uiMsgID)
            {
                case LDWS_CBMSG_WARNNING:
                   DBG_IND("===LDWS Detected===\r\n");
                   //Ux_PostEvent(NVTEVT_ALGMSG_SDEND,1, NVTEVT_ALGMSG_SDEND);
                   break;
                default:
                   break;
            }
        }
        LDWS_RegisterCB(Photo_LdwsCB);
     }
     @endcode
*/
extern void   LDWS_Nvt_RegisterCB(LDWS_NVT_CBMSG_FP CB);

/**
     Lock/Unlock smile detction funcion.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the LDWS
         LDWS_Lock(TRUE);

         // doing something such as capture
         ...

         // end of capture

         // unlock the LDWS
         LDWS_Lock(FALSE);
     }
     @endcode
*/
extern void   LDWS_Nvt_Lock(BOOL bLock);

//2014.12.05 add function declaration
extern UINT32 LDWS_Nvt_CheckInitFlag(void);
extern void LDWS_Nvt_SetInitFlag(UINT32 Flag);
extern LDWS_NVT_INFO LDWS_Nvt_GetLdwsStatus(void);
extern UINT32 LDWS_Nvt_GetVanishingPtPos(void);



#endif

