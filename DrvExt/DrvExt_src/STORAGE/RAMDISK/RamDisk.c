/**
    RAM disk drvier.

    @file       RamDisk.c
    @ingroup    mICardRAM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.
*/

#include <string.h>
#include "RamDisk.h"
#include "ErrorNo.h"
#include "Debug.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          RamDisk
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "[T]" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

typedef struct {
    UINT32 baseaddr;
    UINT32 size;
} RamDiskSettings;

/**
    @addtogroup mICardRAM
*/
//@{

#define RAMDSK_BYTE_PER_SECTOR          (512)
#define RAMDSK_SECTORBITS               (9 )      //512=2^9
#define RAMDSK_PS_BYTE_PER_BLK          (16*1024)
#define _0x1BE                          (446)        // 0x1BE

static ER ramdsk_open(void);
static ER ramdsk_writeSectors(INT8 *, UINT32, UINT32);
static ER ramdsk_readSectors(INT8 *, UINT32, UINT32);
static ER ramdsk_close(void);
static ER ramdsk_lock(void);
static ER ramdsk_unlock(void);
static ER ramdsk_setParam(STRG_SET_PARAM_EVT uiEvt, UINT32 uiParam1, UINT32 uiParam2);
static ER ramdsk_getParam(STRG_GET_PARAM_EVT uiEvt, UINT32 uiParam1, UINT32 uiParam2);
static ER ramdsk_ps_setParam(STRG_SET_PARAM_EVT uiEvt, UINT32 uiParam1, UINT32 uiParam2);
static ER ramdsk_ps_getParam(STRG_GET_PARAM_EVT uiEvt, UINT32 uiParam1, UINT32 uiParam2);
static ER ramdsk_ps_ExtIOCtrl(STRG_EXT_CMD uiCmd, UINT32 param1, UINT32 param2);

STRG_CAP gRAMCap;
#if(NEW_STORAGE_OBJECT  == DISABLE)
STRG_TAB gRAMObj = {STORAGE_CHANGED|STORAGE_READY|STORAGE_SDRAM, &gRAMCap, ramdsk_open, ramdsk_writeSectors, ramdsk_readSectors, ramdsk_format, ramdsk_close, NULL, NULL, NULL};
#else
STORAGE_OBJ gRAMObj = {STRG_TYPE_RAMDISK, (STORAGE_CHANGED|STORAGE_READY), ramdsk_lock, ramdsk_unlock, ramdsk_open, ramdsk_writeSectors, ramdsk_readSectors, ramdsk_close, NULL, NULL, ramdsk_setParam, ramdsk_getParam, NULL};
STORAGE_OBJ gRAMPSObj = {STRG_TYPE_RAMDISK, (STORAGE_CHANGED|STORAGE_READY), ramdsk_lock, ramdsk_unlock, ramdsk_open, ramdsk_writeSectors, ramdsk_readSectors, ramdsk_close, NULL, NULL, ramdsk_ps_setParam, ramdsk_ps_getParam, ramdsk_ps_ExtIOCtrl};
#endif
RamDiskSettings gRAMSet = {0,0};


static UINT32 RamFATInfo[STRG_FAT_SIZE_16384MB_32768MB][STRG_FAT_TBL_PARAM_CNT]={
// ucHeadNum  ucSectorNum  ucRelativeSector usSectorPerCluster ucResvSec
    {   2,          16,         27,         16,                 1  }, //    0M ~     4M
    {   2,          16,         25,         16,                 1  }, //    4M ~     8M
    {   2,          32,         57,         32,                 1  }, //    8M ~    16M
    {   4,          32,         51,         32,                 1  }, //   16M ~    32M
    {   8,          32,         39,         32,                 1  }, //   32M ~    64M
    {   8,          32,         95,         32,                 1  }, //   64M ~   128M
    {  16,          32,         95,         32,                 1  }, //  128M ~   256M
    {  16,          63,        225,         32,                 1  }, //  256M ~   512M
    {  32,          63,        227,         32,                 1  }, //  512M ~  1024M
    { 128,          63,        227,         64,                 1  }, // 1024M ~  2048M
    { 128,          63,       8192,         64,              6162  }, // 2048M ~  4096M
    { 255,          63,       8192,         64,              4122  }, // 4096M ~  8192M
    { 255,          63,       8192,         64,                34  }, // 8192M ~ 16384M
};

//----- Global funcs & variables define elsewhere --------------
//------------------------------------------------------------------

/**
    Get SDMMC storage object

    @return SDMMC storage object pointer

*/
PSTORAGE_OBJ ramdsk_getStorageObject(void)
{
    return &gRAMObj;
}

