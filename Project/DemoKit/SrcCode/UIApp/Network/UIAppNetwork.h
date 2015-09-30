#ifndef _UIAPPNET_H_
#define _UIAPPNET_H_

#include <cyg/io/eth/rltk/819x/wlan/ieee802_mib.h>
#include "wifi_utility.h"

#define TXT_WIFI_AP_MODE  "AP mode"
#define TXT_WIFI_ST_MODE  "station mode"
#define TXT_WIFI_CONNECT  "Conntecting"
#if (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_)
#define TXT_WIFI          "wifi"
#define TXT_WIFI_SET      "wifi Setting"
#define TXT_WIFI_START    "wifi start"
#else
#define TXT_WIFI          "LAN"
#define TXT_WIFI_SET      "LAN Setting"
#define TXT_WIFI_START    "LAN start"

#endif


typedef enum
{
    NET_AP_MODE = 0,
    NET_STATION_MODE,
    NET_MODE_SETTING_MAX
}NET_MODE_SETTING;


typedef enum
{
    NET_AUTH_NONE = 0,
    NET_AUTH_WPA2 ,
    NET_AUTH_SETTING_MAX
}NET_AUTH_SETTING;

extern INT32 UINet_WifiInit(UINT32 mode);
extern INT32 UINet_LanInit(UINT32 mode);
extern INT32 UINet_HFSInit(void);
extern INT32 UINet_WifiUnInit(UINT32 mode);
extern INT32 UINet_LanUnInit(UINT32 mode);
extern INT32 UINet_HFSUnInit(void);
extern char* UINet_GetSSID(void);
extern char* UINet_GetPASSPHRASE(void);
extern char* UINet_GetIP(void);
extern char* UINet_GetMAC(void);
extern char* UINet_GetConnectedMAC(void);
extern void UINet_SetAuthType(NET_AUTH_SETTING authtype);
extern void UINet_SetSSID(char *ssid,UINT32 len);
extern void UINet_SetPASSPHRASE(char *pwd,UINT32 len);
extern void UINet_SetChannel(UINT32 channel);
extern UINT32 UINet_GetChannel(void);
extern void UINet_RemoveUser(void);
extern void UINet_AddACLTable(void);
extern void UINet_ClearACLTable(void);
extern void UINet_SiteSurvey(char *ifname,SS_STATUS_Tp pStatus);
extern void UINet_DumpAllSSID(SS_STATUS_Tp pStatus);

#endif //_UIAPPNET_H_
