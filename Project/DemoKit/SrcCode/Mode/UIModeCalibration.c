////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeCalibration.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIInfo
#include "UIInfo.h"
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIFlow.h"
#endif

int PRIMARY_MODE_CALIBRATION = -1;      ///< calibration


void ModeCalibration_Open(void);
void ModeCalibration_Close(void);

BOOL ModeCalibration_CheckEng(void)
{
  BOOL ret = FALSE;
  FST_FILE filehdl = NULL;

     filehdl = FileSys_OpenFile(ENG_MODE_FILE,FST_OPEN_READ);
     if (filehdl!=NULL)
     {
        FileSys_CloseFile(filehdl);
        ret = TRUE;
     }

  return ret;
}

void ModeCalibration_Open(void)
{
      Input_ResetMask();

#if (UI_STYLE==UI_STYLE_DRIVER)
      Ux_OpenWindow((VControl *)(&UIMenuWndCalibrationCtrl), 0);
#endif

}
void ModeCalibration_Close(void)
{
#if (UI_STYLE==UI_STYLE_DRIVER)
      Ux_CloseWindowClear((VControl *)(&UIMenuWndCalibrationCtrl), 0);
#endif
}


SYS_MODE gModeCalibration =
{
    "CALIBRATION",
    ModeCalibration_Open,
    ModeCalibration_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
