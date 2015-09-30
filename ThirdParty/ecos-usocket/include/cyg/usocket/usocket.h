#ifndef CYGONCE_NET_USOCKET_H
#define CYGONCE_NET_USOCKET_H
/* =================================================================
 *
 *      usocket.h
 *
 *      A simple user socket server
 *
 * =================================================================
 */

#include <pkgconf/system.h>
#include <pkgconf/isoinfra.h>
#include <cyg/hal/hal_tables.h>
#include <stdio.h>

#define CYG_USOCKET_STATUS_CLIENT_CONNECT           0   // client connect.
#define CYG_USOCKET_STATUS_CLIENT_REQUEST           1   // client has request comimg in
#define CYG_USOCKET_STATUS_CLIENT_DISCONNECT        2   // client disconnect.


typedef int usocket_recv(char* addr, int size);
typedef void usocket_notify(int status,int parm);

typedef struct
{
    usocket_notify         *notify;        // notify the status
    usocket_recv           *recv;
    int                     portNum;        // socket port number
    int                     threadPriority; // socket thread priority
    int                     sockbufSize;// socket buffer size
    int                     client_socket;
} usocket_install_obj;


__externC void usocket_install( usocket_install_obj*  pObj);  // install some callback function & config some settings

__externC void usocket_open(void);     // open usocket

__externC void usocket_close(void);    // close usocket

__externC int usocket_send(char* addr, int* size);  // install some callback function & config some settings


/* ----------------------------------------------------------------- */
#endif /* CYGONCE_NET_USOCKET_H                                  */
/* end of usocket.h                                                   */

