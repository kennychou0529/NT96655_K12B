/*******************************************************************
 * morpho_easy_hdr_internal.h
 *------------------------------------------------------------------
 * Copyright (C) 2010-2012 Morpho,Inc.
 *******************************************************************/

#ifndef MORPHO_EASY_HDR_INTERNAL_H
#define MORPHO_EASY_HDR_INTERNAL_H

/*******************************************************************/

#include "morpho_api.h"
#include "morpho_error.h"
#include "morpho_image_data.h"
#include "morpho_image_data_ex.h"
#include "morpho_rect_int.h"

/*******************************************************************/

/* (parameter-limitations) */

#define MORPHO_EASY_HDR_GHOST_REMOVAL_STRENGTH_LEVEL_MIN   0
#define MORPHO_EASY_HDR_GHOST_REMOVAL_STRENGTH_LEVEL_MAX  10

#define MORPHO_EASY_HDR_MERGE_SMOOTHNESS_LEVEL_MIN   0
#define MORPHO_EASY_HDR_MERGE_SMOOTHNESS_LEVEL_MAX  10

#define MORPHO_EASY_HDR_MERGE_PARAM_MIN        0
#define MORPHO_EASY_HDR_MERGE_PARAM_MAX      255

#define MORPHO_EASY_HDR_FM_PARAM_MIN        0
#define MORPHO_EASY_HDR_FM_PARAM_MAX      255

#define MORPHO_EASY_HDR_FM_SIGMA_MIN       1
#define MORPHO_EASY_HDR_FM_SIGMA_MAX     300

#define MORPHO_EASY_HDR_FM_XSIZE_PERMIL_MIN        0
#define MORPHO_EASY_HDR_FM_XSIZE_PERMIL_MAX     1000

#define MORPHO_EASY_HDR_CC_OFFSET_MIN    0
#define MORPHO_EASY_HDR_CC_OFFSET_MAX  255

#define MORPHO_EASY_HDR_CC_GAIN_MIN   100
#define MORPHO_EASY_HDR_CC_GAIN_MAX  2000

#define MORPHO_EASY_HDR_CC_GAMMA_MIN   100
#define MORPHO_EASY_HDR_CC_GAMMA_MAX  2000

/*-----------------------------------------------------------------*/

/* (parameter-defaults) */

#define MORPHO_EASY_HDR_GHOST_REMOVAL_STRENGTH_LEVEL_DEFAULT  6

#define MORPHO_EASY_HDR_IMAGE_ALIGNMENT_DEFAULT    MORPHO_EASY_HDR_ENABLED
#define MORPHO_EASY_HDR_GHOST_REMOVAL_DEFAULT      MORPHO_EASY_HDR_ENABLED
#define MORPHO_EASY_HDR_AUTO_SCALING_DEFAULT       MORPHO_EASY_HDR_ENABLED
#define MORPHO_EASY_HDR_FINE_MERGING_DEFAULT       MORPHO_EASY_HDR_ENABLED

#define MORPHO_EASY_HDR_MERGE_SMOOTHNESS_LEVEL_DEFAULT  6

#define MORPHO_EASY_HDR_MERGE_PARAM1_DEFAULT   0
#define MORPHO_EASY_HDR_MERGE_PARAM2_DEFAULT 128
#define MORPHO_EASY_HDR_MERGE_PARAM3_DEFAULT   0
#define MORPHO_EASY_HDR_MERGE_PARAM4_DEFAULT 255

#define MORPHO_EASY_HDR_FM_PARAM1_DEFAULT 144
#define MORPHO_EASY_HDR_FM_PARAM2_DEFAULT 224
#define MORPHO_EASY_HDR_FM_PARAM3_DEFAULT  32
#define MORPHO_EASY_HDR_FM_PARAM4_DEFAULT 112

#define MORPHO_EASY_HDR_FM_SIGMA_DEFAULT 100

#define MORPHO_EASY_HDR_FM_XSIZE_PERMIL_DEFAULT    0

#define MORPHO_EASY_HDR_CC_Y_OFFSET_DEFAULT  0
#define MORPHO_EASY_HDR_CC_C_OFFSET_DEFAULT  0

#define MORPHO_EASY_HDR_CC_Y_GAIN_DEFAULT  1000
#define MORPHO_EASY_HDR_CC_C_GAIN_DEFAULT  1000

#define MORPHO_EASY_HDR_CC_Y_GAMMA_DEFAULT  1000
#define MORPHO_EASY_HDR_CC_C_GAMMA_DEFAULT  1000

#define MORPHO_EASY_HDR_FACE_DETECTION_DEFAULT  MORPHO_EASY_HDR_ENABLED

#define MORPHO_EASY_HDR_MERGE_FACE_PARAM1_DEFAULT 112
#define MORPHO_EASY_HDR_MERGE_FACE_PARAM2_DEFAULT 224

