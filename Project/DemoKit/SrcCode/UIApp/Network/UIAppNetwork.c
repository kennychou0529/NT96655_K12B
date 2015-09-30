
#include "UIInfo.h"


#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_)
#include <cyg/io/eth/rltk/819x/wlan/wifi_api.h>
#endif

#ifdef __ECOS
#include "net_api.h"
#include <cyg/fileio/fileio.h>
#include "ecos_FileSys.h"
#include <cyg/fs/nvtfs.h>
#include <cyg/hfs/hfs.h>

#include "DhcpNvt.h"
#include <net/dhcpelios/dhcpelios.h>

#include "wps_api.h"
#include "UIAppNetwork.h"
#include "WifiAppCmd.h"
#include "WifiAppXML.h"
#endif
#include "Timer.h"
#include "NVTToolCommand.h"
#include "UIBackgroundObj.h"
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UINet
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#ifdef __ECOS
#define MOUNT_FS_ROOT        "/sdcard"
char gNvtDhcpHostNameCli[DHCP_HOST_NAME_MAX_LEN] = "nvtsys01cli";
char gNvtDhcpHostNameSrv[DHCP_HOST_NAME_MAX_LEN] = "nvtsys01srv";
char gMacAddr[6] = {0,0,0,0x81,0x89,0xe5};
UINT32 gAuthType = NET_AUTH_WPA2;
char gSSID[WSC_MAX_SSID_LEN] = "NVT_CARDV";
char gPASSPHRASE[MAX_WEP_KEY_LEN] = "12345678";
char gCurrIP[NET_IP_MAX_LEN] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
UINT32 gChannel = 0;//default 0:auto
char gLastMacAddr[NET_IP_MAX_LEN] = {0};
char gRemoveMacAddr[NET_IP_MAX_LEN] = {0};
char currentMacAddr[NET_IP_MAX_LEN] = {0};
#define EXAM_NET_IP_ETH0            "192.168.0.3"
#define EXAM_NET_NETMASK_ETH0       "255.255.255.0"
#define EXAM_NET_BRAODCAST_ETH0     "192.168.0.255"
#define EXAM_NET_GATEWAY_ETH0       "192.168.0.1"
#define EXAM_NET_SRVIP_ETH0         "192.168.0.1"
#define EXAM_NET_LEASE_START_ETH0   "192.168.0.12"
#define EXAM_NET_LEASE_END_ETH0     "192.168.0.32"
#define EXAM_NET_LEASE_DNS_ETH0     "0.0.0.0"
#define EXAM_NET_LEASE_WINS_ETH0    "0.0.0.0"

#define EXAM_NET_IP_ETH1            "192.168.1.3"
#define EXAM_NET_NETMASK_ETH1       "255.255.255.0"
#define EXAM_NET_BRAODCAST_ETH1     "192.168.1.255"
#define EXAM_NET_GATEWAY_ETH1       "192.168.1.1"
#define EXAM_NET_SRVIP_ETH1         "192.168.1.1"

#define EXAM_NET_IP_WLAN0            "192.168.1.254"
#define EXAM_NET_NETMASK_WLAN0       "255.255.255.0"
#define EXAM_NET_BRAODCAST_WLAN0     "192.168.1.255"
#define EXAM_NET_GATEWAY_WLAN0       "192.168.1.254"
#define EXAM_NET_SRVIP_WLAN0         "192.168.1.254"
#define EXAM_NET_LEASE_START_WLAN0   "192.168.1.33"
#define EXAM_NET_LEASE_END_WLAN0     "192.168.1.52"
#define EXAM_NET_LEASE_DNS_WLAN0     "0.0.0.0"
#define EXAM_NET_LEASE_WINS_WLAN0    "0.0.0.0"

#define WIFI_SSID_STR                "ssid="
#define WIFI_PASSPHRASE_STR          "passphrase="
#define WIFI_CHANNEL_STR             "channel="
extern UIMenuStoreInfo  currentInfo;

#if (WIFI_FTP_FUNC==ENABLE)
void start_ftpserver(void);
void stop_ftpserver(void);
#endif

char gNetIntf[3][6] =
{
    'e','t','h','0',0,0,
    'e','t','h','1',0,0,
    'w','l','a','n','0',0,
};
cyg_uint32 xExamHfs_getCurrTime(void)
{
    return timer_getCurrentCount(timer_getSysTimerID());
}

