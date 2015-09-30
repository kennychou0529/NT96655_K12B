////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMain.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
//#include "UIMainWnd.h"
//UIInfo
#include "UIInfo.h"

int PRIMARY_MODE_MAIN = -1;      ///< Main

void ModeMain_Open(void);
void ModeMain_Close(void);

void ModeMain_Open(void)
{

    Ux_SetActiveApp(&UISetupObjCtrl);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

    //Ux_OpenWindow((VControl *)(&UIMainWndCtrl), 0);

}
void ModeMain_Close(void)
{

    //Ux_CloseWindowClear((VControl *)(&UIMainWndCtrl), 0);
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

}

SYS_MODE gModeMain =
{
    "MAIN",
    ModeMain_Open,
    ModeMain_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