/*******************************************************************/

typedef struct _morpho_EasyHDR_Callback morpho_EasyHDR_Callback;

/*-----------------------------------------------------------------*/

/** EasyHDR Callback (for multi-thread processing) */
struct _morpho_EasyHDR_Callback
{
    void *p; /** Value passed as the first argument when calling the callback function. */

    void * (* thread_create )(void *p, int index, void *(*start_routine)(void *arg), void *arg);
    int    (* thread_destroy)(void *p, void *thread);
    int    (* thread_join   )(void *p, void *thread, void **value_ptr);

    void * (* mutex_create )(void *p);
    int    (* mutex_destroy)(void *p, void *mutex);
    int    (* mutex_lock   )(void *p, void *mutex);
    int    (* mutex_trylock)(void *p, void *mutex);
    int    (* mutex_unlock )(void *p, void *mutex);

    void * (* cond_create   )(void *p);
    int    (* cond_destroy  )(void *p, void *cond);
    int    (* cond_wait     )(void *p, void *cond, void *lock);
    int    (* cond_signal   )(void *p, void *cond);
    int    (* cond_broadcast)(void *p, void *cond);
};

/*******************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*-----------------------------------------------------------------*/

/**
 * Starts the HDR imaging process(for new merging method).
 *
 * @param[in,out]  p             Pointer to the Morpho HDR object
 * @param[out]     output_image  Pointer to the output image
 * @param[in,out]  input_images  Pointer to the input images
 * @param[in]      nimages       Number of input images
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_mergeEx(
    morpho_EasyHDR *p,
    morpho_ImageDataEx *output_image,
    morpho_ImageDataEx *input_images[],
    int nimages);

/*-----------------------------------------------------------------*/

/**
 * Sets image alignment status to enabled or disabled.
 *
 * Below are values that can be set:
 * - MORPHO_EASY_HDR_ENABLED    Image alignment enabled
 * - MORPHO_EASY_HDR_DISABLED   Image alignment disabled
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setImageAlignmentStatus(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets image alignment status; enabled or disabled.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getImageAlignmentStatus(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets ghost removal status to enabled or disabled.
 *
 * Below are values that can be set:
 * - MORPHO_EASY_HDR_ENABLED   Ghost removal enabled
 * - MORPHO_EASY_HDR_DISABLED  Ghost removal disabled
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setGhostRemovalStatus(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets ghost removal status; enabled or disabled.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getGhostRemovalStatus(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets automatic scaling (clipping) status to enabled or disabled.
 *
 * Below values can be set:
 *  - MORPHO_EASY_HDR_ENABLED   Auto scaling (clipping) enabled
 *  - MORPHO_EASY_HDR_DISABLED  Auto scaling (clipping) disabled
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setAutoScalingStatus(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets automatic scaling (clipping) status; enabled or disabled.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getAutoScalingStatus(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets new merging method status to enabled or disabled.
 *
 * Below values can be set:
 *  - MORPHO_EASY_HDR_ENABLED    Enable new merging method
 *  - MORPHO_EASY_HDR_DISABLED   Disable new merging method
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setFineMergingStatus(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets new merging method status; enabled or disabled.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getFineMergingStatus(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/


/**
 * Gets the valid rectangle after the merge.
 *
 * @param[in,out]  p     Pointer to the Morpho HDR object
 * @param[out]     rect  Valid rectangle
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getReliableRect(
    morpho_EasyHDR *p,
    morpho_RectInt *rect);

/*-----------------------------------------------------------------*/

/**
 * Sets the ghost removal strength level.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set (between 0 and 10)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setGhostRemovalStrengthLevel(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets the ghost removal strength level.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Set value
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getGhostRemovalStrengthLevel(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets the smoothness of luminance change during merge.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set (0-10)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setMergeSmoothnessLevel(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets the smoothness of luminance change during merge.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getMergeSmoothnessLevel(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets merge parameters.
 *
 * @param[in,out]  p       Pointer to the Morpho HDR object
 * @param[in]      value1  Value to be set (0-255)
 * @param[in]      value2  Value to be set (0-255)
 * @param[in]      value3  Value to be set (0-255)
 * @param[in]      value4  Value to be set (0-255)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setMergeParameters(
    morpho_EasyHDR *p,
    int value1,
    int value2,
    int value3,
    int value4);

/**
 * Gets merge parameters.
 *
 * @param[in,out]  p       Pointer to the Morpho HDR object
 * @param[out]     value1  Value that is set
 * @param[out]     value2  Value that is set
 * @param[out]     value3  Value that is set
 * @param[out]     value4  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getMergeParameters(
    morpho_EasyHDR *p,
    int *value1,
    int *value2,
    int *value3,
    int *value4);

/*-----------------------------------------------------------------*/

