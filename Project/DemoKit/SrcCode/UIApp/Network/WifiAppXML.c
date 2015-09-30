#ifdef __ECOS
#include "FileSysTsk.h"
#include <cyg\hfs\hfs.h>
#include "FileDB.h"
#include "PBXFileList_FileDB.h"
#include "DCF.h"
#include "SysCommon.h"
#include "WifiAppCmd.h"
#include "UIPlayObj.h"
#include "UIMovieObj.h"
#include "PrjCfg.h"
#include "Exif.h"
#include "HwMem.h"
#include "UIInfo.h"
#include "GxVideoFile.h"
#include "WifiAppXML.h"
#include "ProjectInfo.h"
#include "UIFlow.h"
#include "UIAppNetwork.h"
#include "RawEncAPI.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          WifiAppXML
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#define QUERY_XML_PATH  "A:\\query.xml"
#define QUERY_CMD_CUR_STS_XML_PATH  "A:\\query_cmd_cur_sts.xml"
#define CMD_STR "custom=1&cmd="
#define PAR_STR "&par="
#define QUERY_FMT  "{\"mfr\":\"%s\",\"type\":\"%d\",\"model\":\"%s\",\"p2p_uuid\":\"\"}"
/*mfr:廠商名稱
type:1-行車紀錄器 2- IP camera
model:產品型號
p2p_uuid:用戶p2p標誌,沒有可以不填 ""
*/


#define FAT_GET_DAY_FROM_DATE(x)     (x & 0x1F)              ///<  get day from date
#define FAT_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)       ///<  get month from date
#define FAT_GET_YEAR_FROM_DATE(x)    ((x >> 9) & 0x7F)+1980  ///<  get year from date
#define FAT_GET_SEC_FROM_TIME(x)     (x & 0x001F)<<1         ///<  seconds(2 seconds / unit)
#define FAT_GET_MIN_FROM_TIME(x)     (x & 0x07E0)>>5         ///<  Minutes
#define FAT_GET_HOUR_FROM_TIME(x)    (x & 0xF800)>>11        ///<  hours

void XML_StringResult(UINT32 cmd,char *str,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);
void XML_ValueResult(UINT32 cmd,UINT64 value,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);


#define HW_TV_ENABLE           0x00000001
#define HW_GSENDOR_ENABLE      0x00000002

static FILEDB_INIT_OBJ gWifiFDBInitObj={
                         "A:\\",  //rootPath
                         NULL,  //defaultfolder
                         NULL,  //filterfolder
                         TRUE,  //bIsRecursive
                         TRUE,  //bIsCyclic
                         TRUE,  //bIsMoveToLastFile
                         TRUE, //bIsSupportLongName
                         FALSE, //bIsDCFFileOnly
                         TRUE,  //bIsFilterOutSameDCFNumFolder
                         {'N','V','T','I','M'}, //OurDCFDirName[5]
                         {'I','M','A','G'}, //OurDCFFileName[4]
                         FALSE,  //bIsFilterOutSameDCFNumFile
                         FALSE, //bIsChkHasFile
                         60,    //u32MaxFilePathLen
                         10000,  //u32MaxFileNum
                         (FILEDB_FMT_JPG|FILEDB_FMT_AVI|FILEDB_FMT_MOV|FILEDB_FMT_MP4),
                         0,     //u32MemAddr
                         0,     //u32MemSize
                         NULL   //fpChkAbort

};

