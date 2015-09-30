/**
    uITRON lock/unlock CPU related header file.

    @file       KerCPU.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _KERCPU_H
#define _KERCPU_H

#include "mIType.h"

/**
    @addtogroup mIKernelCore
*/

/**
    @name CPU utilization call back function
*/
//@{
typedef void (*CPU_CBUTIL)(UINT32);
typedef void (*CPU_CBSTCK)(UINT32);


extern void kdef_ucb(CPU_CBUTIL enterIdle, CPU_CBUTIL exitIdle);
extern void kdef_scb(CPU_CBSTCK stkChk);

//@}

//@{
extern ER       loc_cpu(void);
extern ER       unl_cpu(void);
//@}

#endif