static void UINet_HFSNotifyStatus(int status)
{
    switch (status)
    {
        // HTTP client has request coming in
        case CYG_HFS_STATUS_CLIENT_REQUEST:
             //DBG_IND("client request %05d ms\r\n",xExamHfs_getCurrTime()/1000);
             break;
        // HTTP server send data start
        case CYG_HFS_STATUS_SERVER_RESPONSE_START:
             //DBG_IND("send data start, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
             break;
        // HTTP server send data start
        case CYG_HFS_STATUS_SERVER_RESPONSE_END:
             //DBG_IND("send data end, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
             break;
        // HTTP client disconnect
        case CYG_HFS_STATUS_CLIENT_DISCONNECT:
             //DBG_IND("client disconnect, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
             break;

    }
}
int UINet_HFSUploadResultCb(int result, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    XML_DefaultFormat(WIFIAPP_CMD_UPLOAD_FILE,result,bufAddr,(cyg_uint32 *)bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int UINet_HFSDeleteResultCb(int result, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,result,bufAddr,(cyg_uint32 *)bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

void Net_DhcpCliCb(NET_DHCP_CB_TYPE cbType, unsigned int p1, unsigned int p2, unsigned int p3)
{
    DBG_IND("::type=%d,P=%d, %d, %d\r\n",cbType, p1, p2, p3);
    if (NET_DHCP_CB_TYPE_CLI_REQ_CNT == cbType)
    {
        //post an event to UI-task show (cycle,count) that client request an IP
    }
    else if (NET_DHCP_CB_TYPE_CLI_REQ_RESULT == cbType)
    {
        if (p1)
        {
            DBG_DUMP("DHCP Client IP = 0x%x\r\n",p1);
            //post an event to UI-task to call,Dhcp_Client_Start_Ok(p1) for the first time.
            //post an event to UI-task to show Got-IP
        }
        else
        {
            DBG_DUMP("DHCP Client IP Fail\r\n");
            //post an event to UI-task to Close DHCP Client,Dhcp_Client_Stop(), and show Get-IP fail
        }
    }
    else if (NET_DHCP_CB_TYPE_MAX == cbType)
    {
        OS_DumpKernelResStatus();
    }
}

//////////////////////////////////////////////////////////////////////////
char* UINet_GetConnectedMAC(void)
{
    return currentMacAddr;
}
void UINet_SetAuthType(NET_AUTH_SETTING authtype)
{
    gAuthType = authtype;
}
char* UINet_GetSSID(void)
{
    return gSSID;
}
char* UINet_GetPASSPHRASE(void)
{
    return gPASSPHRASE;
}
char* UINet_GetIP(void)
{
    return gCurrIP;
}
char* UINet_GetMAC(void)
{
    return gMacAddr;
}
void UINet_SetSSID(char *ssid,UINT32 len)
{
    if(len>WSC_MAX_SSID_LEN)
    {
        DBG_ERR("max len %d\r\n",WSC_MAX_SSID_LEN);
        len = WSC_MAX_SSID_LEN;
    }
    memset(gSSID,'\0',WSC_MAX_SSID_LEN);
    sprintf(gSSID,ssid,len);
    DBG_IND("%s\r\n",gSSID);
    strncpy(currentInfo.strSSID, UINet_GetSSID(), (WSC_MAX_SSID_LEN -1)); // Save Wi-Fi SSID.
    currentInfo.strSSID[strlen(currentInfo.strSSID)] = '\0';
}
void UINet_SetPASSPHRASE(char *pwd,UINT32 len)
{

    if(len>MAX_WEP_KEY_LEN)
    {
        DBG_ERR("max len %d\r\n",MAX_WEP_KEY_LEN);
        len = MAX_WEP_KEY_LEN;
    }
    memset(gPASSPHRASE,'\0',MAX_WEP_KEY_LEN);
    strncpy(gPASSPHRASE,pwd,len);
    DBG_IND("%s\r\n",gPASSPHRASE);

    strncpy(currentInfo.strPASSPHRASE, UINet_GetPASSPHRASE(), (MAX_WEP_KEY_LEN -1)); // Save Wi-Fi PASSWORD.
    currentInfo.strPASSPHRASE[strlen(currentInfo.strPASSPHRASE)] = '\0';
}
//0 for auto channel, 1 -14 for 11b/11g,36 -165 for 11a
void UINet_SetChannel(UINT32 channel)
{
    gChannel=channel;
}
UINT32 UINet_GetChannel(void)
{
    return gChannel;
}

#if(_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_)

static int IS_MCAST(unsigned char *da)
{
    if ((*da) & 0x01)
        return 1;
    else
        return 0;
}

void UINet_RemoveUser(void)
{
    UINT32 bNewUser = FALSE;

    CHKPNT;
    //if 2nd user connect,diconnect 1st user
    if(strncmp(currentMacAddr,gLastMacAddr,strlen(currentMacAddr))!=0 )
    {
        //notify current connected socket
        //new connect not get IP and socket not create
        //cannot disconnet immediate,suggest after 500 ms
        WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_REMOVE_BY_USER);
        bNewUser = TRUE;
    }

    Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 1,bNewUser);
}
void UINet_AddACLTable(void)
{
    if(strncmp(currentMacAddr,gLastMacAddr,strlen(currentMacAddr))!=0 )
    {
        if(strlen(gRemoveMacAddr)!=0)
        {
            DBG_ERR("%s not remove from ACL !!\r\n",gRemoveMacAddr);
        }

        strncpy(gRemoveMacAddr,gLastMacAddr,strlen(gLastMacAddr));
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "add_acl_table", gRemoveMacAddr, NULL_STR);
        strncpy(gLastMacAddr,currentMacAddr,strlen(currentMacAddr));
        DBG_IND("********** gLastMacAddr    %s\r\n",gLastMacAddr);
        DBG_IND("********** gRemoveMacAddr  %s\r\n",gRemoveMacAddr);

        BKG_PostEvent(NVTEVT_BKW_WIFI_CLEARACL);

    }
}
void UINet_ClearACLTable(void)
{
    if(strlen(gRemoveMacAddr))
    {
        DBG_IND("**********clr gRemoveMacAddr  %s\r\n",gRemoveMacAddr);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "rm_acl_table", gRemoveMacAddr, NULL_STR);
        memset(gRemoveMacAddr,0,strlen(gRemoveMacAddr));
    }
}


