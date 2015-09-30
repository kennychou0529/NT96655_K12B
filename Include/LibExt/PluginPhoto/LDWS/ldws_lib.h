/**
    LDWS detection library.

    @file       ldws_lib.h
    @ingroup    mILibLDWS

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _LDWS_LIB_H
#define _LDWS_LIB_H

/**
    @addtogroup mILibLDWS
*/

/**
     @name LDWSreturn status
*/
//@{
#define LDWS_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define LDWS_STA_ERROR               (-1)                  ///<  some error occurred
//@}

typedef struct _LDWS_INFO
{
    INT32 ldws_status;
    INT32 Line1_m;  //y1=m1 *x + b1;
    INT32 Line1_b;  //y1=m1 *x + b1;
    INT32 Line2_m;  //y2=m2 *x + b2;
    INT32 Line2_b;  //y2=m2 *x + b2;
    INT32 ldws_warning;
} LDWS_INFO;


/**
     LDWS detection callback message.

*/
typedef enum _LDWS_CBMSG
{
    LDWS_CBMSG_WARNNING = 0,       ///< user will receive this notification when the lane departure warning occur
    LDWS_CBMSG_MAX_CNT,          ///< total callback message numbers
    ENUM_DUMMY4WORD(LDWS_CBMSG)
}
LDWS_CBMSG;


/**
    LDWS callback funcion proto type
*/
typedef void (*LDWS_CBMSG_FP)(LDWS_CBMSG uiMsgID, void *Data);

/**
    Get LDWS library version number.

    @return the version number string.
*/
extern CHAR *LDWS_GetVerInfo(void);
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
extern INT32 LDWS_Init(MEM_RANGE *buf, UINT32 ProcessRate);

/**
     Uninit LDWS engine.

     This function is used to release LDWS Detect engine.
     @return
         - @b LDWS_STA_OK:       Success.
         - @b LDWS_STA_ERROR:    Some error occurred.
*/
extern INT32 LDWS_UnInit(void);

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
extern INT32 LDWS_Process(void);

/**
     Calculate LDWS detection required buffer size.

     @return the required buffer size.
*/
extern UINT32 LDWS_CalcBuffSize(void);


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
extern void   LDWS_RegisterCB(LDWS_CBMSG_FP CB);

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
extern void   LDWS_Lock(BOOL bLock);

//2014.03.06 add function declaration
extern UINT32 LDWS_CheckInitFlag(void);
extern void LDWS_SetInitFlag(UINT32 Flag);
extern LDWS_INFO LDWS_GetResult(void);
//extern void LDWS_RegisterCB(LDWS_CBMSG_FP CB);
//extern void LDWS_Lock(BOOL bLock);


#endif

