/**
    Auto Exposure parameter.

    ae parameter.

    @file       ae_sample_param.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "Awb_alg.h"
#include "Awb_IMX322LQJ_EVB_FF_int.h"

#define AWB_CT_8500K_RGAIN 392
#define AWB_CT_8500K_BGAIN 274

#define AWB_CT_7000K_RGAIN 455
#define AWB_CT_7000K_BGAIN 404

#define AWB_CT_6000K_RGAIN 504
#define AWB_CT_6000K_BGAIN 504

#define AWB_CT_5000K_RGAIN 400
#define AWB_CT_5000K_BGAIN 500

#define AWB_CT_4300K_RGAIN 371
#define AWB_CT_4300K_BGAIN 565

#define AWB_CT_4000K_RGAIN 356
#define AWB_CT_4000K_BGAIN 598

#define AWB_CT_3700K_RGAIN 342 //*
#define AWB_CT_3700K_BGAIN 630

#define AWB_CT_3500K_RGAIN 284
#define AWB_CT_3500K_BGAIN 748

#define AWB_CT_3000K_RGAIN 227 //*
#define AWB_CT_3000K_BGAIN 865

//#NT#2013/09/05#Spark Chou -begin
//#NT#
#if AWB_REFWHITE_REMAP
AWBALG_ELEMET WhiteIndoorElement[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (160<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     255,  86, {  5, 10}, {  5, 10}},
    {AWBALG_TYPE_BG,     180, 140, {  5, 10}, {  5, 10}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement[] =
{
    {AWBALG_TYPE_Y,      (160<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     141,  86, {  5, 10}, {  5, 10}},
    {AWBALG_TYPE_BG,     180, 140, {  5, 10}, {  5, 10}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

#else
//0.97, 0.03, 0.07
AWBALG_ELEMET WhiteIndoorElement[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     216,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     371, 142, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    414,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 469, 302, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   426, 252, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     216, 140, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     216, 142, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    136,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 433, 302, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   426, 252, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};
#endif
//#NT#2013/09/05#Spark Chou -end

AWBALG_TAB AwbWhiteTAB[AWB_MAX_WHITE_TAB] =
{
    {(1<<10),  (1<<4),  WhiteIndoorElement,  AWBALG_ENV_InDoor},
    {(1<<28),  (1<<14), WhiteOutdoorElement, AWBALG_ENV_OutDoor}
};

AWBALG_PDLTAB AwbPDLTAB[AWB_MAX_PDL_TAB]=
{

};

//#NT#2013/09/05#Spark Chou -begin
//#NT#
AWBALG_CT_PARAM AwbCTParam =
{
    (1<<14),      // HCTMaxEV; LV14
    (1<<10),      // HCTMinEV; LV8
    {AWB_CT_6000K_RGAIN, AWB_CT_6000K_BGAIN},   // HCTMinGain; 5500K
    {AWB_CT_8500K_RGAIN, AWB_CT_8500K_BGAIN},   // HCTMaxGain; 8500K
    (1<<14),      // LCTMaxEV; LV14
    (1<<10),      // LCTMinEV; LV8
    {AWB_CT_4300K_RGAIN, AWB_CT_4300K_BGAIN},   // LCTMaxGain; 4000K
    {AWB_CT_3000K_RGAIN, AWB_CT_3000K_BGAIN},   // LCTMinGain; 2300K
};

AWBALG_POSTPROC_PARAM AwbPostParam =
{
    100,  100,        // HCTRatio; //100~200
    {AWB_CT_7000K_RGAIN, AWB_CT_7000K_BGAIN},   // HCTMaxGain;
    {AWB_CT_8500K_RGAIN, AWB_CT_8500K_BGAIN},   // HCTMinGain;
    120,  90,        // LCTRatio;  //100~200
    {AWB_CT_3700K_RGAIN, AWB_CT_3700K_BGAIN},   // LCTMaxGain; 4500K
    {AWB_CT_3000K_RGAIN, AWB_CT_3000K_BGAIN},   // LCTMinGain; 2556K
};
//#NT#2013/09/05#Spark Chou -end

//const UINT32 MwbTAB[AWB_MODE_MAX][2] =
UINT32 MwbTAB[AWB_MODE_MAX][2] =
{
    { 256, 256},
    { 438, 297},//AWB_MODE_DAYLIGHT
    { 375, 340},//AWB_MODE_CLOUDY
    { (197*17/10), (583*7/10)},//AWB_MODE_TUNGSTEN
    { 322, 409},//AWB_MODE_FLUORESCENT1
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}
};


