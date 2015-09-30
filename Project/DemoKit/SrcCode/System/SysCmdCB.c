/*
    System Command Callback

    System Callback for Command-Line Console Module.

    @file       SysCmdCB.c
    @ingroup    mIPRJSYS
    @note       None

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#include "SysCommon.h"
#include "SysMain.h"
#include "GxSystem.h"
#include "PrjCfg.h"
#include "ProjectInfo.h"
//#include "KerVerInfo.h"
//#include "Driver.h"
#include "SysCfg.h"
#include "AppVerInfo.h"
#include "FileSysVerInfo.h"
#include "UIModeMain.h"
#include "UIModePhoto.h"
#include "UIModeMovie.h"
#include "UIModePlayback.h"
//#NT#2012/02/15#Lincy Lin -begin
//#NT#
#include "fs_file_op.h"
#include "FileSysTsk.h"
#include "DCF.h"

#include "UIFramework.h"
#include "NVTUserCommand.h"

#include "AppLib.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "af_api.h"
#include "PStore.h"
#include "IPL_Ctrl.h"
#include "Ae_task.h"
#include "Awb_task.h"
#include "UICommon.h"
#include "Dx.h"
#include "NvtVerInfo.h"
#include "NvtSystem.h"
#include "UIAppMovie.h"  // Isiah, implement YUV merge mode of recording func.
#include "MediaRecAPI.h"

CHAR    FWFileName[] = "A:\\FW96650A.bin";
CHAR    LDFileName[] = "A:\\LD96650A.bin";

//#NT#2012/02/15#Lincy Lin -end

//---------------------SysCmdCB Debug Definition -----------------------------
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysCmdCB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//---------------------SysCmdCB Global Variables -----------------------------
//---------------------SysCmdCB Prototype Declaration  -----------------------
//---------------------SysCmdCB Public API  ----------------------------------
//---------------------SysCmdCB Private API ----------------------------------

///////////////////////////////////////////////////////////////////////////////
// temp memory for user cmd
///////////////////////////////////////////////////////////////////////////////


UINT32 User_GetTempBuffer(UINT32 uiSize)
{
    void *pBuf = 0;
    if(uiSize <= POOL_SIZE_APP)
    {
        pBuf = (void*)OS_GetMempoolAddr(POOL_ID_APP);
    }
    if(pBuf == NULL)
    {
        DBG_ERR("get buffer fail\r\n");
    }
    return (UINT32)pBuf;
}


///////////////////////////////////////////////////////////////////////////////
// user cmd
///////////////////////////////////////////////////////////////////////////////

BOOL Cmd_user_cmd(CHAR* strCmd);
BOOL Cmd_user_dump_pool(CHAR* strCmd);
BOOL Cmd_test_merge_record(CHAR* strCmd);
BOOL Cmd_sensor_sleep(CHAR* strCmd);
BOOL Cmd_sensor_wakeup(CHAR* strCmd);
BOOL Cmd_post_card_write_error(CHAR* strCmd);
BOOL Cmd_post_slow_card_error(CHAR* strCmd);
BOOL Cmd_post_card_full_error(CHAR* strCmd);
BOOL Cmd_time_lapse_test(CHAR* strCmd);

SXCMD_BEGIN(user, "user command")
SXCMD_ITEM("cmd", Cmd_user_cmd, "sample code")
SXCMD_ITEM("dump_pool", Cmd_user_dump_pool, "dump pool")
SXCMD_ITEM("test_merge_record %", Cmd_test_merge_record, "test merge record")
SXCMD_ITEM("sensor_sleep", Cmd_sensor_sleep, "sensor sleep")
SXCMD_ITEM("sensor_wakeup", Cmd_sensor_wakeup, "sensor wakeup")
SXCMD_ITEM("card_wr_err", Cmd_post_card_write_error, "movie recording with card error test")
SXCMD_ITEM("card_slow_err", Cmd_post_slow_card_error, "movie recording with slow card test")
SXCMD_ITEM("card_full", Cmd_post_card_full_error, "movie recording with card full error test")
SXCMD_ITEM("time_lapse_test %", Cmd_time_lapse_test, "movie recording with time-lapse func. test")
SXCMD_END()

BOOL Cmd_user_cmd(CHAR* strCmd)
{
    DBG_DUMP("this is sample cmd!\r\n");
    return TRUE;
}

BOOL Cmd_user_dump_pool(CHAR* strCmd)
{
    UINT32 i;
    DBG_DUMP("dump pool status\r\n");

    debug_msg("POOL_ID_STORAGE_NAND:%d- start addr:0x%08lX, size:0x%08lX\r\n", POOL_ID_STORAGE_NAND, OS_GetMempoolAddr(POOL_ID_STORAGE_NAND), OS_GetMempoolSize(POOL_ID_STORAGE_NAND));
    debug_msg("POOL_ID_APP:%d- start addr:0x%08lX, size:0x%08lX\r\n", POOL_ID_APP, OS_GetMempoolAddr(POOL_ID_APP), OS_GetMempoolSize(POOL_ID_APP));

    debug_msg("------------------------------------------------\r\n");
    for(i = 1; i <= POOL_ID_APP; i++)
    {
        debug_msg("POOL_ID_%d- start addr:0x%08lX, size:0x%08lX\r\n", i, OS_GetMempoolAddr(i), OS_GetMempoolSize(i));
    }

    return TRUE;
}

BOOL Cmd_sensor_sleep(CHAR* strCmd)
{
    IPL_SLEEP_INFO Info;

    Info.Id = 0;

    IPL_SetCmd(IPL_SET_SLEEP, (void *)&Info);
    IPL_WaitCmdFinish();
    DBG_DUMP("IPL set sleep %d\r\n", Info.Id);

    return TRUE;
}

BOOL Cmd_sensor_wakeup(CHAR* strCmd)
{
    IPL_SLEEP_INFO Info;

    Info.Id = 0;

    IPL_SetCmd(IPL_SET_WAKEUP, (void *)&Info);
    IPL_WaitCmdFinish();
    DBG_DUMP("IPL set wakeup %d\r\n", Info.Id);

    return TRUE;
}

BOOL Cmd_test_merge_record(CHAR* strCmd)
{
    BOOL bEnable;
    UINT32 uiInterval;

    DBG_DUMP("Test merge record\r\n");

    sscanf(strCmd,"%d %d", &bEnable, &uiInterval);

    FlowMovie_RecSetYUVMergeRecInterval(uiInterval);
    FlowMovie_RecSetYUVMergeRecCounter(0);
    FlowMovie_RecSetYUVMergeMode(bEnable);

    if(bEnable)
        debug_msg("Enable YUV merge record mode, interval=%d\r\n", uiInterval);
    else
        debug_msg("Disable YUV merge record mode\r\n");

    return TRUE;
}

extern void     MediaRecUti_SetResult(UINT32 result);
BOOL Cmd_post_card_write_error(CHAR* strCmd)
{
    //Ux_PostEvent(NVTEVT_CB_MOVIE_WR_ERROR, 0);
    MediaRecUti_SetResult(MEDIAREC_EVENT_RESULT_WRITE_ERR);
    debug_msg("Post MEDIAREC_EVENT_RESULT_WRITE_ERR\r\n");
    return TRUE;
}

BOOL Cmd_post_slow_card_error(CHAR* strCmd)
{
    //Ux_PostEvent(NVTEVT_CB_MOVIE_SLOW, 0);
    MediaRecUti_SetResult(MEDIAREC_EVENT_RESULT_SLOW);
    debug_msg("Post MEDIAREC_EVENT_RESULT_SLOW\r\n");
    return TRUE;
}

BOOL Cmd_post_card_full_error(CHAR* strCmd)
{
    //Ux_PostEvent(NVTEVT_CB_MOVIE_FULL, 0);
    MediaRecUti_SetResult(MEDIAREC_EVENT_RESULT_FULL);
    debug_msg("Post MEDIAREC_EVENT_RESULT_FULL\r\n");
    return TRUE;
}

BOOL Cmd_time_lapse_test(CHAR* strCmd)
{
    UINT32 uiTmpLapseOption;

    sscanf(strCmd,"%d", &uiTmpLapseOption);

    DBG_DUMP("Test time-lapse option=%d\r\n", uiTmpLapseOption);

    switch(uiTmpLapseOption)
    {
      case 0:
      default:
        SysSetFlag(FL_MOVIE_TIMELAPSE_REC,  MOVIE_TIMELAPSEREC_OFF);
        break;
      case 1:
        SysSetFlag(FL_MOVIE_TIMELAPSE_REC,  MOVIE_TIMELAPSEREC_100MS);
        break;
      case 2:
        SysSetFlag(FL_MOVIE_TIMELAPSE_REC,  MOVIE_TIMELAPSEREC_200MS);
        break;
      case 3:
        SysSetFlag(FL_MOVIE_TIMELAPSE_REC,  MOVIE_TIMELAPSEREC_500MS);
        break;
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// dsc cmd
///////////////////////////////////////////////////////////////////////////////

BOOL Cmd_dsc_boot(CHAR* strCmd);
BOOL Cmd_dsc_shutdown(CHAR* strCmd);
BOOL Cmd_dsc_dumpver_common(CHAR* strCmd);
BOOL Cmd_dsc_dumpver(CHAR* strCmd);
SXCMD_BEGIN(dsc, "platform dsc command")
SXCMD_ITEM("boot", Cmd_dsc_boot,"boot on dsc")
SXCMD_ITEM("shutdown", Cmd_dsc_shutdown,"shutdown dsc")
SXCMD_ITEM("dumpver", Cmd_dsc_dumpver, "dump all library version info")
SXCMD_END()

BOOL Cmd_dsc_boot(CHAR* strCmd)
{
    DBG_DUMP("Dsc Boot\r\n");
    //DscMain();
    // Start Dsc System
    //System_PowerOn(SYS_POWERON_NORMAL);
    GxSystem_PowerOn(); //boot start
    return TRUE;
}
BOOL Cmd_dsc_shutdown(CHAR* strCmd)
{
    DBG_DUMP("Dsc Shutdown\r\n");
    //System_PowerOff(SYS_POWEROFF_NORMAL);
    Ux_SendEvent(0, NVTEVT_SYSTEM_SHUTDOWN, 1, 0); //shutdown start
    return TRUE;
}


BOOL Cmd_dsc_dumpver(CHAR* strCmd)
{
    Nvtver_DumpVerInfo();
    return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
// key input
///////////////////////////////////////////////////////////////////////////////


//FILTER for KEY EMULATION
BOOL User_CommandFilter(CHAR* pcCmdStr)
{
    int slen = strlen(pcCmdStr);

    switch (*pcCmdStr)
    {
    // Key Macro
    case 'k':
        if(!strncmp(pcCmdStr, "koff", 4) && (slen == 4))
        {
            strcpy(pcCmdStr, "key power");
        }
        else
        if (!strncmp(pcCmdStr, "kdel", 4) && (slen == 4))
        {
            strcpy(pcCmdStr, "key delete");
        }
        else
        if (!strncmp(pcCmdStr, "km", 2) && (slen == 2))
        {
            strcpy(pcCmdStr, "key menu");
        }
        else
        if (!strncmp(pcCmdStr, "kmd", 3) && (slen == 3))
        {
            strcpy(pcCmdStr, "key mode");
        }
        else
        if (!strncmp(pcCmdStr, "kmp", 3) && (slen == 3))
        {
            strcpy(pcCmdStr, "key playback");
        }
        else
        if (!strncmp(pcCmdStr, "kmv", 3) && (slen == 3))
        {
            strcpy(pcCmdStr, "key movie");
        }
        else
        if (!strncmp(pcCmdStr, "ku", 2) && (slen == 2))
        {
            strcpy(pcCmdStr, "key up");
        }
        else
        if (!strncmp(pcCmdStr, "kd", 2) && (slen == 2))
        {
            strcpy(pcCmdStr, "key down");
        }
        else
        if (!strncmp(pcCmdStr, "kr", 2) && (slen == 2))
        {
            strcpy(pcCmdStr, "key right");
        }
        else
        if (!strncmp(pcCmdStr, "kl", 2) && (slen == 2))
        {
            strcpy(pcCmdStr, "key left");
        }
        else
        if (!strncmp(pcCmdStr, "ke", 2) && (slen == 2))
        {
            strcpy(pcCmdStr, "key enter");
        }
        else
        if (!strncmp(pcCmdStr, "kzi", 3) && (slen == 3))
        {
            strcpy(pcCmdStr, "key zoomin");
        }
        else
        if (!strncmp(pcCmdStr, "kzo", 3) && (slen == 3))
        {
            strcpy(pcCmdStr, "key zoomout");
        }
        else
        if (!strncmp(pcCmdStr, "kzr", 3) && (slen == 3))
        {
            strcpy(pcCmdStr, "key zoom release");
        }
        else
        if (!strncmp(pcCmdStr, "ks1", 3) && (slen == 3))
        {
            strcpy(pcCmdStr, "key shutter1");
        }
        else
        if (!strncmp(pcCmdStr, "ks2", 3) && (slen == 3))
        {
            strcpy(pcCmdStr, "key shutter2");
        }
        if (!strncmp(pcCmdStr, "ki", 2) && (slen == 2))
        {
            strcpy(pcCmdStr, "key i");
        }
        break;
    }
    return FALSE;
}


BOOL Cmd_key_power(CHAR* strCmd);
BOOL Cmd_key_delete(CHAR* strCmd);
BOOL Cmd_key_menu(CHAR* strCmd);
BOOL Cmd_key_mode(CHAR* strCmd);
BOOL Cmd_key_playback(CHAR* strCmd);
BOOL Cmd_key_movie(CHAR* strCmd);
BOOL Cmd_key_up(CHAR* strCmd);
BOOL Cmd_key_down(CHAR* strCmd);
BOOL Cmd_key_right(CHAR* strCmd);
BOOL Cmd_key_left(CHAR* strCmd);
BOOL Cmd_key_enter(CHAR* strCmd);
BOOL Cmd_key_zoomin(CHAR* strCmd);
BOOL Cmd_key_zoomout(CHAR* strCmd);
BOOL Cmd_key_zoomin_release(CHAR* strCmd);
BOOL Cmd_key_zoomout_release(CHAR* strCmd);
BOOL Cmd_key_zoomrelease(CHAR* strCmd);
BOOL Cmd_key_shutter1(CHAR* strCmd);
BOOL Cmd_key_shutter2(CHAR* strCmd);
BOOL Cmd_key_shutter1rel(CHAR* strCmd);
BOOL Cmd_key_shutter2rel(CHAR* strCmd);
BOOL Cmd_key_i(CHAR* strCmd);

SXCMD_BEGIN(key, "key command")
SXCMD_ITEM("power", Cmd_key_power, "power key")
SXCMD_ITEM("delete", Cmd_key_delete, "delete key")
SXCMD_ITEM("menu", Cmd_key_menu, "menu key")
SXCMD_ITEM("mode", Cmd_key_mode, "mode key")
SXCMD_ITEM("playback", Cmd_key_playback, "playback key")
SXCMD_ITEM("movie", Cmd_key_movie, "movie key")
SXCMD_ITEM("up", Cmd_key_up, "up key")
SXCMD_ITEM("down", Cmd_key_down, "down key")
SXCMD_ITEM("right", Cmd_key_right, "right key")
SXCMD_ITEM("left", Cmd_key_left, "left key")
SXCMD_ITEM("enter", Cmd_key_enter, "enter key")
SXCMD_ITEM("zoomin", Cmd_key_zoomin, "zoomin key pressed")
SXCMD_ITEM("zoomout", Cmd_key_zoomout, "zoomout key pressed")
SXCMD_ITEM("zoominrel", Cmd_key_zoomin_release, "zoomin key released")
SXCMD_ITEM("zoomoutrel", Cmd_key_zoomout_release, "zoomout key released")
SXCMD_ITEM("zoomrel", Cmd_key_zoomrelease, "zoom key released")
SXCMD_ITEM("shutter1", Cmd_key_shutter1, "shutter1 key pressed")
SXCMD_ITEM("shutter2", Cmd_key_shutter2, "suutter2 key pressed")
SXCMD_ITEM("shutter1rel", Cmd_key_shutter1rel, "shutter1 key released")
SXCMD_ITEM("shutter2rel", Cmd_key_shutter2rel, "shutter2 key released")
SXCMD_ITEM("i", Cmd_key_i, "i key pressed")
SXCMD_END()

//#Fix warning message
//UINT32  uiKeyTmpMsk;

BOOL Cmd_key_power(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_XXX)
    {
        DBG_DUMP("Power Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_POWER, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_POWER, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_delete(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_DEL)
    {
        DBG_DUMP("Del Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_DEL, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_DEL, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_menu(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_MENU)
    {
        DBG_DUMP("Menu Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_MENU, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_MENU, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_mode(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_MODE)
    {
        DBG_DUMP("Mode Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_MODE, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_MODE, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_playback(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_PLAYBACK)
    {
        DBG_DUMP("Playback Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_PLAYBACK, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_PLAYBACK, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_movie(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_PLAYBACK)
    {
        DBG_DUMP("Movie Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_MOVIE, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_MOVIE, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_up(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_UP)
    {
        DBG_DUMP("Up Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_UP, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_UP, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_down(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_DOWN)
    {
        DBG_DUMP("Down Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_DOWN, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_DOWN, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_right(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_RIGHT)
    {
        DBG_DUMP("Right Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_RIGHT, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_RIGHT, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_left(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_LEFT)
    {
        DBG_DUMP("Left Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_LEFT, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_enter(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ENTER)
    {
        DBG_DUMP("Enter Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_ENTER, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_zoomin(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ZOOMIN)
    {
        DBG_DUMP("Zoom In Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_zoomout(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ZOOMOUT)
    {
        DBG_DUMP("Zoom Out Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 0);
#endif
        return TRUE;
    }
}

BOOL Cmd_key_zoomin_release(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ZOOMIN)
    {
        DBG_DUMP("Zoom In Key release\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_RELEASE);
#else
        Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 0);
#endif
        return TRUE;
    }
}

BOOL Cmd_key_zoomout_release(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ZOOMOUT)
    {
        DBG_DUMP("Zoom Out Key release\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_RELEASE);
#else
        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 0);
#endif
        return TRUE;
    }
}

BOOL Cmd_key_zoomrelease(CHAR* strCmd)
{
    {
        DBG_DUMP("zoom release Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_ZOOMRELEASE, 1, NVTEVT_KEY_ZOOMRELEASE);
#else
        Ux_PostEvent(NVTEVT_KEY_ZOOMRELEASE, 0);
#endif
        return TRUE;
    }
}

BOOL Cmd_key_shutter1(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_SHUTTER1)
    {
        DBG_DUMP("Shutter1 Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 0);
#endif
        return TRUE;
    }
}
BOOL Cmd_key_shutter2(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_SHUTTER2)
    {
        DBG_DUMP("Shutter2 Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
#else
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 0);
#endif
        return TRUE;
    }
}

BOOL Cmd_key_shutter1rel(CHAR* strCmd)
{
    {
        DBG_DUMP("Shutter1 release Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 1, NVTEVT_KEY_RELEASE);
#else
        Ux_PostEvent(NVTEVT_KEY_SHUTTER1_REL, 0);
#endif
        return TRUE;
    }
}

BOOL Cmd_key_shutter2rel(CHAR* strCmd)
{
    {
        DBG_DUMP("Shutter2 release Key\r\n");
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_RELEASE);
#else
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2_REL, 0);
#endif
        return TRUE;
    }
}
#include "UIMenuWndWiFiWait.h"
BOOL Cmd_key_i(CHAR* strCmd)
{
    DBG_DUMP("I Key\r\n");

    Ux_OpenWindow(&UIMenuWndWiFiWaitCtrl, 0);
    BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
    return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
// mode input
///////////////////////////////////////////////////////////////////////////////

BOOL Cmd_mode_main(CHAR* strCmd);
BOOL Cmd_mode_photo(CHAR* strCmd);
BOOL Cmd_mode_movie(CHAR* strCmd);
BOOL Cmd_mode_playback(CHAR* strCmd);
BOOL Cmd_mode_msdc(CHAR* strCmd);
BOOL Cmd_mode_sidc(CHAR* strCmd);
BOOL Cmd_mode_pcc(CHAR* strCmd);
BOOL Cmd_mode_lview(CHAR* strCmd);

SXCMD_BEGIN(mode, "mode switch")
SXCMD_ITEM("main", Cmd_mode_main, "main mode")
SXCMD_ITEM("photo", Cmd_mode_photo, "photo mode")
SXCMD_ITEM("movie", Cmd_mode_movie, "movie mode")
SXCMD_ITEM("playback", Cmd_mode_playback, "playback mode")
SXCMD_ITEM("msdc", Cmd_mode_msdc, "msdc mode")
SXCMD_ITEM("sidc", Cmd_mode_sidc, "sidc mode")
SXCMD_ITEM("pcc", Cmd_mode_pcc, "pcc mode")
SXCMD_ITEM("lview %", Cmd_mode_lview, "live view mode setting")
SXCMD_END()

BOOL Cmd_mode_main(CHAR* strCmd)
{
    DBG_DUMP("Main Mode\r\n");
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
    return TRUE;
}
BOOL Cmd_mode_photo(CHAR* strCmd)
{
    DBG_DUMP("Photo Mode\r\n");
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
    return TRUE;
}
BOOL Cmd_mode_movie(CHAR* strCmd)
{
    DBG_DUMP("Movie Mode\r\n");
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
    return TRUE;
}
BOOL Cmd_mode_playback(CHAR* strCmd)
{
    DBG_DUMP("Playback Mode\r\n");
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
    return TRUE;
}
BOOL Cmd_mode_msdc(CHAR* strCmd)
{
    DBG_DUMP("USB MSDC Mode\r\n");
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_USBMSDC);
    return TRUE;
}
BOOL Cmd_mode_sidc(CHAR* strCmd)
{
#if (UI_STYLE!=UI_STYLE_DRIVER)
    DBG_DUMP("USB SIDC Mode\r\n");
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_USBSIDC);
#endif
    return TRUE;
}
BOOL Cmd_mode_pcc(CHAR* strCmd)
{
    DBG_DUMP("USB PCC Mode\r\n");
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_USBPCC);
    return TRUE;
}

UINT32      g_lviewframeRate = 30,g_lviewTargetRate=(400*1024), g_lviewQueueFrame=2, g_lviewPort=8192, g_lviewTaskPri=6, g_hwmemcpy = 1, g_maxJpgSize = 204800, g_reqDelay=0;

BOOL Cmd_mode_lview(CHAR* strCmd)
{
    sscanf(strCmd,"%d %d %d %d %d %d %d %d",&g_lviewframeRate,&g_lviewTargetRate,&g_lviewQueueFrame,&g_lviewPort,&g_lviewTaskPri, &g_hwmemcpy, &g_maxJpgSize, &g_reqDelay);
    return TRUE;
}