void UINet_StationStatus_CB(char *pIntfName, char *pMacAddr, int status)
{
    //#NT#2013/12/06#KS Hung -begin
    //#NT#Post AUTHORIZED_OK event
    if (status == WIFI_STA_STATUS_ASSOCIATED) {

    DBG_IND("ASSOCIATED %d ms\r\n",Perf_GetCurrent()/1000);

        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is associated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_REASSOCIATED) {
        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is reassociated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DISASSOCIATED) {
        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));

        Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
    }
    else if (status == WIFI_STA_STATUS_DEAUTHENTICATED) {
        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is deauthenticated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));

        Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
    }
    else if (status == WIFI_STA_STATUS_PORT_AUTHORIZED) {

        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is port authorized\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));

        {
            sprintf(currentMacAddr,"%02x%02x%02x%02x%02x%02x",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
            DBG_IND("********** currentMacAddr  %s\r\n",currentMacAddr);

            if(strlen(gLastMacAddr)==0)
            {
                strncpy(gLastMacAddr,currentMacAddr,strlen(currentMacAddr));
                DBG_IND("********** gLastMacAddr    %s\r\n",gLastMacAddr);
            }

            Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 1,0);
        }

    }
    else if (status == WIFI_AP_READY) {
        DBG_IND("AP ready\r\n");
        DBG_ERR("AP ready %d ms\r\n",Perf_GetCurrent()/1000);

    }
    else
        printf("unknown status %d\r\n",status);
    //#NT#2013/12/06#KS Hung -end
}


void UINet_Link2APStatus_CB(char *pIntfName, int status)
{
    if (status == WIFI_LINK_STATUS_CONNECTED) {
        DBG_IND("%s: Connected with AP\n", pIntfName);
        #if 0 // Unused code. Because new Wi-Fi driver has fixed this issue.
        RunSystemCmd(NULL_FILE, "ifconfig", "wlan0", gCurrIP, NULL_STR, NULL_STR);  // Issue ARP Request to router.
        #endif
        Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 0);
    }
    else if (status == WIFI_LINK_STATUS_DISCONNECTED) {
        DBG_IND("%s: Disconnected with AP\n", pIntfName);
    }
    else
        DBG_IND("unknown status\n");
}

void UINet_DhcpCb(DHCPD_CLI_STS cliSts, dhcp_assign_ip_info *ipInfo, dhcpd_lease_tbl_loc *pTbl)
{
    switch(cliSts)
    {
    case DHCPD_CLI_STS_RELEASE_IP:
        break;
    case DHCPD_CLI_STS_REQUEST_IP:
        DBG_IND("REQUEST_IP %d ms\r\n",Perf_GetCurrent()/1000);
        break;
    case DHCPD_CLI_STS_DECLINE_IP:
        break;
    case DHCPD_CLI_STS_SRVSTART:
        break;
    case DHCPD_CLI_STS_SRVREADY:
        break;
    default:
        break;
    }
}

