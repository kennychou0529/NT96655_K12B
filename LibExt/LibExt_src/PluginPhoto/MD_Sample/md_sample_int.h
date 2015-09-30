/**
    Motion Detection Sample.

    

    @file       MDAlg.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _MD_SAMPLE_H_
#define _MD_SAMPLE_H_

#if 0
#define MD_CAWIN_X           16//16//32
#define MD_CAWIN_Y          16 //16//32
#define MD_CAWIN_TOTAL       (MD_CAWIN_X * MD_CAWIN_Y)
#define MD_BLKDIFF_THR       30//60//160
#define MD_BLKCNT_THR        0//64
#define MD_DIFFSUM_THR     400//800//1536 //3072
#endif
#if 1
#define MD_CAWIN_X           32
#define MD_CAWIN_Y           32
#define MD_CAWIN_TOTAL       (MD_CAWIN_X * MD_CAWIN_Y)
#define MD_BLKDIFF_THR       160
#define MD_BLKCNT_THR        64
#define MD_DIFFSUM_THR       3072
#endif
#endif //_MD_SAMPLE_H_
