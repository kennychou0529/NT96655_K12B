/*******************************************************************
 * morpho_easy_hdr.h
 *------------------------------------------------------------------
 * Copyright (C) 2010-2012 Morpho,Inc.
 *******************************************************************/

#ifndef MORPHO_EASY_HDR_H
#define MORPHO_EASY_HDR_H

/*******************************************************************/

#include "morpho_api.h"
#include "morpho_error.h"
#include "morpho_image_data.h"

/*******************************************************************/

#define MORPHO_EASY_HDR_VER "Morpho EasyHDR Ver.2.1.14 2012/11/06"

/*-----------------------------------------------------------------*/

/* (input-limitaions) */

#define MORPHO_EASY_HDR_MIN_IMAGE_WIDTH    100
#define MORPHO_EASY_HDR_MAX_IMAGE_WIDTH   8192
#define MORPHO_EASY_HDR_MIN_IMAGE_HEIGHT   100
#define MORPHO_EASY_HDR_MAX_IMAGE_HEIGHT  8192
#define MORPHO_EASY_HDR_MIN_NIMAGES   2
#define MORPHO_EASY_HDR_MAX_NIMAGES  10

/*-----------------------------------------------------------------*/

/* (merge-status) */

#define MORPHO_EASY_HDR_OK                             0x00000000
#define MORPHO_EASY_HDR_ERROR_IMAGE_ALIGNMENT_FAILURE  0x00000001
#define MORPHO_EASY_HDR_ERROR_EXP_ESTIMATION_FAILURE   0x00000002
#define MORPHO_EASY_HDR_ERROR_MOSTLY_GHOST             0x00000004
#define MORPHO_EASY_HDR_ERROR_INTERNAL                 0x80000000

/*-----------------------------------------------------------------*/

/* (parameter-defines) */

#define MORPHO_EASY_HDR_DISABLED  0
#define MORPHO_EASY_HDR_ENABLED   1

/*-----------------------------------------------------------------*/

/* (parameter-limitations) */

#define MORPHO_EASY_HDR_GHOST_DETECTION_SENSITIVITY_LEVEL_MIN   0
#define MORPHO_EASY_HDR_GHOST_DETECTION_SENSITIVITY_LEVEL_MAX  10

#define MORPHO_EASY_HDR_RELIABLE_RECT_RATE_THRESHOLD_MIN    0
#define MORPHO_EASY_HDR_RELIABLE_RECT_RATE_THRESHOLD_MAX  100

#define MORPHO_EASY_HDR_GHOST_RATE_THRESHOLD_MIN    0
#define MORPHO_EASY_HDR_GHOST_RATE_THRESHOLD_MAX  100

#define MORPHO_EASY_HDR_FM_CC_LEVEL_MIN    0
#define MORPHO_EASY_HDR_FM_CC_LEVEL_MAX  500

/*-----------------------------------------------------------------*/

/* (parameter-defaults) */

#define MORPHO_EASY_HDR_FAIL_SOFT_MERGING_DEFAULT  MORPHO_EASY_HDR_ENABLED

#define MORPHO_EASY_HDR_GHOST_DETECTION_SENSITIVITY_LEVEL_DEFAULT  7

#define MORPHO_EASY_HDR_RELIABLE_RECT_RATE_THRESHOLD_DEFAULT  80

#define MORPHO_EASY_HDR_GHOST_RATE_THRESHOLD_DEFAULT  85

#define MORPHO_EASY_HDR_FM_CC_CONTRAST_LEVEL_DEFAULT    8
#define MORPHO_EASY_HDR_FM_CC_SATURATION_LEVEL_DEFAULT  2

/*******************************************************************/

typedef struct _morpho_EasyHDR morpho_EasyHDR;

/*-----------------------------------------------------------------*/

/** EasyHDR */
struct _morpho_EasyHDR
{
    void *p; /** Pointer to Morpho HDR internal processes */
};

/*******************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*-----------------------------------------------------------------*/

/**
 * Gets the version information of the Morpho HDR library.
 *
 * @return Version information string of Morpho HDR library
 */
MORPHO_API(char const *)
morpho_EasyHDR_getVersion(void);

/*-----------------------------------------------------------------*/

/**
 * Allocates the memory area size required for running Morpho HDR.
 *
 * @param[in]  max_width   Maximum width of input image
 * @param[in]  max_height  Maximum height of input image
 * @param[in]  format      Image format
 *
 * @return The size of the memory area required for executing Morpho HDR
 */
MORPHO_API(int)
morpho_EasyHDR_getBufferSize(
    int max_width,
    int max_height,
    char const *format);

/**
 * Initializes Morpho HDR.
 *
 * @param[in,out]  p            Pointer to the Morpho HDR object
 * @param[out]     buffer       Pointer to the memory allocated based on the byte size obtained by morpho_EasyHDR_getBufferSize
 * @param[in]      buffer_size  Byte size obtained by morpho_EasyHDR_getBufferSize
 * @param[in]      nthreads     Number of threads to use for processing
 * @param[in]      reserved     Callback function group
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_initialize(
    morpho_EasyHDR *p,
    void *buffer,
    int buffer_size,
    int nthreads,
    void const *reserved);

/**
 * Finalizes Morpho HDR.
 *
 * @param[in,out]  p  Pointer to the Morpho HDR object
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_finalize(
    morpho_EasyHDR *p);

/*-----------------------------------------------------------------*/

