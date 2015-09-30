#ifndef _WIFI_UTILITY_H
#define _WIFI_UTILITY_H

#define MAX_BSS_DESC    64

enum NETWORK_TYPE {
    WIRELESS_11B = 1,
    WIRELESS_11G = 2,
    WIRELESS_11A = 4,
    WIRELESS_11N = 8,
    WIRELESS_11AC = 64
};

typedef enum _Capability {
    cESS        = 0x01,
    cIBSS       = 0x02,
    cPollable       = 0x04,
    cPollReq        = 0x01,
    cPrivacy        = 0x10,
    cShortPreamble  = 0x20,
} Capability;

typedef struct _sitesurvey_status {
    unsigned char number;
    unsigned char pad[3];
    //BssDscr bssdb[MAX_BSS_DESC];
    struct bss_desc bssdb[MAX_BSS_DESC];
} SS_STATUS_T, *SS_STATUS_Tp;


extern int getWlSiteSurveyRequest(char *interface, int *pStatus);
extern int getWlSiteSurveyResult(char *interface, SS_STATUS_Tp pStatus );


#endif
