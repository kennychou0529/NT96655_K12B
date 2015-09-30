#ifndef _ENGINEERMODE_H
#define _ENGINEERMODE_H

#define ENG_MODE_FILE  "A:\\engmode"

extern volatile BOOL  g_bCalbirationOpened;

extern void Engineer_InstallID(void) _SECTION(".kercfg_text");
extern void EngineerMode_Open(void);

#endif