BOOL UINet_SetFixLeaseInfo(UINT32 sec,UINT32 cnt)
{
    NET_DHCP_LEASE_CONF leaseInfo;

    memset((void *)&leaseInfo, 0, sizeof(NET_DHCP_LEASE_CONF));//
    memcpy((void *)leaseInfo.ip_pool_start, EXAM_NET_LEASE_START_WLAN0, strlen(EXAM_NET_LEASE_START_WLAN0));
    memcpy((void *)leaseInfo.ip_pool_end, EXAM_NET_LEASE_END_WLAN0, strlen(EXAM_NET_LEASE_END_WLAN0));
    memcpy((void *)leaseInfo.ip_dns, EXAM_NET_LEASE_DNS_WLAN0, strlen(EXAM_NET_LEASE_DNS_WLAN0));
    memcpy((void *)leaseInfo.ip_wins, EXAM_NET_LEASE_WINS_WLAN0, strlen(EXAM_NET_LEASE_WINS_WLAN0));
    memcpy((void *)leaseInfo.ip_server, EXAM_NET_SRVIP_WLAN0, strlen(EXAM_NET_SRVIP_WLAN0));
    memcpy((void *)leaseInfo.ip_gateway, EXAM_NET_GATEWAY_WLAN0, strlen(EXAM_NET_GATEWAY_WLAN0));
    memcpy((void *)leaseInfo.ip_subnetmask, EXAM_NET_NETMASK_WLAN0, strlen(EXAM_NET_NETMASK_WLAN0));
    memcpy((void *)leaseInfo.ip_broadcast, EXAM_NET_BRAODCAST_WLAN0, strlen(EXAM_NET_BRAODCAST_WLAN0));
    leaseInfo.lease_time = (unsigned int)sec;
    leaseInfo.max_lease_cnt = (unsigned int)cnt;
    Dhcp_Conf_Lease(&leaseInfo);

    return TRUE;
}

static CHAR    wifissid_str[30] = {0};

