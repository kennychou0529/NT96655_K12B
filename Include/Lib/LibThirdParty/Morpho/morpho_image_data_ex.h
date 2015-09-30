/**
 * @file     morpho_image_data_ex.h
 * @brief    Image data structure definition
 * @version  1.0.0
 * @date     2010-03-30
 *
 * Copyright (C) 2010-2012 Morpho, Inc.
 */

#ifndef MORPHO_IMAGE_DATA_EX_H
#define MORPHO_IMAGE_DATA_EX_H

#include "morpho_image_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    int y;
    int u;
    int v;
} morpho_ImageYuvPlanarPitch;
    
typedef struct{
    int y;
    int uv;
} morpho_ImageYuvSemiPlanarPitch;

/** image data. */
typedef struct {
    int width;              /**< Width */
    int height;             /**< Height */
    union{
        void *p;            /**< Pointer for image data */
        morpho_ImageYuvPlanar planar;
        morpho_ImageYuvSemiPlanar semi_planar;
    } dat;
    union{
        int p;              /**< Line offset */
        morpho_ImageYuvPlanarPitch planar;
        morpho_ImageYuvSemiPlanarPitch semi_planar;
    } pitch;
} morpho_ImageDataEx;


#ifdef __cplusplus
}
#endif

#endif /* #ifndef MORPHO_IMAGE_DATA_EX_H */
