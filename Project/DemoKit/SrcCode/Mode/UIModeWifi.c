#if defined(__ECOS)

#include "UIMenuWndWiFiModuleLink.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "SysMain.h"


void UI_OpenWifiWnd(void)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)!=System_GetState(SYS_STATE_PREVSUBMODE))
    {
        Ux_OpenWindow(&UIMenuWndWiFiModuleLinkCtrl,0);
    }
}

void UI_CloseWifiWnd(void)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)!=System_GetState(SYS_STATE_NEXTSUBMODE))
    {
        Ux_CloseWindow(&UIMenuWndWiFiModuleLinkCtrl,0);
    }
}

#else
void UI_OpenWifiWnd(void)
{
}
void UI_CloseWifiWnd(void)
{
}
#endif