INT32 UINet_WifiInit(UINT32 mode)
{
    UINT32 result=0;
    static BOOL bInit = FALSE;
    char tmpBuf[40];

    DBG_IND("[SetupObj] set wifi %d \r\n",mode);

    if(!bInit)
    {
        result = wifi_init_wlan0_netdev(0);
        if (result!=0 )
        {
            DBG_ERR("wifi init fail %d\r\n",result);
            return E_SYS;
        }
        bInit = TRUE;
    }

    if (is_interface_up(&gNetIntf[2][0]))
    {
        DBG_IND("Down interface=%s\r\n",&gNetIntf[2][0]);
        interface_down(&gNetIntf[2][0]);
    }

    if(mode == NET_AP_MODE) //////////////////////ap mode //////////////////////////////
    {
        //#NT#2013/12/06#KS Hung -begin
        //register sta status cb
        register_wifi_sta_status_cb("wlan0", UINet_StationStatus_CB); // AP mode
        //#NT#2013/12/06#KS Hung end

        // mac address setting
        if (wifi_get_wlan0_efuse_mac(gMacAddr) < 0)
        {
            printf("wifi_get_wlan0_efuse_mac() fail. Use hardcoded mac.\n");
            set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
        }
        else
        {
            unsigned char zero[] = {0, 0, 0, 0, 0, 0};

            printf("wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\n",
            gMacAddr[0], gMacAddr[1], gMacAddr[2],
            gMacAddr[3], gMacAddr[4], gMacAddr[5]);

            //add mtg   change  wifi   ssid=================
            sprintf(wifissid_str,"DECAI_%02x:%02x:%02x:%02x:%02x:%02x",gMacAddr[0], gMacAddr[1], gMacAddr[2],
            gMacAddr[3], gMacAddr[4], gMacAddr[5]);
            UINet_SetSSID(wifissid_str,strlen(wifissid_str));	
            //add  mtg  end ====================
            //sanity check
            if (!memcmp(gMacAddr, zero, MACADDRLEN) || IS_MCAST((unsigned char *)gMacAddr))
            {
                printf("efuse mac format is invalid. Use hardcoded mac.\n");
                set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
            }
            else
            {
                set_mac_address("wlan0", gMacAddr);
            }
        }

        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "opmode=16", NULL_STR);
        if(currentInfo.strSSID[0] == 0)
        {
#if (MAC_APPEN_SSID==ENABLE)
        sprintf(tmpBuf,"%s%s%02x%02x%02x%02x%02x%02x",WIFI_SSID_STR,gSSID, gMacAddr[0], gMacAddr[1], gMacAddr[2],
            gMacAddr[3], gMacAddr[4], gMacAddr[5]);
#else
        sprintf(tmpBuf,"%s%s",WIFI_SSID_STR,gSSID);
#endif
        }
        else
        {
            sprintf(tmpBuf,"%s%s", WIFI_SSID_STR, currentInfo.strSSID);
        }
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpBuf, NULL_STR); //ssid=NVT_SYSTEM
        sprintf(tmpBuf,"%s%d",WIFI_CHANNEL_STR,gChannel);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpBuf, NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "band=11", NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "ampdu=1", NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "use40M=1", NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "2ndchoffset=1", NULL_STR); // 1:below, 2:above
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "shortGI20M=1", NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "shortGI40M=1", NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "autoch_ss_to=100", NULL_STR);

        //set WPA2
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=2", NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=2", NULL_STR);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa2_cipher=8", NULL_STR);
        sprintf(tmpBuf,"%s%s",WIFI_PASSPHRASE_STR,gPASSPHRASE);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpBuf, NULL_STR); //passphrase=12345678

        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "aclmode=2", NULL_STR);

        DBG_IND("Default IP=%s, wpa2=%d\r\n",EXAM_NET_IP_WLAN0, 1);
        memcpy((void *)gCurrIP, EXAM_NET_IP_WLAN0, NET_IP_MAX_LEN);

        interface_config(&gNetIntf[2][0], gCurrIP, "255.255.255.0");
        DBG_IND(" setip=%s -\r\n", gCurrIP);

        //set DHCP
        Dhcp_Set_Interface(&gNetIntf[2][0]);
        dhcpd_reg_cli_attach_cb_func(UINet_DhcpCb);
        #if(IP_LIMITE==ENABLE)
        UINet_SetFixLeaseInfo(1200,3);  //20 min, 3 IP only
        #endif
        if (NET_DHCP_RET_OK != Dhcp_Server_Start(gNvtDhcpHostNameSrv))
        {
            DBG_ERR("xExamDhcpNvt_OpenSrv fail\r\n");
            return E_SYS;
        }

    }
    else if(mode == NET_STATION_MODE) //////////////////////station mode //////////////////////////////
    {

        //register link status cb
        register_wifi_link_status_cb("wlan0", UINet_Link2APStatus_CB);

        //xExamDhcpNvt_WifiUtil_sta();
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "opmode=8", NULL_STR);
        sprintf(tmpBuf,"%s%s",WIFI_SSID_STR,gSSID);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpBuf, NULL_STR);

        if(gAuthType ==NET_AUTH_NONE)
        {
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "802_1x=0", NULL_STR);
        }
        else
        {
            //xExamDhcpNvt_WifiUtil_wpa2();
            #if 1 // WPA2
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa2_cipher=8", NULL_STR);
            #else // WPA
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=1", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa_cipher=2", NULL_STR);
            #endif
            sprintf(tmpBuf,"%s%s",WIFI_PASSPHRASE_STR,gPASSPHRASE);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpBuf, NULL_STR);
        }
        //xExamDhcpNvt_WifiUtil_setMacAddr();
        // mac address setting
        if (wifi_get_wlan0_efuse_mac(gMacAddr) < 0)
        {
            printf("wifi_get_wlan0_efuse_mac() fail. Use hardcoded mac.\n");
            set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
        }
        else
        {
            unsigned char zero[] = {0, 0, 0, 0, 0, 0};

            printf("wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\n",
            gMacAddr[0], gMacAddr[1], gMacAddr[2],
            gMacAddr[3], gMacAddr[4], gMacAddr[5]);
            //sanity check
            if (!memcmp(gMacAddr, zero, MACADDRLEN) || IS_MCAST((unsigned char *)gMacAddr))
            {
                printf("efuse mac format is invalid. Use hardcoded mac.\n");
                set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
                printf(gMacAddr,"%c%c%c%c%c%c",0,0, 0,0x81,0x89,0xe5);
            printf("default wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\n",
            gMacAddr[0], gMacAddr[1], gMacAddr[2],
            gMacAddr[3], gMacAddr[4], gMacAddr[5]);

            }
            else
            {
                set_mac_address("wlan0", gMacAddr);
            }

        }

        //start DHCP client
        {
            unsigned int gotIP = 0;
            if (NET_DHCP_RET_OK != Dhcp_Client_Start(gNvtDhcpHostNameCli, Net_DhcpCliCb, TRUE))
            {
                DBG_ERR("dhcp cli fail\r\n");
                return E_SYS;
            }
            else
            {
                Dhcp_Client_Get_IP(&gotIP);
                sprintf(gCurrIP,"%d.%d.%d.%d",(gotIP&0xFF), (gotIP>>8)&0xFF,(gotIP>>16)&0xFF,(gotIP>>24)&0xFF );
                DBG_IND("^R:::: ip=0x%x %s\r\n",gotIP,gCurrIP);
            }
        }
    }
    else
    {
        result = E_SYS;
        DBG_ERR("not support mode %d\r\n",mode);
    }

    if(result==NET_DHCP_RET_OK)
    {
        result=UINet_HFSInit();
    }
    return E_OK;
}

