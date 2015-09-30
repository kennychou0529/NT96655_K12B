#ifndef CYGONCE_NET_HFS_H
#define CYGONCE_NET_HFS_H
/* =================================================================
 *
 *      hfs.h
 *
 *      A simple http file server.
 *
 * =================================================================
 */

#include <pkgconf/system.h>
#include <pkgconf/isoinfra.h>
#include <cyg/hal/hal_tables.h>
#include <stdio.h>

typedef unsigned long       HFS_U32;
typedef unsigned char       HFS_U8;


#define CYG_HFS_STATUS_CLIENT_REQUEST           0   // client has request comimg in
#define CYG_HFS_STATUS_SERVER_RESPONSE_START    1   // server send response data start
#define CYG_HFS_STATUS_SERVER_RESPONSE_END      2   // server send response data end
#define CYG_HFS_STATUS_CLIENT_DISCONNECT        3   // client disconnect.

#define CYG_HFS_CB_GETDATA_RETURN_ERROR        -1   // has error
#define CYG_HFS_CB_GETDATA_RETURN_OK           0    // ok get all data
#define CYG_HFS_CB_GETDATA_RETURN_CONTINUE     1    // has more data need to get

#define CYG_HFS_UPLOAD_OK                      0    // upload file ok
#define CYG_HFS_UPLOAD_FAIL_FILE_EXIST         -1   // upload file fail because of file exist
#define CYG_HFS_UPLOAD_FAIL_RECEIVE_ERROR      -2   // receive data has some error
#define CYG_HFS_UPLOAD_FAIL_WRITE_ERROR        -3   // write file has some error
#define CYG_HFS_UPLOAD_FAIL_FILENAME_EMPTY     -4   // file name is emtpy

#define CYG_HFS_DELETE_OK                      0    // delete file ok
#define CYG_HFS_DELETE_FAIL                    -1   // delete file fail


#define CYG_HFS_ROOT_DIR_MAXLEN                64   // root dir max path length
#define CYG_HFS_KEY_MAXLEN                     64   // key max length
#define CYG_HFS_NAME_MAXLEN                    32   // user name max length
#define CYG_HFS_PWD_MAXLEN                     32   // user passwd max length
#define CYG_HFS_REQUEST_PATH_MAXLEN            128  // request url path max length
#define CYG_HFS_MIMETYPE_MAXLEN                40   // mime type max length
#define CYG_HFS_USER_QUERY_MAXLEN              24   // client query string max length



typedef void cyg_hfs_notify(int status);
typedef int  cyg_hfs_getCustomData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
typedef int  cyg_hfs_check_password(const char *username, const char *password, char *key,char* questionstr);
typedef int  cyg_hfs_client_query(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
typedef int  cyg_hfs_upload_result_cb(int result, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);
typedef int  cyg_hfs_delete_result_cb(int result, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);


typedef struct
{
    cyg_hfs_notify          *notify;        // notify the status
    cyg_hfs_getCustomData   *getCustomData; // get custom data
    cyg_hfs_check_password  *check_pwd;     // check password callback function
    int                     portNum;        // server port number
    int                     threadPriority; // server thread priority
    char                    rootdir[CYG_HFS_ROOT_DIR_MAXLEN + 1];   /* root of the visible filesystem */
    char                    key[CYG_HFS_KEY_MAXLEN];
    int                     sockbufSize;    // socket buffer size
    int                     tos;            // type of service
    char                    clientQueryStr[CYG_HFS_USER_QUERY_MAXLEN]; // client query string
    cyg_hfs_client_query    *clientQueryCb; // client query callback function
    int                     timeoutCnt;     // timeout counter for send & receive , time base is 0
    cyg_hfs_upload_result_cb *uploadResultCb;
    cyg_hfs_delete_result_cb *deleteResultCb;
} cyg_hfs_open_obj;

__externC void cyg_hfs_open(cyg_hfs_open_obj*  pObj);         // Start hfs server

__externC void cyg_hfs_close(void);                           // Stop hfs server



/* ----------------------------------------------------------------- */
#endif /* CYGONCE_NET_HFS_H                                          */
/* end of hfs.h                                                      */