/**
 * Gets merge parameters.
 *
 * @param[in,out]  p       Pointer to the Morpho HDR object
 * @param[in]      value1  Value to be set (0-255)
 * @param[in]      value2  Value to be set (0-255)
 * @param[in]      value3  Value to be set (0-255)
 * @param[in]      value4  Value to be set (0-255)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setFMParameters(
    morpho_EasyHDR *p,
    int value1,
    int value2,
    int value3,
    int value4);

/**
 * Gets parameters for new merging method.
 *
 * @param[in,out]  p       Pointer to the Morpho HDR object
 * @param[out]     value1  Value that is set
 * @param[out]     value2  Value that is set
 * @param[out]     value3  Value that is set
 * @param[out]     value4  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getFMParameters(
    morpho_EasyHDR *p,
    int *value1,
    int *value2,
    int *value3,
    int *value4);

/*-----------------------------------------------------------------*/

/**
 * Sets merge parameters (standard deviation; new merging method)
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set (0-300)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setFMSigma(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets merge parameters (standard deviation; new merging method)
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getFMSigma(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets merge parameters (Minimum size[‰]; new merging method)
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set (0-1000)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setFMXSizePermil(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets merge parameters (Minimum size[‰]; new merging method)
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getFMXSizePermil(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets the color correction parameters.
 *
 * @param[in,out]  p         Pointer to the Morpho HDR object
 * @param[in]      y_offset  Value to be set (0-255)
 * @param[in]      y_gain    Value to be set (100-2000)
 * @param[in]      y_gamma   Value to be set (100-2000）
 * @param[in]      c_offset  Value to be set (0-255）
 * @param[in]      c_gain    Value to be set (100-2000）
 * @param[in]      c_gamma   Value to be set (100-2000）
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setColorCorrectionParameters(
    morpho_EasyHDR *p,
    int y_offset,
    int y_gain,
    int y_gamma,
    int c_offset,
    int c_gain,
    int c_gamma);

/**
 * Gets the color correction parameters.
 *
 * @param[in,out]  p         Pointer to the Morpho HDR object
 * @param[out]     y_offset  Value that is set
 * @param[out]     y_gain    Value that is set
 * @param[out]     y_gamma   Value that is set
 * @param[out]     c_offset  Value that is set
 * @param[out]     c_gain    Value that is set
 * @param[out]     c_gamma   Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getColorCorrectionParameters(
    morpho_EasyHDR *p,
    int *y_offset,
    int *y_gain,
    int *y_gamma,
    int *c_offset,
    int *c_gain,
    int *c_gamma);

/*-----------------------------------------------------------------*/

/**
 * Sets face detection status to enabled or disabled.
 *
 * Below are values that can be set:
 *  - MORPHO_EASY_HDR_ENABLED    Face detection enabled
 *  - MORPHO_EASY_HDR_DISABLED   Face detection disabled
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[in]      value  Value to be set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setFaceDetectionStatus(
    morpho_EasyHDR *p,
    int value);

/**
 * Gets face detection status; enabled or disabled.
 *
 * @param[in,out]  p      Pointer to the Morpho HDR object
 * @param[out]     value  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getFaceDetectionStatus(
    morpho_EasyHDR *p,
    int *value);

/*-----------------------------------------------------------------*/

/**
 * Sets merge parameters (for face detection)
 *
 * @param[in,out]  p       Pointer to the Morpho HDR object
 * @param[in]      value1  Value to be set (0-255)
 * @param[in]      value2  Value to be set (0-255)
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_setMergeFaceParameters(
    morpho_EasyHDR *p,
    int value1,
    int value2);

/**
 * Gets merge parameters (for face detection) 
 *
 * @param[in,out]  p       Pointer to the Morpho HDR object
 * @param[out]     value1  Value that is set
 * @param[out]     value2  Value that is set
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_getMergeFaceParameters(
    morpho_EasyHDR *p,
    int *value1,
    int *value2);

/*-----------------------------------------------------------------*/

/**
 * Creates thumbnails (shrunk output image).
 *
 * @param[in,out]  p                Pointer to the Morpho HDR object
 * @param[out]     thumbnail_image  Output image
 * @param[in]      output_image     Input image
 *
 * @return Error code (refer to the definitions in morpho_error.h)
 */
MORPHO_API(int)
morpho_EasyHDR_makeThumbnail(
    morpho_EasyHDR *p,
    morpho_ImageData *thumbnail_image,
    morpho_ImageData const *output_image);

/*-----------------------------------------------------------------*/

#ifdef __cplusplus
} /* extern "C" */
#endif

/*******************************************************************/

#endif /* !MORPHO_EASY_HDR_INTERNAL_H */

/*******************************************************************/
/* [EOF] */
