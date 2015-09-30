/**
 * @file     morpho_get_image_size.h
 * @brief    Function to get the memory size required for image
 * @version  1.0.0
 * @date     2008-07-01
 * 
 * Copyright (C) 2006-2012 Morpho, Inc.
 */

#ifndef MORPHO_GET_IMAGE_SIZE_H
#define MORPHO_GET_IMAGE_SIZE_H

#include "morpho_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the memory size required to store the image
 *
 * @param width       Image width
 * @param height      Image height
 * @param p_format    Image format string
 * @return            Memory size required for image
 */
#define morpho_getImageSize mor_istab3_IF_getImageSize

MORPHO_API(int)
morpho_getImageSize(int width, int height, const char *p_format);

/**
 * Get the Y image data size
 *
 * @param width       Image width
 * @param height      Image height
 * @param p_format    Image format string
 * @return            Y image data size
 */
#define morpho_getImageSizeY mor_istab3_IF_getImageSizeY

MORPHO_API(int)
morpho_getImageSizeY(int width, int height, const char *p_format);

/**
 * Get the U image data size
 *
 * @param width       Image width
 * @param height      Image height
 * @param p_format    Image format string
 * @return            U image data size
 */
#define morpho_getImageSizeU mor_istab3_IF_getImageSizeU

MORPHO_API(int)
morpho_getImageSizeU(int width, int height, const char *p_format);

/**
 * Get the V image data size
 *
 * @param width       Image width
 * @param height      Image height
 * @param p_format    Image format string
 * @return            V image data size
 */
#define morpho_getImageSizeV mor_istab3_IF_getImageSizeV

MORPHO_API(int)
morpho_getImageSizeV(int width, int height, const char *p_format);

/**
 * Get the UV image data size
 *
 * @param width       Image width
 * @param height      Image height
 * @param p_format    Image format string
 * @return            UV image data size
 */
#define morpho_getImageSizeUV mor_istab3_IF_getImageSizeUV

MORPHO_API(int)
morpho_getImageSizeUV(int width, int height, const char *p_format);


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MORPHO_GET_IMAGE_SIZE_H */
