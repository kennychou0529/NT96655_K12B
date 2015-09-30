#ifndef _WIFI_API_H
#define _WIFI_API_H

/*
 * pri: use to assign wifi thread priority, 0 - default priority
 * retrun: 0 success, -1 fail
 */
extern int wifi_init_wlan0_netdev(int pri);
extern int wifi_get_wlan0_efuse_mac(char *pMacAddr); //0 success, -1 fail

// for client mode
#define WIFI_LINK_STATUS_DISCONNECTED	0
#define WIFI_LINK_STATUS_CONNECTED		1
#define WIFI_LINK_STATUS_AUTH_FAIL		2
#define WIFI_LINK_STATUS_DISASSOCIATED	3
#define WIFI_LINK_STATUS_DEAUTHENTICATED	4
#define WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT	5
// callback function prototype for link status report
typedef void (wifi_link_status_cb_func_t)(char *pIntfName, int status);

//return 1: register success, 0: register fail.
extern int register_wifi_link_status_cb(char *pIntfName, wifi_link_status_cb_func_t *pFunc);

// for AP mode
#define WIFI_STA_STATUS_ASSOCIATED	1
#define WIFI_STA_STATUS_REASSOCIATED	2
#define WIFI_STA_STATUS_DISASSOCIATED	3
#define WIFI_STA_STATUS_DEAUTHENTICATED	4
#define WIFI_STA_STATUS_PORT_AUTHORIZED	5
#define WIFI_STA_STATUS_AUTH_FAIL		6
#define WIFI_STA_STATUS_EXPIRED		7
#define WIFI_AP_READY				8
// callback function prototype for station status report
typedef void (wifi_sta_status_cb_func_t)(char *pIntfName, char *pMacAddr, int status);

//return 1: register success, 0: register fail.
extern int register_wifi_sta_status_cb(char *pIntfName, wifi_sta_status_cb_func_t *pFunc);

// for p2p
#define WIFI_P2P_EVENT_BACK2DEV            1
#define WIFI_P2P_EVENT_START_DHCPD      2
#define WIFI_P2P_EVENT_START_DHCPC      3
// callback function prototype for p2p
typedef void (p2p_event_indicate_cb_func_t)(char *pIntfName, int status);

//return 1: register success, 0: register fail.
extern int register_p2p_event_indicate_cb(char *pIntfName, p2p_event_indicate_cb_func_t *pFunc);

extern int wifi_ignore_down_up(char *pIntfName, int val); // 1:success, 0:fail

#endif
