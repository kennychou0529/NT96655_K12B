/**
    Motion Detection api.


    @file       ie_api.h
    @ingroup    mILibMDAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IE_API_H_
#define _IE_API_H_

typedef struct
{
    UINT32 format;
    UINT32 width;
    UINT32 height;
    UINT32 pY;
    UINT32 pU;
    UINT32 pV;
} IE_YUV_INFO;

extern ER IE_PerformImg(UINT32 filterID, IE_YUV_INFO srcInfo, IE_YUV_INFO patInfo, IE_YUV_INFO destInfo, UINT32 WorkingBufAddr);


#endif //_MD_API_H_