/**
 * Starts the HDR imaging process.
 *
 * @param[in,out]  p             Pointer to the Morpho HDR object
 * @param[out]     output_image  Pointer to the output image
 * @param[in,out]  input_images  Pointer to the input images
 * @param[in]      nimages       Number of input images
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_merge(
    morpho_EasyHDR *p,
    morpho_ImageData *output_image,
    morpho_ImageData *input_images[],
    int nimages);

/*-----------------------------------------------------------------*/

/**
 * Starts the HDR imaging process.
 *
 * @param[in,out]  p  Pointer to the Morpho HDR object
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_process(
    morpho_EasyHDR *p);

/*-----------------------------------------------------------------*/

/**
 * Suspends the HDR imaging process. (for batch execution only)
 *
 * @param[in,out]  p  Pointer to the Morpho HDR object
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_suspend(
    morpho_EasyHDR *p);

/**
 * Resumes the HDR imaging process. (for batch execution only)
 *
 * @param[in,out]  p  Pointer to the Morpho HDR object
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_resume(
    morpho_EasyHDR *p);

/*-----------------------------------------------------------------*/

/**
 * Gets the merging status.
 *
 * @param[in,out]  p  Pointer to the Morpho HDR object
 *
 * @return Status code
 */
MORPHO_API(int)
morpho_EasyHDR_getMergeStatus(
    morpho_EasyHDR *p);

/*-----------------------------------------------------------------*/

/**
 * Sets the image format.
 *
 * @param[in,out]  p       Pointer to the Morpho HDR object
 * @param[in]      format  Image format
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setImageFormat(
    morpho_EasyHDR *p,
    char const *format);

/**
 * Gets the image format setting.
 *
 * @param[in,out]  p            Pointer to the Morpho HDR object
 * @param[out]     buffer       Buffer where string describing the image format is stored.
 * @param[in]      buffer_size  Buffer size (including terminal character)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getImageFormat(
    morpho_EasyHDR *p,
    char *buffer,
    int buffer_size);

/*-----------------------------------------------------------------*/

/**
 * Sets whether Fail-soft-merging is enabled or not.
 *
 * The following values can be retrieved.
 * - MORPHO_EASY_HDR_ENABLED  (1)  Fail-soft-Merging enabled.
 * - MORPHO_EASY_HDR_DISABLED (0)  Fail-soft-Merging disabled.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setFailSoftMergingStatus(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets whether Fail-soft-merging is enabled or not.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Set value
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getFailSoftMergingStatus(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets the ghost detection sensitivity level.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set (between 0 and 10)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setGhostDetectionSensitivityLevel(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets the ghost detection sensitivity level.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Set value
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getGhostDetectionSensitivityLevel(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets the threshold value for valid rectangle.
 *
 * @param[in,out]  p     Pointer to the Morpho HDR object
 * @param[in]      rate  Valid area(center rate % rectangle)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setReliableRectRateThreshold(
    morpho_EasyHDR *p,
    int rate);

/**
 * Gets the threshold value for valid rectangle.
 *
 * @param[in,out]  p     Pointer to the Morpho HDR object
 * @param[out]     rate  Valid area(center rate % rectangle)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getReliableRectRateThreshold(
    morpho_EasyHDR *p,
    int *rate);

/*-----------------------------------------------------------------*/

/**
 * Sets the threshold value for ghost rate.
 *
 * @param[in,out]  p     Pointer to the Morpho HDR object
 * @param[in]      rate  Value to be set(between 0 and 100)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setGhostRateThreshold(
    morpho_EasyHDR *p,
    int rate);

/**
 * Gets the threshold value for ghost rate.
 *
 * @param[in,out]  p     Pointer to the Morpho HDR object
 * @param[out]     rate  Set Value
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getGhostRateThreshold(
    morpho_EasyHDR *p,
    int *rate);

/*-----------------------------------------------------------------*/

/**
 * Sets the color correction level (for new merging method)
 *
 * @param[in,out]  p                 Pointer to the Morpho HDR object
 * @param[in]      contrast_level    Value for contrast level (between 0 and 500)
 * @param[in]      saturation_level  Value for saturation level (between 0 and 500)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setFMColorCorrectionLevels(
    morpho_EasyHDR *p,
    int contrast_level,
    int saturation_level);

/**
 * Gets the color correction level (for new merging method).
 *
 * @param[in,out]  p                 Pointer to the Morpho HDR object
 * @param[out]     contrast_level    Set value for contrast level
 * @param[out]     saturation_level  Set value for saturation level
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getFMColorCorrectionLevels(
    morpho_EasyHDR *p,
    int *contrast_level,
    int *saturation_level);

/*-----------------------------------------------------------------*/

#ifdef __cplusplus
} /* extern "C" */
#endif

/*******************************************************************/

#endif /* !MORPHO_EASY_HDR_H */

/*******************************************************************/
/* [EOF] */