/**
    Get SDMMC storage object

    @return SDMMC storage object pointer

*/
PSTORAGE_OBJ ramdsk_ps_getStorageObject(void)
{
    return &gRAMPSObj;
}
/**
    Open RAM disk.

    Initialize the File System, the initial function occupy the DRAM
    size of the RamDiskSettings.size bytes start from RamDiskSettings.baseaddr
    which are initialized by ramdsk_init() at system startup
    The minimal size of RAM disk is 2*32*RAMDSK_BYTE_PER_SECTOR

    This RAM disk is designed to be data lost after powered off.  So each time
    this function is called, the RAM disk is formatted to the designated setting.

    @return E_OK if success, other value otherwise
*/
ER ramdsk_open(void)
{
    if (!gRAMSet.size)
        return E_SYS;

    gRAMCap.uiTotalHeads        = 2;        // Total heads
    gRAMCap.uiSectorsPerTrack    = 32;    // Sectors per track
    gRAMCap.uiTotalCylinders    = (gRAMSet.size / (gRAMCap.uiTotalHeads*gRAMCap.uiSectorsPerTrack*RAMDSK_BYTE_PER_SECTOR));        // Total cylinders
    gRAMCap.uiTotalSectors    = gRAMCap.uiTotalHeads*gRAMCap.uiSectorsPerTrack*gRAMCap.uiTotalCylinders;    // Total sectors
    gRAMCap.uiBytesPerSector    = RAMDSK_BYTE_PER_SECTOR;    // 1 sector = 512 bytes

    //ramdsk_format();

    return E_OK;
}


/**
    Write RAM disk sectors

    @param pcBuf     : src data addr
    @param ulLBAddr: which Sector in the current LUN is specified
    @param ulSctCnt: the nums of sector data will be write
    @return E_OK if successful
*/
ER ramdsk_writeSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt)
{
    UINT32 *SrcAddr, *DstAddr;
    unsigned long DataSize;
    //unsigned long i;

    DstAddr =(unsigned long *)((unsigned int)gRAMSet.baseaddr + (ulLBAddr << RAMDSK_SECTORBITS));
    SrcAddr =(unsigned long *)pcBuf;
    DataSize = ulSctCnt << RAMDSK_SECTORBITS;

    //for(i=0; i<DataSize; i+=4)
    //    *DstAddr++ = *SrcAddr++;
    memcpy(DstAddr, SrcAddr, DataSize);
    return E_OK;
}

/**
    Read RAM disk sectors

    @param pcBuf     : dst data addr
    @param ulLBAddr: which Sector in the current LUN is specified
    @param ulSctCnt: the nums of sector data will be write
    @return E_OK if successful
*/
ER ramdsk_readSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt)
{
    UINT32 *SrcAddr, *DstAddr;
    unsigned long DataSize;
    //unsigned long i;

    SrcAddr =(unsigned long *)((unsigned int)gRAMSet.baseaddr + (ulLBAddr << RAMDSK_SECTORBITS));
    DstAddr =(unsigned long *)pcBuf;
    DataSize = ulSctCnt << RAMDSK_SECTORBITS;

    //for(i=0; i<DataSize; i+=4)
    //    *DstAddr++ = *SrcAddr++;
    memcpy(DstAddr, SrcAddr, DataSize);
    return E_OK;
}

/**
    Close RAM disk

    @return E_OK if successful, other value if otherwise
*/
ER ramdsk_close(void)
{
    return E_OK;
}

ER ramdsk_lock(void)
{
    return E_OK;
}

ER ramdsk_unlock(void)
{
    return E_OK;
}
/**
    Set RAM disk Parameters
    STRG_SET_MEMORY_REGION:Set RAM disk base address and size.
    The minimal size of RAM disk is 2*32*RAMDSK_BYTE_PER_SECTOR
*/
static ER ramdsk_setParam(STRG_SET_PARAM_EVT uiEvt, UINT32 uiParam1, UINT32 uiParam2)
{
    ER  eRet = E_NOSPT;

    switch(uiEvt)
    {
    case STRG_SET_MEMORY_REGION:
    {
        gRAMSet.baseaddr = uiParam1;
        gRAMSet.size = uiParam2;
        eRet = E_OK;
    }
    break;

    default:
        DBG_ERR("No Implement! uiEvt %d\r\n",uiEvt);
        break;
    }

    return eRet;
}