INT32 UINet_WifiUnInit(UINT32 mode)
{
    UINT32 ret = 0;

    if(mode == NET_AP_MODE) //ap mode
        ret =Dhcp_Server_Stop(FALSE);
    else
        ret =Dhcp_Client_Stop();

    if(ret==E_OK)
    {
        ret =interface_down(&gNetIntf[2][0]);
    }
    else
    {
        DBG_ERR("stop DHCP fail %d\r\n",ret);
    }

    return ret;
}
#elif defined(__ECOS) && (_NETWORK_ == _NETWORK_SPI_EVB_ETHERNET_)
static NET_IP_PAIR gExamNetIP[NET_IP_PAIR_MAX_CNT] =
{
    //eth0
    {EXAM_NET_IP_ETH0,EXAM_NET_NETMASK_ETH0,EXAM_NET_BRAODCAST_ETH0,EXAM_NET_GATEWAY_ETH0,EXAM_NET_SRVIP_ETH0, 0},
    //eth1
    {EXAM_NET_IP_ETH1,EXAM_NET_NETMASK_ETH1,EXAM_NET_BRAODCAST_ETH1,EXAM_NET_GATEWAY_ETH1,EXAM_NET_SRVIP_ETH1, 1},
    //wlan0
    {EXAM_NET_IP_WLAN0,EXAM_NET_NETMASK_WLAN0,EXAM_NET_BRAODCAST_WLAN0,EXAM_NET_GATEWAY_WLAN0,EXAM_NET_SRVIP_WLAN0, 2}
};

INT32 UINet_LanInit(UINT32 mode)
{
    NET_DHCP_RET ret =0;

    DBG_IND("[SetupObj] set Lan %d\r\n",mode);

    if(mode == NET_AP_MODE) //ap mode
    {
        ret=Dhcp_Init_Network_With_IP(gExamNetIP, NET_IP_PAIR_MAX_CNT);
        if (NET_DHCP_RET_OK == ret)
        {
            ret=Dhcp_Server_Start(gNvtDhcpHostNameSrv);
            if(NET_DHCP_RET_OK !=ret )
            {
                DBG_ERR("AP fail %d\r\n",ret);
            }
        }
        else
        {
            DBG_ERR("eth0 fail %d\r\n",ret);
        }
    }
    else if(mode == NET_STATION_MODE) //station mode
    {
        //start DHCP client
        unsigned int gotIP = 0;
        ret=Dhcp_Client_Start(gNvtDhcpHostNameCli, Net_DhcpCb, TRUE);
        if (NET_DHCP_RET_OK != ret)
        {
            DBG_ERR("dhcp cli fail\r\n");
            return E_SYS;
        }
        else
        {
            Dhcp_Client_Get_IP(&gotIP);
            sprintf(gCurrIP,"%d.%d.%d.%d",(gotIP&0xFF), (gotIP>>8)&0xFF,(gotIP>>16)&0xFF,(gotIP>>24)&0xFF );
            DBG_IND("^R:::: ip=0x%x %s\r\n",gotIP,gCurrIP);

            #if 0
            debug_err(("%x \r\n",gotIP));
            debug_err(("%x \r\n",(gotIP&0xFF) ));
            debug_err(("%x \r\n",(gotIP>>8)&0xFF ));
            debug_err(("%x \r\n",(gotIP>>16)&0xFF ));
            debug_err(("%x \r\n",(gotIP>>24)&0xFF ));
            #endif

        }
    }
    else
    {
        ret = E_SYS;
        DBG_ERR("not support mode %d\r\n",mode);
    }

    if(ret==NET_DHCP_RET_OK)
    {
        ret=UINet_HFSInit();
    }

    return ret;
}



INT32 UINet_LanUnInit(UINT32 mode)
{
    UINT32 ret = 0;
    if(mode == NET_AP_MODE) //ap mode
        ret =Dhcp_Server_Stop(FALSE);
    else
        ret =Dhcp_Client_Stop();
    return ret;
}
#endif

INT32 UINet_HFSInit(void)
{
    int err;

    cyg_FileSys_RegisterCB();
    err = mount( "", MOUNT_FS_ROOT, "nvtfs" );
    if( err < 0 ) DBG_ERR("mount fs fail %d\r\n",err);
    //hfs start
    {
        cyg_hfs_open_obj  hfsObj={0};
        // register callback function of get XML data.
        hfsObj.getCustomData = WifiCmd_GetData;
        // noify some status of HTTP server
        hfsObj.notify = UINet_HFSNotifyStatus;
        // if need to check user & password
        hfsObj.check_pwd = NULL;//xExamHfs_CheckPasswd;
        // set port number
        hfsObj.portNum = 80;
        // set thread priority
        hfsObj.threadPriority = 20;
        // set socket buffer
        hfsObj.sockbufSize = 10240;//10K slow down get file speed
        // set query callback
        hfsObj.clientQueryCb = WifiCmd_getQueryData;
        hfsObj.uploadResultCb = UINet_HFSUploadResultCb;
        hfsObj.deleteResultCb = UINet_HFSDeleteResultCb;
        // set query string
        strcpy(hfsObj.clientQueryStr, "query_devinfo");
        // set HFS root dir path
        strcpy(hfsObj.rootdir, MOUNT_FS_ROOT);


        // start HFS
        cyg_hfs_open(&hfsObj);

    }

    WifiAppCmd_init();
    UserSocket_Open();
    #if (WIFI_FTP_FUNC==ENABLE)
    start_ftpserver();
    #endif

    return err;
}

