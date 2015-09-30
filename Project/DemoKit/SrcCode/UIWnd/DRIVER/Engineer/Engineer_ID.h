#include "Type.h"

#ifndef _ENGINEER_ID_H
#define _ENGINEER_ID_H

extern void Engineer_InstallID(void) _SECTION(".kercfg_text");

extern UINT32 _SECTION(".kercfg_data") FLG_ID_ENGINEER;

#endif //_ENGINEER_ID_H