/**
    Get RAM disk Parameters
*/
static ER ramdsk_getParam(STRG_GET_PARAM_EVT uiEvt, UINT32 uiParam1, UINT32 uiParam2)
{
    ER  eRet = E_NOSPT;

    switch(uiEvt)
    {
    case STRG_GET_CAP_TAB:
    {
        memcpy((void *)uiParam1, (void *)&gRAMCap, sizeof(STRG_CAP));
        eRet = E_OK;
    }
    break;
    case STRG_GET_FAT_TABLE_PARAM:
    {
        UINT32 uiIndex=0;
        PSTRG_FATINFO strgFATInfo = (PSTRG_FATINFO)uiParam2;

        if(uiParam1 <= 8032)
        {   uiIndex = STRG_FAT_SIZE_4MB_8MB;  }
        else if ((uiParam1 > 8032) && (uiParam1 <= 16224))      // 8M
        {   uiIndex = STRG_FAT_SIZE_8MB_16MB;  }
        else if ((uiParam1 > 16224) && (uiParam1 <= 32448))     // 16M
        {   uiIndex = STRG_FAT_SIZE_16MB_32MB;  }
        else if ((uiParam1 > 32448) && (uiParam1 <= 64896))     // 32M
        {   uiIndex = STRG_FAT_SIZE_32MB_64MB;  }
        else if ((uiParam1 > 64896) && (uiParam1 <= 129792))    // 64M
        {   uiIndex = STRG_FAT_SIZE_64MB_128MB;  }
        else if ((uiParam1 > 129792) && (uiParam1 <= 259584))   // 128M
        {   uiIndex = STRG_FAT_SIZE_128MB_256MB;  }
        else if ((uiParam1 > 259584) && (uiParam1 <= 519168))   // 256M
        {   uiIndex = STRG_FAT_SIZE_256MB_512MB;  }
        else if ((uiParam1 > 519168) && (uiParam1 <= 1038336))  // 512M
        {   uiIndex = STRG_FAT_SIZE_512MB_1024MB;  }
        else if ((uiParam1 > 1038336) && (uiParam1 <= 2076672)) // 1024M
        {   uiIndex = STRG_FAT_SIZE_1024MB_2048MB;  }
        else if ((uiParam1 > 2076672) && (uiParam1 <= 4153344)) // 2048M
        {   uiIndex = STRG_FAT_SIZE_2048MB_4096MB;  }
        else if ((uiParam1 > 4153344) && (uiParam1 <= 8323072)) // 4096M
        {   uiIndex = STRG_FAT_SIZE_4096MB_8192MB; }
        else if((uiParam1 > 8323072) )
        {   uiIndex = STRG_FAT_SIZE_8192MB_16384MB; }

        strgFATInfo->uiHeadNum          = RamFATInfo[uiIndex][STRG_FAT_TBL_HEADNUM];
        strgFATInfo->uiSectorNum        = RamFATInfo[uiIndex][STRG_FAT_TBL_SECTORNUM];
        strgFATInfo->uiRelativeSector   = RamFATInfo[uiIndex][STRG_FAT_TBL_RELATIVESECTOR];
        strgFATInfo->uiSectorPerCluster = RamFATInfo[uiIndex][STRG_FAT_TBL_SECPERCLUSTER];
        strgFATInfo->uiResvSec          = RamFATInfo[uiIndex][STRG_FAT_TBL_RESERVEDSECTOR];
        eRet = E_OK;
    }
    break;

    default:
        DBG_ERR("No Implement! uiEvt %d\r\n",uiEvt);
        break;
    }

    return eRet;
}


/**
    Set PStore RAM disk Parameters
*/
static ER ramdsk_ps_setParam(STRG_SET_PARAM_EVT uiEvt, UINT32 uiParam1, UINT32 uiParam2)
{
    ER  eRet = E_NOSPT;

    switch(uiEvt)
    {
    case STRG_SET_PARTITION_SIZE:
    {
        gRAMSet.size = uiParam2;
        eRet = E_OK;
    }
    break;

    default:
        DBG_ERR("No Implement! uiEvt %d\r\n",uiEvt);
        break;
    }

    return eRet;
}

/**
    Get PStore RAM disk Parameters
*/
static ER ramdsk_ps_getParam(STRG_GET_PARAM_EVT uiEvt, UINT32 uiParam1, UINT32 uiParam2)
{
    ER  eRet = E_NOSPT;

    switch(uiEvt)
    {
    case STRG_GET_BEST_ACCESS_SIZE:
    case STRG_GET_SECTOR_SIZE:
    {
        *(UINT32 *)(uiParam1) = RAMDSK_PS_BYTE_PER_BLK;
        eRet = E_OK;
    }
    break;
    case STRG_GET_CAP_TAB:
    {
        gRAMCap.uiTotalSectors = gRAMCap.uiTotalSectors * RAMDSK_BYTE_PER_SECTOR/RAMDSK_PS_BYTE_PER_BLK;
        memcpy((void *)uiParam1, (void *)&gRAMCap, sizeof(STRG_CAP));
        eRet = E_OK;
    }
    break;

    case STRG_GET_DEVICE_PHY_SIZE:
    {
        *(UINT32 *)(uiParam1) = gRAMSet.size;
        eRet = E_OK;
    }
    break;

    default:
        DBG_ERR("No Implement! uiEvt %d\r\n",uiEvt);
        break;
    }

    return eRet;
}

static ER  ramdsk_ps_ExtIOCtrl(STRG_EXT_CMD uiCmd, UINT32 param1, UINT32 param2)
{
    switch(uiCmd)
    {
        case STRG_EXT_CMD_FORMAT:
            memset((char *)gRAMSet.baseaddr,0xFF,gRAMSet.size);
            return E_OK;
        break;

        default:
            return E_NOSPT;
        break;

    }
    return E_OK;
}

//@}