INT32 UINet_HFSUnInit(void)
{
    cyg_hfs_close();
    #if (WIFI_FTP_FUNC==ENABLE)
    stop_ftpserver();
    #endif

    umount(MOUNT_FS_ROOT);

    return E_OK;
}

#if (WIFI_FTP_FUNC==ENABLE)
////////////////////////////////////FTP server ///////////////////////////////////

#include <network.h>
#include <net/ftpd.h>

#define STACK_SIZE (32*1024 + 0x1000)
static char stackFtpd[STACK_SIZE];
static cyg_thread thread_dataFtpd;
static cyg_handle_t thread_handleFtpd;

static ftp_server_t gFtpServer;

static int ftpCheckPasswd(const char * username, const char * passwd)
{
    if (strncmp(username, "ftpuser", strlen("ftpuser")) == 0)
    {
        if (strncmp(passwd, "12345", strlen("12345")) == 0)
        {
            // return 0 indicates user accepted
            return 0;
        }
    }
    else if (strncmp(username, "nvtuser", strlen("nvtuser")) == 0)
    {
        if (strncmp(passwd, "nvt123", strlen("nvt123")) == 0)
        {
            // return 0 indicates user accepted
            return 0;
        }
    }

    // return 1 indicates user rejected
    return 1;
}


static void ftpd_thread(cyg_addrword_t p)
{
    int sts;

    memset(&gFtpServer, 0, sizeof(gFtpServer));
    gFtpServer.allow_guest = 0;     // allow anonymous login
    gFtpServer.check_pwd = ftpCheckPasswd;
    gFtpServer.max_nr_of_clients = 4;
    strcpy(gFtpServer.chrootdir, MOUNT_FS_ROOT);
    sts = ftpd_server(&gFtpServer);
    if (sts != 0)
    {
        printf("%s: result is %d\r\n", __func__, sts);
    }
    printf("ftpd ended\r\n");

    cyg_thread_exit();
}

void start_ftpserver(void)
{
    // assume a file system is already mounted on /sdcard/ftp_home

/* Create a main thread, so we can run the scheduler and have time 'pass' */
    cyg_thread_create(10,                // Priority - just a number
                    ftpd_thread,          // entry
                    0,                 // entry parameter
                    "FTP test",        // Name
                    &stackFtpd[0],         // Stack
                    STACK_SIZE,        // Size
                    &thread_handleFtpd,    // Handle
                    &thread_dataFtpd       // Thread data structure
                    );
    cyg_thread_resume(thread_handleFtpd);  /* Start it */
}


static ftp_server_t gFtpServer;
static cyg_handle_t thread_handleFtpd;

void stop_ftpserver(void)
{
    // assume gFtpServer and thread_handleFtpd are those used to start ftp server

ftpd_server_stop(&gFtpServer);
cyg_thread_delete(thread_handleFtpd);
}
#endif

