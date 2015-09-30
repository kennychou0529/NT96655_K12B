#ifdef __ECOS
#include <cyg/hfs/hfs.h>
#include "WifiCmdParser.h"
#include "FileSysTsk.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          WifiCmdParser
#define __DBGLVL__          2 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////



extern UINT32 _SECTION(".kercfg_data") FLG_ID_WIFICMD;
extern UINT32 _SECTION(".kercfg_data") WIFICMD_SEM_ID;

static WIFI_CMD_ENTRY *g_pCmdTab;
static WifiCmd_DefCB *gDefReturnFormat =0;
static WifiCmd_EventHandle *gEventHandle =0;
static UINT32 g_result = 0;
static UINT32 g_receiver = 0;
extern UINT32 WifiCmd_WaitFinish(FLGPTN waiptn);
extern void   WifiCmd_Lock(void);
extern void   WifiCmd_Unlock(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WifiCmd_ReceiveCmd(UINT32 enable)
{
    DBG_IND("WifiCmd_ReceiveCmd %d\r\n",enable);

    if(enable)
    {
        if( (!g_pCmdTab)||(!gEventHandle) )
        {
            DBG_ERR("no Cmd Table or event handle\r\n");
            return;
        }
        clr_flg(FLG_ID_WIFICMD,0xFFFFFFFF);
        g_receiver = enable;
    }
    else
    {
        g_receiver = enable;
        //UI want to close,some app cmd might wait UI finish
        //can not wait flag by UI,therefore set flag for app cmd
        WifiCmd_Done(0xFFFFFFFF,WIFI_CMD_TERMINATE);
    }
}



void WifiCmd_Lock(void)
{
    DBG_IND("Lock\r\n");
    wai_sem(WIFICMD_SEM_ID);
}
void WifiCmd_Unlock(void)
{
    DBG_IND("Unlock\r\n");
    sig_sem(WIFICMD_SEM_ID);
}
void WifiCmd_Done(FLGPTN flag,UINT32 result)
{
    DBG_IND("flag %x result %d\r\n",flag,result);
    g_result = result;
    set_flg(FLG_ID_WIFICMD, flag);

}

UINT32 WifiCmd_WaitFinish(FLGPTN waiptn)
{
    FLGPTN uiFlag;

    DBG_IND("waiptn %x\r\n",waiptn);
    wai_flg(&uiFlag, FLG_ID_WIFICMD, waiptn, (TWF_ORW));
    return g_result;
}

void WifiCmd_SetExecTable(WIFI_CMD_ENTRY *pAppCmdTbl)
{
    g_pCmdTab = pAppCmdTbl;
}
WIFI_CMD_ENTRY *WifiCmd_GetExecTable(void)
{
    return g_pCmdTab;
}
void WifiCmd_SetDefautCB(UINT32 defaultCB)
{
    gDefReturnFormat = (WifiCmd_DefCB *)defaultCB;
}
void WifiCmd_SetEventHandle(UINT32 eventHandle)
{
    gEventHandle = (WifiCmd_EventHandle *)eventHandle;
}

static UINT32 WifiCmd_DispatchCmd(UINT32 cmd,UINT32 par,UINT32 *UserCB)
{
    int i = 0;
    UINT32 ret=0;

    if(g_receiver)
    {
        while(g_pCmdTab[i].cmd != 0)
        {
            if(cmd == g_pCmdTab[i].cmd)
            {
                //UINT32 t1,t2;
                //t1 = Perf_GetCurrent()/1000;
                DBG_IND("cmd:%d evt:%x par:%d CB:%x wait:%x\r\n",g_pCmdTab[i].cmd,g_pCmdTab[i].event,par,g_pCmdTab[i].usrCB,g_pCmdTab[i].Waitflag);
                if(g_pCmdTab[i].Waitflag)
                {
                    clr_flg(FLG_ID_WIFICMD, g_pCmdTab[i].Waitflag);
                    g_result = 0;
                }
                if(gEventHandle && g_pCmdTab[i].event)
                    gEventHandle(g_pCmdTab[i].event, 1,par);
                if(g_pCmdTab[i].Waitflag)
                    ret = WifiCmd_WaitFinish(g_pCmdTab[i].Waitflag);
                if(g_pCmdTab[i].usrCB)
                    *UserCB = (UINT32)g_pCmdTab[i].usrCB;
                //t2 = Perf_GetCurrent()/1000;
                //DBG_ERR("time %d ms\r\n",t2-t1);
                DBG_IND("ret %d\r\n",ret);
                return ret;
            }
            i++;
        }
    }
    else
    {
        DBG_ERR("should WifiCmd_ReceiveCmd enable\r\n");
    }
    return WIFI_CMD_NOT_FOUND;

}
int WifiCmd_GetData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32            cmd=0,par=0;
    char*             pch=0;
    UINT32            ret =0;
    UINT32            UserCB = 0;
    static char       parStr[64]; //post event,data in stack would release
    DBG_IND("Data2 path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);

    if(strncmp(argument,CMD_STR,strlen(CMD_STR))==0)
    {
        if(segmentCount==0)
            WifiCmd_Lock();
        sscanf(argument+strlen(CMD_STR),"%d",&cmd);
        pch=strchr(argument+strlen(CMD_STR),'&');
        if(pch)
        {
            if(strncmp(pch,PAR_STR,strlen(PAR_STR))==0)
            {
                sscanf(pch+strlen(PAR_STR),"%d",&par);
                ret = WifiCmd_DispatchCmd(cmd,par,&UserCB);
            }
            else if(strncmp(pch,PARS_STR,strlen(PARS_STR))==0)
            {
                sscanf(pch+strlen(PARS_STR),"%s",&parStr);
                ret = WifiCmd_DispatchCmd(cmd,(UINT32)parStr,&UserCB);
            }
        }
        else
        {
            ret = WifiCmd_DispatchCmd(cmd,0,&UserCB);
        }
        if(UserCB)
        {
            UINT32 hfs_result;
            hfs_result = ((cyg_hfs_getCustomData *)UserCB)(path,argument,bufAddr,bufSize,mimeType,segmentCount);

            if(hfs_result != CYG_HFS_CB_GETDATA_RETURN_CONTINUE)
                WifiCmd_Unlock();
            return hfs_result;
        }
        else //default return value xml
        {
            if(gDefReturnFormat)
            {
                gDefReturnFormat(cmd,ret,bufAddr,bufSize,mimeType);
                WifiCmd_Unlock();
                return CYG_HFS_CB_GETDATA_RETURN_OK;
            }
            else
            {
                DBG_ERR("no default CB\r\n");
                WifiCmd_Unlock();
                return CYG_HFS_CB_GETDATA_RETURN_ERROR;
            }
        }
    }
    else  //for test,list url command
    {
        UINT32            len=0;
        char*             buf = (char*)bufAddr;
        FST_FILE          filehdl=0;
        char              pFilePath[32];
        UINT32            fileLen =*bufSize;

        sprintf(pFilePath,"%s",HTML_PATH);  //html of all command list
        filehdl = FileSys_OpenFile(pFilePath,FST_OPEN_READ);
        if(filehdl)
        {
            // set the data mimetype
            strcpy(mimeType,"text/html");
            FileSys_ReadFile(filehdl, (UINT8 *)buf, &fileLen, 0,0);
            FileSys_CloseFile(filehdl);
            *bufSize = fileLen;
            *(buf+fileLen) ='\0';
        }
        else
        {
            strcpy(mimeType,"text/html");
            len = sprintf(buf,"no %s file",HTML_PATH);
            buf+=len;
            *bufSize = (cyg_uint32)(buf)-bufAddr;

        }
    }
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

#endif
