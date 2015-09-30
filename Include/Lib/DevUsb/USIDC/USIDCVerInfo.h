/**
    Header file of USIDC version and build date information

    @file       USIDCVerInfo.h
    @ingroup    mIUSIDC
    @version    V1.92.000.G
    @author     Lily Kao
    @date       2010/12/17

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _USIDCVERINFO_H
#define _USIDCVERINFO_H

#include "Type.h"

/**
    @addtogroup mIUSIDC
*/
//@{

/**
    Get USIDC version information

    Get USIDC version information. Return pointer of 9 characters (include
    null terminated character). Example: 2.00.002

    @return Pointer of 8 characters of version information + null terminated character.
*/
extern CHAR *Sidc_GetVerInfo(void);

/**
    Get USIDC build date and time

    Get USIDC build date and time. Return pointer of 22 characters (include
    null terminated character). Example: Mar 19 2009, 18:29:28

    @return Pointer of 21 characters of build date and time + null terminated character.
*/
extern CHAR *Sidc_GetBuildDate(void);


//@}
#endif