void UINet_print_bss_desc(struct bss_desc *pBss)
{
    char ssidbuf[40], tmp2Buf[20], tmp3Buf[20];

    //BSSID
    DBG_DUMP("%02x:%02x:%02x:%02x:%02x:%02x",
        pBss->bssid[0], pBss->bssid[1], pBss->bssid[2],
        pBss->bssid[3], pBss->bssid[4], pBss->bssid[5]);

    //channel
    DBG_DUMP("  %3d", pBss->channel);

    //band
    tmp2Buf[0] = 0;
    if (pBss->network & WIRELESS_11N) {
        strcat(tmp2Buf, "N");
    }
    if (pBss->network & WIRELESS_11G) {
        strcat(tmp2Buf, "G");
    }
    if (pBss->network & WIRELESS_11B) {
        strcat(tmp2Buf, "B");
    }
    DBG_DUMP("%5s", tmp2Buf);

    //rssi
    DBG_DUMP(" %3d", pBss->rssi);

    //signal quality
    DBG_DUMP("   %3d ", pBss->sq);

    //Security
    memset(tmp3Buf,0x00,sizeof(tmp3Buf));

    // refer to get_security_info() for details
    if ((pBss->capability & cPrivacy) == 0) {
        strcpy(tmp2Buf, "");
        strcpy(tmp3Buf, "None");
    }
    else {
        if (pBss->t_stamp[0] == 0) {
            strcpy(tmp2Buf, "");
            strcpy(tmp3Buf, "WEP");
        }
        else {
            int wpa_exist = 0, idx = 0;
            if (pBss->t_stamp[0] & 0x0000ffff) {
                idx = sprintf(tmp2Buf,"%s","WPA");
                if (((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x4)
                    idx += sprintf(tmp2Buf+idx,"%s","-PSK");
                else if(((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x2)
                    idx += sprintf(tmp2Buf+idx,"%s","-1X");
                wpa_exist = 1;

                if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x5)
                    sprintf(tmp3Buf,"%s","AES/TKIP");
                else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x4)
                    sprintf(tmp3Buf,"%s","AES");
                else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x1)
                    sprintf(tmp3Buf,"%s","TKIP");
            }
            if (pBss->t_stamp[0] & 0xffff0000) {
                if (wpa_exist)
                    idx += sprintf(tmp2Buf+idx,"%s","/");
                idx += sprintf(tmp2Buf+idx,"%s","WPA2");
                if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x4)
                    idx += sprintf(tmp2Buf+idx,"%s","-PSK");
                else if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x2)
                    idx += sprintf(tmp2Buf+idx,"%s","-1X");

                if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x5)
                    sprintf(tmp3Buf,"%s","AES/TKIP");
                else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x4)
                    sprintf(tmp3Buf,"%s","AES");
                else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x1)
                    sprintf(tmp3Buf,"%s","TKIP");
            }
        }
    }
    DBG_DUMP("%-10s ", tmp3Buf);
    DBG_DUMP("%-16s ", tmp2Buf);

    //SSID
    memcpy(ssidbuf, pBss->ssid, pBss->ssidlen);
    ssidbuf[pBss->ssidlen] = '\0';
    DBG_DUMP("%s\r\n", ssidbuf);
}

void UINet_DumpAllSSID(SS_STATUS_Tp pStatus)
{
    int i;
    struct bss_desc *pBss;

    if(!pStatus)
    {
        DBG_ERR("no pStatus \r\n");
        return ;
    }

    DBG_DUMP("pStatus->number %d\r\n",pStatus->number);

    //success
    DBG_DUMP(" #      BSSID          ch band rssi  sq  enc        auth             SSID\r\n");
    for (i=0; i<pStatus->number && pStatus->number!=0xff; i++) {
        pBss = &pStatus->bssdb[i];
        DBG_DUMP("%2d ", i);
        UINet_print_bss_desc(pBss);
    }
    //free(pStatus);
    pStatus = NULL;

}
void UINet_SiteSurvey(char *ifname,SS_STATUS_Tp pStatus)
{
    int status;
    unsigned char res;
    int wait_time;

    //below is copied from formWlSiteSurvey()
    {
        // issue scan request
        wait_time = 0;
        while (1) {
            // ==== modified by GANTOE for site survey 2008/12/26 ====
            switch(getWlSiteSurveyRequest(ifname, &status))
            {
                case -2:
                    DBG_ERR("Auto scan running!!please wait...\n");
                    goto ss_err;
                    break;
                case -1:
                    DBG_ERR("Site-survey request failed!\n");
                    goto ss_err;
                    break;
                default:
                    break;
            }

            if (status != 0) {  // not ready
                if (wait_time++ > 5) {
                    DBG_ERR("scan request timeout!\n");
                    goto ss_err;
                }
                //sleep(1);
                Delay_DelayMs(1000);
            }
            else
                break;
        }

        // wait until scan completely
        wait_time = 0;
        while (1) {
            res = 1;    // only request request status
            if (getWlSiteSurveyResult(ifname, (SS_STATUS_Tp)&res) < 0 ) {
                DBG_ERR("Read site-survey status failed!");
                goto ss_err;
            }
            if (res == 0xff) {   // in progress
                if (wait_time++ > 10)
                {
                    DBG_ERR("scan timeout!");
                    goto ss_err;
                }
                //sleep(1);
                Delay_DelayMs(1000);
            }
            else
                break;
        }
    }

    pStatus->number = 0; // request BSS DB
    if (getWlSiteSurveyResult(ifname, pStatus) < 0 ) {
        DBG_ERR("Read site-survey status failed!\n");
        //free(pStatus);
        pStatus = NULL;
        goto ss_err;
    }

    return;

ss_err:
    //failed
    //free(pStatus);
    pStatus = NULL;
}
#endif