static void XML_ecos2NvtPath(const char* inPath, char* outPath)
{
    outPath+=sprintf(outPath,"A:");
    //inPath+=strlen(MOUNT_FS_ROOT);
    while (*inPath !=0)
    {
        if (*inPath == '/')
            *outPath = '\\';
        else
            *outPath = *inPath;
        inPath++;
        outPath++;
    }
    //*outPath++ = '\\'; //If adding this, it will be regarded as folder.
    *outPath = 0;
}
static void XML_Nvt2ecosPath(const char* inPath, char* outPath)
{
    //outPath+=sprintf(outPath,MOUNT_FS_ROOT);
    inPath+=strlen("A:");
    while (*inPath !=0)
    {
        if (*inPath == '\\')
            *outPath = '/';
        else
            *outPath = *inPath;
        inPath++;
        outPath++;
    }
    *outPath = 0;
}
#if USE_FILEDB
int XML_PBGetData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    static UINT32     gIndex;
    UINT32            len, fileCount, i, bufflen = *bufSize;
    char*             buf = (char*)bufAddr;
    PPBX_FLIST_OBJ    pFlist = PBXFList_FDB_getObject();
    UINT32            FileDBHandle = 0;

    Ux_SendEvent(0,NVTEVT_PLAYINIT,0);

    if (segmentCount == 0)
    {
        len = sprintf(buf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
        buf+=len;
    }

    pFlist->GetInfo(PBX_FLIST_GETINFO_DB_HANDLE,&FileDBHandle,0);
    fileCount = FileDB_GetTotalFileNum(FileDBHandle);

    if (segmentCount == 0)
    {
        // reset some global variables
        gIndex = 0;
    }
    DBG_IND("gIndex = %d %d \r\n",gIndex,fileCount);
    for (i=gIndex;i<fileCount;i++)
    {
        PFILEDB_FILE_ATTR        pFileAttr;
        // check buffer length , reserved 512 bytes
        // should not write data over buffer length
        if ((cyg_uint32)buf-bufAddr > bufflen - 512)
        {
            DBG_IND("totallen=%d >  bufflen(%d)-512\r\n",(cyg_uint32)buf-bufAddr,bufflen);
            *bufSize = (cyg_uint32)(buf)-bufAddr;
            gIndex = i;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        // get dcf file
        pFileAttr = FileDB_SearhFile(FileDBHandle, i);

        //debug_msg("file %s %d\r\n",pFileAttr->filePath,pFileAttr->fileSize);

        {
            len = sprintf(buf,"<ALLFile><File>\n<NAME>%s</NAME>\n<FPATH>%s</FPATH>\n",pFileAttr->filename,pFileAttr->filePath);
            buf+=len;
            len = sprintf(buf,"<SIZE>%d</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n<ATTR>%d</ATTR></File>\n</ALLFile>\n",pFileAttr->fileSize,(pFileAttr->lastWriteDate <<16) + pFileAttr->lastWriteTime,
                  FAT_GET_YEAR_FROM_DATE(pFileAttr->lastWriteDate),FAT_GET_MONTH_FROM_DATE(pFileAttr->lastWriteDate),FAT_GET_DAY_FROM_DATE(pFileAttr->lastWriteDate),
                  FAT_GET_HOUR_FROM_TIME(pFileAttr->lastWriteTime),FAT_GET_MIN_FROM_TIME(pFileAttr->lastWriteTime),FAT_GET_SEC_FROM_TIME(pFileAttr->lastWriteTime),
                  pFileAttr->attrib );
            buf+=len;
        }
    }
    len = sprintf(buf,"</LIST>\n");
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


#else

int XML_PBGetData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    static UINT32     gIndex;
    UINT32            len, fileCount, i, bufflen = *bufSize, FileType;
    char*             buf = (char*)bufAddr;
    char              tempPath[128];
    FST_FILE_STATUS   FileStat;
    FST_FILE          filehdl;
    char              dcfFilePath[128];

    // set the data mimetype
    strcpy(mimeType,"text/xml");
    XML_ecos2NvtPath(path, tempPath);

    if (segmentCount == 0)
    {
        len = sprintf(buf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
        buf+=len;
    }
    fileCount = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
    if (segmentCount == 0)
    {
        // reset some global variables
        gIndex = 1;
    }
    DBG_IND("gIndex = %d %d \r\n",gIndex,fileCount);
    for (i=gIndex;i<=fileCount;i++)
    {
        // check buffer length , reserved 512 bytes
        // should not write data over buffer length
        if ((cyg_uint32)buf-bufAddr > bufflen - 512)
        {
            DBG_IND("totallen=%d >  bufflen(%d)-512\r\n",(cyg_uint32)buf-bufAddr,bufflen);
            *bufSize = (cyg_uint32)(buf)-bufAddr;
            gIndex = i;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        // get dcf file
        DCF_SetCurIndex(i);
        FileType = DCF_GetDBInfo(DCF_INFO_CUR_FILE_TYPE);

        if (FileType & DCF_FILE_TYPE_JPG)
        {
            DCF_GetObjPath(i, DCF_FILE_TYPE_JPG, dcfFilePath);
        }
        else if (FileType & DCF_FILE_TYPE_MOV)
        {
            DCF_GetObjPath(i, DCF_FILE_TYPE_MOV, dcfFilePath);
        }
        else
        {
            continue;
        }
        // get file state
        filehdl = FileSys_OpenFile(dcfFilePath,FST_OPEN_READ);
        FileSys_StatFile(filehdl, &FileStat);
        FileSys_CloseFile(filehdl);
        XML_Nvt2ecosPath(dcfFilePath,tempPath);

        // this is a dir
        if (M_IsDirectory(FileStat.uiAttrib))
        {
            len = sprintf(buf,"<Dir>\n<NAME>\n<![CDATA[%s]]></NAME><FPATH>\n<![CDATA[%s]]></FPATH>",&tempPath[15],tempPath);
            buf+=len;
            len = sprintf(buf,"<TIMECODE>%ld</TIMECODE><TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n</Dir>\n",(FileStat.uiModifiedDate <<16) + FileStat.uiModifiedTime,
                  FAT_GET_YEAR_FROM_DATE(FileStat.uiModifiedDate),FAT_GET_MONTH_FROM_DATE(FileStat.uiModifiedDate),FAT_GET_DAY_FROM_DATE(FileStat.uiModifiedDate),
                  FAT_GET_HOUR_FROM_TIME(FileStat.uiModifiedTime),FAT_GET_MIN_FROM_TIME(FileStat.uiModifiedTime),FAT_GET_SEC_FROM_TIME(FileStat.uiModifiedTime));
            buf+=len;
        }
        // this is a file
        else
        {
            len = sprintf(buf,"<ALLFile><File>\n<NAME>\n<![CDATA[%s]]></NAME><FPATH>\n<![CDATA[%s]]></FPATH>",&tempPath[15],tempPath);
            buf+=len;
            len = sprintf(buf,"<SIZE>%lld</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n</File>\n</ALLFile>\n",FileStat.uiFileSize,(FileStat.uiModifiedDate <<16) + FileStat.uiModifiedTime,
                  FAT_GET_YEAR_FROM_DATE(FileStat.uiModifiedDate),FAT_GET_MONTH_FROM_DATE(FileStat.uiModifiedDate),FAT_GET_DAY_FROM_DATE(FileStat.uiModifiedDate),
                  FAT_GET_HOUR_FROM_TIME(FileStat.uiModifiedTime),FAT_GET_MIN_FROM_TIME(FileStat.uiModifiedTime),FAT_GET_SEC_FROM_TIME(FileStat.uiModifiedTime));
            buf+=len;
        }
    }
    len = sprintf(buf,"</LIST>\n");
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
#endif


int XML_GetModeData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32            len;
    char*             buf = (char*)bufAddr;
    UINT32            par =0;
    char              tmpString[32];

    // set the data mimetype
    strcpy(mimeType,"text/xml");
    DBG_IND("path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);

    sprintf(tmpString,"custom=1&cmd=%d&par=",WIFIAPP_CMD_MODECHANGE);
    if(strncmp(argument,tmpString,strlen(tmpString))==0)
    {
        sscanf(argument+strlen(tmpString),"%d",&par);
    }
    else
    {
        *bufSize =0;
        DBG_ERR("error par\r\n");
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    if (par == WIFI_APP_MODE_PLAYBACK)
    {
        return XML_PBGetData(path,argument,bufAddr,bufSize,mimeType,segmentCount);
    }
    else if (par == WIFI_APP_MODE_PHOTO)
    {
        len = sprintf(buf,DEF_XML_HEAD);
        buf+=len;
        len = sprintf(buf,"<FREEPICNUM>%d</FREEPICNUM>\n",UI_GetData(FL_WIFI_PHOTO_FREEPICNUM));
        buf+=len;
        *bufSize = (cyg_uint32)(buf)-bufAddr;

        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }
    else if (par == WIFI_APP_MODE_MOVIE)
    {
        len = sprintf(buf,DEF_XML_HEAD);
        buf+=len;
        len = sprintf(buf,"<MAXRECTIME>%d</MAXRECTIME>\n",UI_GetData(FL_WIFI_MOVIE_MAXRECTIME));
        buf+=len;
        *bufSize = (cyg_uint32)(buf)-bufAddr;
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    *bufSize =0;
     DBG_ERR("error mode\r\n");
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}




int XML_QueryCmd(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32            len, i=0;
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              pFilePath[64];
    UINT32            fileLen =*bufSize-512;
    UINT32            bufflen =*bufSize-512;
    WIFI_CMD_ENTRY*   appCmd=0;
    // set the data mimetype
    strcpy(mimeType,"text/xml");
    sprintf(pFilePath,"%s",QUERY_XML_PATH);  //html of all command list
    filehdl = FileSys_OpenFile(pFilePath,FST_OPEN_READ);
    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8 *)buf, &fileLen, 0,0);
        FileSys_CloseFile(filehdl);
        *bufSize = fileLen;
        *(buf+fileLen) ='\0';
    }
    else
    {
        if (segmentCount == 0)
        {
            len = sprintf(buf,DEF_XML_HEAD);
            buf+=len;
        }

        len = sprintf(buf,"<Function>\n");
        buf+=len;
        appCmd= WifiCmd_GetExecTable();
        while(appCmd[i].cmd != 0)
        {
            len = sprintf(buf,"<Cmd>%d</Cmd>\n",appCmd[i].cmd);
            buf+=len;
            i++;
        }
        len = sprintf(buf,"</Function>\n");
        buf+=len;

        *bufSize = (cyg_uint32)(buf)-bufAddr;
    }

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }


    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


FST_FILE          gMovFilehdl;
ER XML_VdoReadCB(UINT32 pos, UINT32 size, UINT32 addr)
{
    ER result =E_SYS;

    DBG_IND("XML_VdoReadCB  pos=0x%x, size=%d, addr=0x%x\r\n",pos,size,addr);
    if(gMovFilehdl)
    {
        FileSys_SeekFile(gMovFilehdl, pos, FST_SEEK_SET);
        //not close file,close file in XML_GetThumbnail()
        result=FileSys_ReadFile(gMovFilehdl, (UINT8 *)addr, &size, 0,0);
    }
    return result;
}

int XML_GetThumbnail(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              tempPath[128];
    UINT32            TempLen,TempBuf;
    UINT32            bufflen = *bufSize-512;
    UINT32            ThumbOffset =0;
    UINT32            ThumbSize =0;
    char*             pch;
    UINT32            result =0;

    if(!SxCmd_GetTempMem(POOL_SIZE_FILEDB))
    {
        XML_DefaultFormat(WIFIAPP_CMD_THUMB,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }



    // set the data mimetype
    strcpy(mimeType,"image/jpeg");
    XML_ecos2NvtPath(path, tempPath);

    pch=strchr(tempPath,'.');
    if((strncmp(pch+1,"mov",3)==0)||(strncmp(pch+1,"MOV",3)==0))
    {
        FST_FILE_STATUS FileStat;
        GXVIDEO_INFO VideoInfo;
        MEM_RANGE WorkBuf;
        UINT32 uiBufferNeeded=0;


        gMovFilehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
        if(gMovFilehdl)
        {
            FileSys_StatFile(gMovFilehdl,&FileStat);
            GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
            TempBuf = SxCmd_GetTempMem(uiBufferNeeded);
            //parse video info for single mode only
            WorkBuf.Addr = TempBuf;
            WorkBuf.Size = uiBufferNeeded;
            result = GxVidFile_ParseVideoInfo(XML_VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
            if((result==GXVIDEO_PRSERR_OK)&&(VideoInfo.uiThumbSize))
            {
                FileSys_SeekFile(gMovFilehdl, VideoInfo.uiThumbOffset, FST_SEEK_SET);
                FileSys_ReadFile(gMovFilehdl, (UINT8 *)buf, &VideoInfo.uiThumbSize, 0,0);
                FileSys_CloseFile(gMovFilehdl);
                *bufSize = VideoInfo.uiThumbSize;
            }
            else
            {
                FileSys_CloseFile(gMovFilehdl);
                result = WIFIAPP_RET_EXIF_ERR;
                DBG_ERR("exif error\r\n");
            }
        }
        else
        {
            result = WIFIAPP_RET_NOFILE;
            DBG_ERR("no %s\r\n",tempPath);
        }

    }
    else
    {

        filehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);

        if(filehdl)
        {
            MEM_RANGE ExifData;
            EXIF_GETTAG exifTag;
            BOOL bIsLittleEndian=0;
            UINT32 uiTiffOffsetBase=0;

            TempLen= MAX_APP1_SIZE;
            TempBuf = SxCmd_GetTempMem(TempLen);
            FileSys_ReadFile(filehdl, (UINT8 *)TempBuf, &TempLen, 0,0);
            FileSys_CloseFile(filehdl);

            ExifData.Size = MAX_APP1_SIZE;
            ExifData.Addr = TempBuf;
            if(EXIF_ER_OK == Exif_ParseExif(&ExifData))
            {
                Exif_GetInfo(EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
                Exif_GetInfo(EXIFINFO_TIFF_OFFSET_BASE, &uiTiffOffsetBase, sizeof(uiTiffOffsetBase));
                //find thumbnail
                exifTag.uiTagIfd = EXIF_IFD_1ST;
                exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT;
                if(E_OK == Exif_GetTag(&exifTag))
                {
                    ThumbOffset = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian) + uiTiffOffsetBase;
                    exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT_LENGTH;
                    if(E_OK == Exif_GetTag(&exifTag))
                        ThumbSize = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
                }
            }
            else
            {
                result = WIFIAPP_RET_EXIF_ERR;
                DBG_ERR("exif error\r\n");
            }

            if(ThumbSize)
            {
                if(ThumbSize<bufflen)
                {
                    hwmem_open();
                    hwmem_memcpy((UINT32)buf, TempBuf+ThumbOffset, ThumbSize);
                    hwmem_close();
                    *bufSize = ThumbSize;
                    //debug_msg("photo thumb trans %d\r\n",*bufSize);

                }
                else
                {
                    result = WIFIAPP_RET_NOBUF;
                    DBG_ERR("size too large\r\n");
                }
            }
        }
        else
        {
            result = WIFIAPP_RET_NOFILE;
            DBG_ERR("no %s\r\n",tempPath);
        }
    }

    if(result!=0)
        XML_DefaultFormat(WIFIAPP_CMD_THUMB,result,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetScreen(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              tempPath[128];
    UINT32            TempLen,TempBuf=0;
    UINT32            bufflen = *bufSize-512;
    UINT32            ScreenOffset =0;
    UINT32            ScreenSize =0;
    static UINT32     remain =0;
    static UINT32     ScreenBuf = 0;
    char*             pch;
    UINT32            result=0;

    if(!SxCmd_GetTempMem(POOL_SIZE_FILEDB))
    {
        XML_DefaultFormat(WIFIAPP_CMD_SCREEN,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }


    // set the data mimetype
    strcpy(mimeType,"image/jpeg");
    XML_ecos2NvtPath(path, tempPath);

    pch=strchr(tempPath,'.');
    if((strncmp(pch+1,"mov",3)==0)||(strncmp(pch+1,"MOV",3)==0))
    {
        if(segmentCount ==0)
        {
            FST_FILE_STATUS FileStat;
            GXVIDEO_INFO VideoInfo;
            MEM_RANGE WorkBuf;
            UINT32 uiBufferNeeded=0;

            gMovFilehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
            if(gMovFilehdl)
            {
                FileSys_StatFile(gMovFilehdl,&FileStat);
                GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
                TempBuf = SxCmd_GetTempMem(uiBufferNeeded);
                //parse video info for single mode only
                WorkBuf.Addr = TempBuf;
                WorkBuf.Size = uiBufferNeeded;
                result = GxVidFile_ParseVideoInfo(XML_VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
                if((result==GXVIDEO_PRSERR_OK)&&(VideoInfo.uiScraSize))
                {
                    FileSys_SeekFile(gMovFilehdl, VideoInfo.uiScraOffset, FST_SEEK_SET);
                    FileSys_ReadFile(gMovFilehdl, (UINT8 *)TempBuf, &VideoInfo.uiScraSize, 0,0);
                    FileSys_CloseFile(gMovFilehdl);
                    remain = VideoInfo.uiScraSize;
                    ScreenBuf = TempBuf;
                }
                else
                {
                    FileSys_CloseFile(gMovFilehdl);
                    result = WIFIAPP_RET_EXIF_ERR;
                    DBG_ERR("exif error\r\n");
                }
            }
            else
            {
                result = WIFIAPP_RET_NOFILE;
                DBG_ERR("no %s\r\n",tempPath);
            }

        }

    }
    else
    {

        if(segmentCount ==0)
        {
            filehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);

            if(filehdl)
            {
                MEM_RANGE ExifData;
                MAKERNOTE_INFO MakernoteInfo;

                TempLen= MAX_APP1_SIZE;
                TempBuf = SxCmd_GetTempMem(TempLen);
                FileSys_ReadFile(filehdl, (UINT8 *)TempBuf, &TempLen, 0,0);

                ExifData.Size = MAX_APP1_SIZE;
                ExifData.Addr = TempBuf;
                if(EXIF_ER_OK == Exif_ParseExif(&ExifData))
                {
                   //find screennail
                    Exif_GetInfo(EXIFINFO_MAKERNOTE, &MakernoteInfo, sizeof(MakernoteInfo));
                    ScreenOffset = MakernoteInfo.uiScreennailOffset;
                    ScreenSize = MakernoteInfo.uiScreennailSize;
                    //read screennail to buffer
                    ScreenBuf = SxCmd_GetTempMem(ScreenSize);
                    if(ScreenBuf)
                    {
                        FileSys_SeekFile(filehdl, ScreenOffset, FST_SEEK_SET);
                        FileSys_ReadFile(filehdl, (UINT8 *)ScreenBuf, &ScreenSize, 0,0);
                        FileSys_CloseFile(filehdl);
                        remain = ScreenSize;
                    }
                    else
                    {
                        FileSys_CloseFile(filehdl);
                        result = WIFIAPP_RET_NOBUF;
                        DBG_ERR("no TempBuf\r\n");
                    }
                }
                else
                {
                    result = WIFIAPP_RET_EXIF_ERR;
                    FileSys_CloseFile(filehdl);
                    DBG_ERR("exif error\r\n");
                }
            }
            else
            {
                result = WIFIAPP_RET_NOFILE;
                DBG_ERR("no %s\r\n",tempPath);
            }
        }

    }
    if(ScreenBuf)
    {
        if(remain>bufflen)
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, ScreenBuf+bufflen*segmentCount, bufflen);
            hwmem_close();
            *bufSize = bufflen;
            remain-=bufflen;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        else
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, ScreenBuf+bufflen*segmentCount, remain);
            hwmem_close();
            *bufSize = remain;
            remain=0;
            ScreenBuf =0;
            //debug_msg("last trans ok\r\n");
        }
    }

    if(result!=0)
        XML_DefaultFormat(WIFIAPP_CMD_SCREEN,result,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetRawEncJpg(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char*             buf = (char*)bufAddr;
    UINT32            bufflen = *bufSize-512;
    static UINT32     remain =0;
    static UINT32     uiJpgAddr = 0;
    UINT32            uiJpgSize = 0;
    UINT32            result=0;


    // set the data mimetype
    strcpy(mimeType,"image/jpeg");

    if(segmentCount ==0)
    {
        RawEnc_GetJpgData(&uiJpgAddr, &uiJpgSize, 0);
        if(uiJpgAddr)
        {
            remain = uiJpgSize;
        }
        else
        {
            result = WIFIAPP_RET_NOFILE;
            DBG_ERR("no JPG file\r\n");
        }
    }

    if(uiJpgAddr)
    {
        if(remain>bufflen)
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, uiJpgAddr+bufflen*segmentCount, bufflen);
            hwmem_close();
            *bufSize = bufflen;
            remain-=bufflen;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        else // last data.
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, uiJpgAddr+bufflen*segmentCount, remain);
            hwmem_close();
            *bufSize = remain;
            remain=0;
            uiJpgAddr =0;
            //debug_msg("last trans ok\r\n");
        }
    }

    if(result!=0)
        XML_DefaultFormat(WIFIAPP_CMD_MOVIE_GET_RAWENC_JPG, result, bufAddr, bufSize, mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetVersion(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_StringResult(WIFIAPP_CMD_VERSION,Prj_GetVersionString(),bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_FileList(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{

    PFILEDB_INIT_OBJ   pFDBInitObj = &gWifiFDBInitObj;
    FILEDB_HANDLE      FileDBHandle =0;

    Perf_Mark();

    pFDBInitObj->u32MemAddr = SxCmd_GetTempMem(POOL_SIZE_FILEDB);

    if(!pFDBInitObj->u32MemAddr)
    {
        XML_DefaultFormat(WIFIAPP_CMD_FILELIST,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
    FileDBHandle = FileDB_Create(pFDBInitObj);
    DBG_IND("createTime = %04d ms \r\n",Perf_GetDuration()/1000);
    Perf_Mark();
    FileDB_SortBy(FileDBHandle,FILEDB_SORT_BY_MODDATE,FALSE);
    DBG_IND("sortTime = %04d ms \r\n",Perf_GetDuration()/1000);

    //FileDB_DumpInfo(FileDBHandle);

{
    static UINT32     gIndex;
    UINT32            len, fileCount, i, bufflen = *bufSize;
    char*             buf = (char*)bufAddr;


    if (segmentCount == 0)
    {
        len = sprintf(buf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
        buf+=len;
    }

    fileCount = FileDB_GetTotalFileNum(FileDBHandle);
    debug_msg("fileCount  %d\r\n",fileCount);

    if (segmentCount == 0)
    {
        // reset some global variables
        gIndex = 0;
    }
    DBG_IND("gIndex = %d %d \r\n",gIndex,fileCount);
    for (i=gIndex;i<fileCount;i++)
    {
        PFILEDB_FILE_ATTR        pFileAttr;
        // check buffer length , reserved 512 bytes
        // should not write data over buffer length
        if ((cyg_uint32)buf-bufAddr > bufflen - 512)
        {
            DBG_IND("totallen=%d >  bufflen(%d)-512\r\n",(cyg_uint32)buf-bufAddr,bufflen);
            *bufSize = (cyg_uint32)(buf)-bufAddr;
            gIndex = i;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        // get dcf file
        pFileAttr = FileDB_SearhFile(FileDBHandle, i);

        debug_msg("file %s %d\r\n",pFileAttr->filePath,pFileAttr->fileSize);

        {
            len = sprintf(buf,"<ALLFile><File>\n<NAME>%s</NAME>\n<FPATH>%s</FPATH>\n",pFileAttr->filename,pFileAttr->filePath);
            buf+=len;
            len = sprintf(buf,"<SIZE>%d</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n<ATTR>%d</ATTR></File>\n</ALLFile>\n",pFileAttr->fileSize,(pFileAttr->lastWriteDate <<16) + pFileAttr->lastWriteTime,
                  FAT_GET_YEAR_FROM_DATE(pFileAttr->lastWriteDate),FAT_GET_MONTH_FROM_DATE(pFileAttr->lastWriteDate),FAT_GET_DAY_FROM_DATE(pFileAttr->lastWriteDate),
                  FAT_GET_HOUR_FROM_TIME(pFileAttr->lastWriteTime),FAT_GET_MIN_FROM_TIME(pFileAttr->lastWriteTime),FAT_GET_SEC_FROM_TIME(pFileAttr->lastWriteTime),
                  pFileAttr->attrib );
            buf+=len;
        }
    }
    len = sprintf(buf,"</LIST>\n");
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    FileDB_Release(FileDBHandle);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}




}

#define GetDVRStatusIsPlayMovie  1;
#define GetDVRStatusIsNotPlayMovie  0;


int XML_DVR_Info(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
#if 0

    UINT32            len;
   // bufflen = *bufSize;
    char*             buf = (char*)bufAddr;
	int FL_ISPLAY_NOW;

	

	 if ((System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) )
	 	{
			switch(g_PlbData.State)
			{
				case PLB_ST_PLAY_MOV:
				case PLB_ST_FWD_MOV:
				case PLB_ST_BWD_MOV:
				case PLB_ST_PAUSE_MOV:
					FL_ISPLAY_NOW = GetDVRStatusIsPlayMovie;
				break;
				default:
					FL_ISPLAY_NOW = GetDVRStatusIsNotPlayMovie;
					break;
			}
	 	}
	 else
	 	{
	 	FL_ISPLAY_NOW = GetDVRStatusIsNotPlayMovie;
	 	}

	 
        len = sprintf(buf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<MENUINFO>\n");
        buf+=len;


        
			//PHOTO MENU
            len = sprintf(buf,"<FL_PHOTO_SIZE>%d</FL_PHOTO_SIZE>\n<FL_QUALITY>%d</FL_QUALITY>\n<FL_COLOR_EFFECT>%d</FL_COLOR_EFFECT>\n<FL_SCENE>%d</FL_SCENE>\n<FL_EV>%d</FL_EV>\n<FL_ISO>%d</FL_ISO>\n<FL_WB>%d</FL_WB>\n<FL_SHARPNESS>%d</FL_SHARPNESS>\n<FL_SATURATION>%d</FL_SATURATION>\n<FL_ANTISHAKE>%d</FL_ANTISHAKE>\n<FL_DATE_STAMP>%d</FL_DATE_STAMP>\n<FL_CONTINUE_SHOT>%d</FL_CONTINUE_SHOT>\n",
				UI_GetData(FL_PHOTO_SIZE),UI_GetData(FL_QUALITY),UI_GetData(FL_COLOR_EFFECT),UI_GetData(FL_SCENE),
				UI_GetData(FL_EV),UI_GetData(FL_ISO),UI_GetData(FL_WB),UI_GetData(FL_SHARPNESS),
				UI_GetData(FL_SATURATION),UI_GetData(FL_ANTISHAKE),UI_GetData(FL_DATE_STAMP),UI_GetData(FL_CONTINUE_SHOT));
            buf+=len;

			//MOVIE MENU
	     len = sprintf(buf,"<FL_MOVIE_SIZE>%d</FL_MOVIE_SIZE>\n<FL_MOVIE_QUALITY>%d</FL_MOVIE_QUALITY>\n<FL_MOVIE_COLOR>%d</FL_MOVIE_COLOR>\n<FL_MOVIE_CYCLIC_REC>%d</FL_MOVIE_CYCLIC_REC>\n<FL_MOVIE_TIMELAPSE_REC>%d</FL_MOVIE_TIMELAPSE_REC>\n<FL_MOVIE_MOTION_DET>%d</FL_MOVIE_MOTION_DET>\n<FL_MOVIE_AUDIO>%d</FL_MOVIE_AUDIO>\n<FL_MOVIE_DATEIMPRINT>%d</FL_MOVIE_DATEIMPRINT>\n<FL_SENSOR_ROTATE>%d</FL_SENSOR_ROTATE>\n<FL_MOVIE_HDR>%d</FL_MOVIE_HDR>\n<FL_ISREC_NOW>%d</FL_ISREC_NOW>\n",
				UI_GetData(FL_MOVIE_SIZE),UI_GetData(FL_MOVIE_QUALITY),UI_GetData(FL_MOVIE_COLOR),UI_GetData(FL_MOVIE_CYCLIC_REC),
				UI_GetData(FL_MOVIE_TIMELAPSE_REC),UI_GetData(FL_MOVIE_MOTION_DET),UI_GetData(FL_MOVIE_AUDIO),UI_GetData(FL_MOVIE_DATEIMPRINT),
				UI_GetData(FL_SENSOR_ROTATE),UI_GetData(FL_MOVIE_HDR), UIFlowWndWiFiMovie_GetStatus());
            buf+=len;
			//PLAYBACK AND SYSTEM

		len = sprintf(buf,"<FL_ISPLAY_NOW>%d</FL_ISPLAY_NOW>\n<FL_DATE_TIME>%d</FL_DATE_TIME>\n<FL_BEEP>%d</FL_BEEP>\n<FL_FREQUENCY>%d</FL_FREQUENCY>\n<FL_TV_MODE>%d</FL_TV_MODE>\n<FL_DATE_FORMAT>%d</FL_DATE_FORMAT>\n<FL_ISBAT_LOW>%d</FL_ISBAT_LOW>\n<FL_ISNO_CARD>%d</FL_ISNO_CARD>\n",  
				FL_ISPLAY_NOW ,UI_GetData(FL_DATE_TIME),UI_GetData(FL_BEEP),UI_GetData(FL_FREQUENCY),
				UI_GetData(FL_TV_MODE),UI_GetData(FL_DATE_FORMAT), GxPower_GetControl(GXPWR_CTRL_BATTERY_LEVEL) , UI_GetData(FL_CardStatus) );
            buf+=len;	
             
       
    len = sprintf(buf,"</MENUINFO>\n");
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

   

    return CYG_HFS_CB_GETDATA_RETURN_OK;




#endif
}


void XML_DefaultFormat(UINT32 cmd,UINT32 ret,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    UINT32 len=0;
    UINT32 bufflen = *bufSize-512;
    char*  buf = (char*)bufAddr;

    strcpy(mimeType,"text/xml");
    len = sprintf(buf,DEF_XML_HEAD);
    buf+=len;
    len = sprintf(buf,DEF_XML_RET,cmd,ret);
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }
}

void XML_StringResult(UINT32 cmd,char *str,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    UINT32 len=0;
    UINT32 bufflen = *bufSize-512;
    char*  buf = (char*)bufAddr;

    strcpy(mimeType,"text/xml");
    len = sprintf(buf,DEF_XML_HEAD);
    buf+=len;
    len = sprintf(buf,DEF_XML_STR,cmd,0,str);//status OK
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }
}

void XML_ValueResult(UINT32 cmd,UINT64 value,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    UINT32 len=0;
    UINT32 bufflen = *bufSize-512;
    char*  buf = (char*)bufAddr;

    strcpy(mimeType,"text/xml");
    len = sprintf(buf,DEF_XML_HEAD);
    buf+=len;
    len = sprintf(buf,DEF_XML_VALUE,cmd,0,value);//status OK
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }
}

int XML_QueryCmd_CurSts(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    static UINT32     uiCmdIndex;
    UINT32            len/*, i=0*/;
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              pFilePath[64];
    UINT32            fileLen =*bufSize-512;
    UINT32            bufflen =*bufSize-512;
    WIFI_CMD_ENTRY *  appCmd=0;


    // set the data mimetype
    strcpy(mimeType,"text/xml");
    sprintf(pFilePath,"%s",QUERY_CMD_CUR_STS_XML_PATH);  //html of all command status list
    filehdl = FileSys_OpenFile(pFilePath,FST_OPEN_READ);
    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8 *)buf, &fileLen, 0,0);
        FileSys_CloseFile(filehdl);
        *bufSize = fileLen;
        *(buf+fileLen) ='\0';

        uiCmdIndex = 0;
    }
    else
    {
        if (segmentCount == 0)
        {
            len = sprintf(buf,DEF_XML_HEAD);
            buf+=len;
            uiCmdIndex = 0;
        }

        len = sprintf(buf,"<Function>\n");
        buf+=len;
        appCmd= WifiCmd_GetExecTable();
        while(appCmd[uiCmdIndex].cmd != 0)
        {
            if(appCmd[uiCmdIndex].UIflag != FL_NULL)
            {
                len = sprintf(buf, DEF_XML_CMD_CUR_STS, appCmd[uiCmdIndex].cmd, UI_GetData(appCmd[uiCmdIndex].UIflag));
                buf+=len;
                //debug_msg(DEF_XML_CMD_CUR_STS, appCmd[uiCmdIndex].cmd, UI_GetData(appCmd[uiCmdIndex].UIflag));
            }
            uiCmdIndex++;

            // check buffer length , reserved 512 bytes
            // should not write data over buffer length
            if ((cyg_uint32)buf-bufAddr > bufflen - 512)
            {
                DBG_IND("totallen=%d >  bufflen(%d)-512\r\n",(cyg_uint32)buf-bufAddr,bufflen);
                *bufSize = (cyg_uint32)(buf)-bufAddr;
                return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
            }
        }

        len = sprintf(buf,"</Function>\n");
        buf+=len;
        *bufSize = (cyg_uint32)(buf)-bufAddr;
    }

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }


    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetHeartBeat(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_DefaultFormat(WIFIAPP_CMD_HEARTBEAT,0,bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


int XML_GetRTPSTATUS(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_ValueResult(WIFIAPP_CMD_GETRTPSTATUS,GetRTPstatus(),bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetFreePictureNum(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_ValueResult(WIFIAPP_CMD_FREE_PIC_NUM,PhotoExe_GetFreePicNum(),bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
int XML_GetMaxRecordTime(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32 sec = UIFlowWiFiMovie_GetMaxRecTime();
    XML_ValueResult(WIFIAPP_CMD_MAX_RECORD_TIME,sec,bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetDiskFreeSpace(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    DBG_IND("FileSys_GetDiskInfo(FST_INFO_DISK_SIZE) %lld\r\n",FileSys_GetDiskInfo(FST_INFO_DISK_SIZE));
    XML_ValueResult(WIFIAPP_CMD_DISK_FREE_SPACE, FileSys_GetDiskInfo(FST_INFO_FREE_SPACE), bufAddr, bufSize, mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_DeleteOnePicture(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char fullPath[128] ={0};
    int ret;
    char*             pch=0;
    UINT8      attrib;

    pch=strrchr(argument,'&');
    if(strncmp(pch,PARS_STR,strlen(PARS_STR))==0)
    {
        sscanf(pch+strlen(PARS_STR),"%s",&fullPath);
    }
    DBG_IND("fullPath path=%s\r\n",fullPath);

    ret = FileSys_GetAttrib(fullPath,&attrib);
    if ( (ret == E_OK) && (M_IsReadOnly(attrib) == TRUE) )
        {
            DBG_IND("File Locked\r\n");
            XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,WIFIAPP_RET_FILE_LOCKED,bufAddr,bufSize,mimeType);
    }
    else if (ret == FST_STA_FILE_NOT_EXIST)
    {
        DBG_IND("File not existed\r\n");
        XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,WIFIAPP_RET_NOFILE,bufAddr,bufSize,mimeType);
        }
        else
        {
        ret = FileSys_DeleteFile(fullPath);
            DBG_IND("ret = %d\r\n", ret);
            if (ret != FST_STA_OK)
            {
            XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,WIFIAPP_RET_FILE_ERROR,bufAddr,bufSize,mimeType);
            }
            else
            {
            #if 0
            Index = FileDB_GetIndexByPath(FileDBHdl,fullPath);
            DBG_IND("Index = %04d\r\n",Index);
            if (Index != FILEDB_SEARCH_ERR)
            {
                FileDB_DeleteFile(FileDBHdl,Index);
            }
            #endif

            XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,WIFIAPP_RET_OK,bufAddr,bufSize,mimeType);
        }
    }

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_DeleteAllPicture(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char  tempPath[128];
    BOOL  isCurrFileReadOnly = FALSE;
    UINT32 Index;
    PFILEDB_FILE_ATTR FileAttr = NULL;
    INT32             ret;
    INT32             FileNum,i;
    PFILEDB_INIT_OBJ   pFDBInitObj = &gWifiFDBInitObj;
    FILEDB_HANDLE      FileDBHandle =0;

    Perf_Mark();

    pFDBInitObj->u32MemAddr = SxCmd_GetTempMem(POOL_SIZE_FILEDB);
    if(!pFDBInitObj->u32MemAddr)
    {
        XML_DefaultFormat(WIFIAPP_CMD_DELETE_ALL,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }
    pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
    FileDBHandle = FileDB_Create(pFDBInitObj);
    DBG_IND("createTime = %04d ms \r\n",Perf_GetDuration()/1000);
    Perf_Mark();
    FileDB_SortBy(FileDBHandle,FILEDB_SORT_BY_MODDATE,FALSE);
    DBG_IND("sortTime = %04d ms \r\n",Perf_GetDuration()/1000);

    FileAttr = FileDB_CurrFile(FileDBHandle);
    if( FileAttr && M_IsReadOnly(FileAttr->attrib))
    {
        isCurrFileReadOnly = TRUE;
        strncpy(tempPath,FileAttr->filePath,sizeof(tempPath));
    }

    FileNum = FileDB_GetTotalFileNum(FileDBHandle);
    for (i=FileNum-1;i>=0;i--)
    {
        FileAttr = FileDB_SearhFile(FileDBHandle,i);
        if (FileAttr)
        {
            if (M_IsReadOnly(FileAttr->attrib))
            {
                //DBG_IND("File Locked\r\n");
                DBG_IND("File %s is locked\r\n", FileAttr->filePath);
                continue;
            }
            ret = FileSys_DeleteFile(FileAttr->filePath);
            //DBG_IND("i = %04d path=%s\r\n",i,FileAttr->filePath);
            if (ret != FST_STA_OK)
            {
                DBG_IND("Delete %s failed\r\n", FileAttr->filePath);
            }
            else
            {
                FileDB_DeleteFile(FileDBHandle,i);
                DBG_IND("Delete %s OK\r\n", FileAttr->filePath);
            }
        }
        else
        {
            DBG_IND("%s not existed\r\n", FileAttr->filePath);
        }
    }

    if (isCurrFileReadOnly)
    {
       Index = FileDB_GetIndexByPath(FileDBHandle,tempPath);
       FileDB_SearhFile(FileDBHandle,Index);
    }

    FileDB_Release(FileDBHandle);

    XML_DefaultFormat(WIFIAPP_CMD_DELETE_ALL,WIFIAPP_RET_OK,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


int XML_GetPictureEnd(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32            len;
    char*             buf = (char*)bufAddr;
    UINT32            result =0;
    UINT32            FreePicNum=0;
    char*             pFilePath=0;
    char*             pFileName=0;
    // set the data mimetype
    strcpy(mimeType,"text/xml");
    DBG_IND("path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);


    if(UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL))
    {
        result = WIFIAPP_RET_FOLDER_FULL;
        DBG_ERR("folder full\r\n");
    }
    else if(UIStorageCheck(STORAGE_CHECK_FULL, &FreePicNum))
    {
        result = WIFIAPP_RET_STORAGE_FULL;
        DBG_ERR("storage full\r\n");
    }
    else if(UI_GetData(FL_FSStatus)== FS_DISK_ERROR)
    {
        result = WIFIAPP_RET_FILE_ERROR;
        DBG_ERR("write file fail\r\n");
    }
    else
    {
        pFilePath = DscPhoto_GetLastWriteFilePath();
        //DBG_ERR("path %s %d",pFilePath,strlen(pFilePath));
        if(strlen(pFilePath))
        {
            pFileName = strrchr(pFilePath,'\\')+1;
        } 
        else
        {
            result = WIFIAPP_RET_FILE_ERROR;
            DBG_ERR("write file fail\r\n");
        }
    }
    if(result == 0)
    {
        len = sprintf(buf,DEF_XML_HEAD);
        buf+=len;
        len = sprintf(buf,"<Function>\n");
        buf+=len;
        len = sprintf(buf,DEF_XML_CMD_CUR_STS,WIFIAPP_CMD_CAPTURE,result);
        buf+=len;
        len = sprintf(buf,"<File>\n<NAME>%s</NAME>\n<FPATH>%s</FPATH></File>\n",pFileName,pFilePath);
        buf+=len;
        len = sprintf(buf,"<FREEPICNUM>%d</FREEPICNUM>\n",FreePicNum);
        buf+=len;
        len = sprintf(buf,"</Function>\n");
        buf+=len;
        *bufSize = (cyg_uint32)(buf)-bufAddr;
    }
    else
    {
        XML_DefaultFormat(WIFIAPP_CMD_CAPTURE,result,bufAddr,bufSize,mimeType);
    }
    DBG_ERR("%s",bufAddr);
    return CYG_HFS_CB_GETDATA_RETURN_OK;

}

int XML_GetBattery(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_ValueResult(WIFIAPP_CMD_GET_BATTERY,GetBatteryLevel(),bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
int XML_HWCapability(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32 value =0;
    #if(TV_FUNC==ENABLE)
    value |= HW_TV_ENABLE;
    #endif
    #if (GSENSOR_FUNCTION == ENABLE)
    value |= HW_GSENDOR_ENABLE;
    #endif
    //DBG_ERR("XML_HWCapability value %x\r\n",value);
    XML_ValueResult(WIFIAPP_CMD_GET_HW_CAP,value,bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


int XML_GetMovieRecStatus(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    if(UIFlowWndWiFiMovie_GetStatus() == WIFI_MOV_ST_RECORD )
        XML_ValueResult(WIFIAPP_CMD_MOVIE_RECORDING_TIME,FlowMovie_GetRecCurrTime(),bufAddr,bufSize,mimeType);
    else
        XML_ValueResult(WIFIAPP_CMD_MOVIE_RECORDING_TIME,0,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetCardStatus(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    // check card inserted
    XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, UI_GetData(FL_CardStatus), bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetDownloadURL(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_StringResult(WIFIAPP_CMD_GET_DOWNLOAD_URL, WIFI_APP_DOWNLOAD_URL, bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetUpdateFWPath(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char url[128];
    char FWPath[64];

    XML_Nvt2ecosPath(FW_UPDATE_NAME,FWPath);
    sprintf(url,"http://%s%s",UINet_GetIP(),(char *)FWPath);

    XML_StringResult(WIFIAPP_CMD_GET_UPDATEFW_PATH, url, bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


int WifiCmd_getQueryData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{

    UINT32 len=0;
    UINT32 bufflen = *bufSize-512;
    char*  buf = (char*)bufAddr;

    DBG_DUMP("Data2 path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);

    strcpy(mimeType,"text/xml");

    len = sprintf(buf,QUERY_FMT,WIFI_APP_MANUFACTURER,1,WIFI_APP_MODLE);
    DBG_DUMP(buf);
    buf+=len;

    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }
    return CYG_HFS_CB_GETDATA_RETURN_OK;

}
#endif
