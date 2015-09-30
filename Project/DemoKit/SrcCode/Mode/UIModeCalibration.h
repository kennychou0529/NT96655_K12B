/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIMode.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Mode Functions
                This file is the user interface ( for interchange flow control).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UIMODECALIBRATION_H
#define _UIMODECALIBRATION_H

#include "SysMain.h"

#define ENG_MODE_FILE  "A:\\engmode"

extern SYS_MODE gModeCalibration;
extern int PRIMARY_MODE_CALIBRATION;   ///< Calibration

extern BOOL ModeCalibration_CheckEng(void);


#endif //_UIMODECALIBRATION_H
