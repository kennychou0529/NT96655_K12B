/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2009.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: DIS Library Module                                                  *
* Description:                                                             *
* Author: Connie Yu                                                         *
****************************************************************************/

/**
    DIS lib

    Sample module detailed description.

    @file       Dis_alg.h
    @ingroup    mILibDIS
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/** \addtogroup DISLib */
//@{

#ifndef _DIS_COMMON_H_
#define _DIS_COMMON_H_

//#include "dis_lib.h"

#define MDIS_PRV_MAX_SIZE    2304* 1296 * 2 * 2 ///< MAXSIZE*1.44*FMT_422*2
#define DISLIB_VECTOR_NORM      12      ///< output vector norm factor(2^12)

typedef struct
{
    UINT32  Hsz;
    UINT32  Vsz;
} DIS_SIZE;

/**
    DIS Motion Vector.

    Structure of directional motion vector elements.
*/
//@{
typedef struct _DISALG_MOTION_VECTOR
{
    INT32 iX; ///< x component
    INT32 iY; ///< y component
} DISALG_MOTION_VECTOR;
//@}

/**
    DIS ALG Entire Configuration

    Structure of DIS ALG parameters when one wants to configure this module.
*/
//@{
typedef struct _DISALG_PARAM_STRUCT
{
    //DIS_ENGINE_OPERATION DIS_operation; ///< DIS engine operation
    UINT32 InSizeH;                     ///< input horizontal pixels
    UINT32 InSizeV;                     ///< input vertical pixels
    UINT32 InLineOfs;                   ///< input line offset
    UINT32 InAdd0;                      ///< input starting address 0
    UINT32 InAdd1;                      ///< input starting address 1
    UINT32 InAdd2;                      ///< input starting address 2
    UINT16 frame_cnt;                   ///< frame count
} DISALG_PARAM;
//@}

/**
    DIS ALG result Vector information

    Structure of DIS ALG result vector information.
*/
//@{
typedef struct _DISALG_VECTOR_STRUCT
{
    DISALG_MOTION_VECTOR vector;///< result vector
    UINT16 frame_cnt;           ///< frame count
    UINT32 score;               ///< 1: useable, 0: not sure
} DISALG_VECTOR;
//@}

extern CHAR *Dis_GetVerInfo(void);
extern CHAR *Dis_GetBuildDate(void);

extern void dis_initialize(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void dis_process(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void dis_end(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern UINT32 dis_GetPrvMaxBuffer(void);
extern void dis_setDisInfor(DISALG_PARAM *disInfo);
extern void dis_processOpen(void);
extern void dis_processClose(void);
extern void dis_processLock(BOOL lock);
extern void dis_eventStart(void);
extern void dis_eventPause(void);
extern void dis_accumUpdateProcess(void);
extern UINT16 dis_getDisViewRatio(void);
extern void dis_setDisViewRatio(UINT16 ratio);
extern DISALG_MOTION_VECTOR dis_getIdeCorrVec(void);
extern ER dis_getFrameCorrVec(DISALG_VECTOR *Corr, UINT16 frame_cnt);
//extern DIS_PROC_EVENT dis_getProcEvent(void);
extern UINT32 dis_getTriggerStatus(void);

#endif

