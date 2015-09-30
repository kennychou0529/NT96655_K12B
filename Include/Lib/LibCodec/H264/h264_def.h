#ifndef _H264_DEF_H_
#define _H264_DEF_H_

#include "Type.h"


#define H264ENC_NUM_REF         2   /*!< number of reference buffer in encoder.   */
#define H264ENC_NUM_BREC        2   /*!< number of B frame reconstruction buffer. */

#define H264ENC_BS_OUTBUF_MAX_SIZE 0x200000

#define H264ENC_HEADER_MAXSIZE  256

#define VIR_2_PHY_ADDR(tmp)     (tmp & 0x1FFFFFFF)

#define H264ENC_SUCCESS         1
#define H264ENC_FAIL            0

#define NALU_TYPE_SLICE    1
#define NALU_TYPE_DPA      2
#define NALU_TYPE_DPB      3
#define NALU_TYPE_DPC      4
#define NALU_TYPE_IDR      5
#define NALU_TYPE_SEI      6
#define NALU_TYPE_SPS      7
#define NALU_TYPE_PPS      8
#define NALU_TYPE_AUD      9
#define NALU_TYPE_EOSEQ    10
#define NALU_TYPE_EOSTREAM 11
#define NALU_TYPE_FILL     12


typedef enum
{
    PROFILE_BASELINE      =  66,
    PROFILE_MAIN          =  77,
    PROFILE_EXTENDED      =  88,
    PROFILE_HIGH          = 100,
    PROFILE_HIGH10        = 110,
    PROFILE_HIGH422       = 122,
    PROFILE_HIGH444       = 244,
    PROFILE_CAVLC444      =  44,

}PROFILE_TYPE;


typedef enum
{
    CAVLC = 0,
    CABAC = 1,
}ENTROPY_CODING_MODE;

typedef enum
{
    INTERLACED_FIELD = 0,
    PROGRESSIVE_FRAME = 1,
}FRAME_MSB_FLAG;

typedef enum
{
    LIST_MODE_RESERVED = 0,
    L0_PREDICTION = 1,
    L1_PREDICTION = 2,
    L0_AND_L1_PREDICTION = 3,
}PIC_LIST_MODE;

typedef enum
{
    TRANS_8X8_DISABLE = 0,
    TRANS_8X8_ENABLE = 1,
}TRANS_8X8_MODE;

typedef enum
{
    P_SLICE = 0,
    B_SLICE = 1,
    I_SLICE = 2,
    IDR_SLICE = 3,
}SLICE_TYPE;

#endif
