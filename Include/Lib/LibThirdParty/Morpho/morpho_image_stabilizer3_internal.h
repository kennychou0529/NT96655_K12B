/*=====================================================================
   morpho_image_stabilizer3_internal.h
   [SJIS/CRLF] { Ç† ïÑçÜâªï˚éÆé©ìÆîªíËóp }

   Copyright(c) 2006-2013 Morpho,Inc.
======================================================================*/

#ifndef MORPHO_IMAGE_STABILIZER3_INTERNAL_H
# define MORPHO_IMAGE_STABILIZER3_INTERNAL_H

/*--------------------------------------------------------------------*/

#include "morpho_image_data_ex.h"

/*--------------------------------------------------------------------*/

# ifdef __cplusplus
extern "C" {
# endif

/*====================================================================*/
#if defined(MORPHO_USE_IMAGE_DATA_PITCH)

/**
 * Image stabilizer: processing start
 * output_image may be the same as a input image.
 *
 * @param[in,out] stabilizer    Pointer to the image stabilizer
 * @param[out]    output_image  Pointer to the output image
 * @param[in]     image_num     Number of input images
 * @return  Error code(morpho_error.h)
 */
#if !defined(MORPHO_IMAGE_STABILIZER3_USE_HW_IF)
MORPHO_API(int)
morpho_ImageStabilizer3_startEx(
    morpho_ImageStabilizer3 *stabilizer,
    morpho_ImageDataEx *output_image,
    morpho_ImageDataEx *work_image,
    int image_num);
#else
MORPHO_API(int)
morpho_ImageStabilizer3_startEx(
    morpho_ImageStabilizer3 *stabilizer,
    morpho_ImageDataEx *output_image,
    int image_num);
#endif

/**
 * Image stabilizer: motion detection
 *
 * @param[in,out] stabilizer   Pointer to the image stabilizer
 * @param[in]     input_image  Pointer to the input image
 * @param[out]    motion_data  moiton data
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_detectMotionEx(
    morpho_ImageStabilizer3 *stabilizer,
    const morpho_ImageDataEx *input_image,
    morpho_MotionData *motion_data);

/**
 * Image stabilizer: alpha blending
 *
 * @param[in,out] stabilizer   Pointer to the image stabilizer
 * @param[in]     input_image  Pointer to the input image
 * @param[in]     motion_data  moiton data
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_renderEx(
    morpho_ImageStabilizer3 *stabilizer,
    const morpho_ImageDataEx *input_image,
    morpho_MotionData *motion_data);

# ifdef MORPHO_IMAGE_STABILIZER3_USE_HW_IF
/**
 * Image stabilizer: blending area acquisition
 *
 * @param[in,out] stabilizer   Pointer to the image stabilizer
 * @param[in]     input_image  Pointer to the input image
 * @param[out]    rect         Rectangular area to blend
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_calcAccumRectEx(
    morpho_ImageStabilizer3 *stabilizer,
    const morpho_ImageDataEx *input_image,
    morpho_RectInt *rect);

/**
 * Image stabilizer: alignment
 *
 * @param[in,out] stabilizer    Pointer to the image stabilizer
 * @param[in]     input_image   Pointer to the input image
 * @param[out]    output_image  Pointer to the output image
 * @param[out]    rect          Rectangular area to blend
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_alignImageEx(
    morpho_ImageStabilizer3 *stabilizer,
    morpho_ImageDataEx *output_image,
    const morpho_ImageDataEx *input_image,
    morpho_RectInt *rect);

/**
 * Image stabilizer: ghost detection & blending mask generation
 *
 * @param[in,out] stabilizer   Pointer to the image stabilizer
 * @param[in]     input_image  Pointer to the input image
 * @param[in]     alpha_tbl    blending mask
 * @param[in]     rect         Rectangular area to blend
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_detectGhostEx(
    morpho_ImageStabilizer3 *stabilizer,
    unsigned char *alpha_tbl,
    const morpho_ImageDataEx *input_image,
    const morpho_RectInt *rect);

/**
 * Image stabilizer: alpha blending
 *
 * @param[in,out] stabilizer   Pointer to the image stabilizer
 * @param[in]     input_image  Pointer to the input image
 * @param[in]     alpha_tbl    blending mask
 * @param[in]     rect         Rectangular area to blend
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_blendEx(
    morpho_ImageStabilizer3 *stabilizer,
    const morpho_ImageDataEx *input_image,
    const unsigned char *alpha_tbl,
    const morpho_RectInt *rect);

# endif    /* MORPHO_IMAGE_STABILIZER3_USE_HW_IF */

/**
 * Image stabilizer: noise reduction for Y image
 *
 * @param[in,out] stabilizer    Pointer to the image stabilizer
 * @param[out]    output_image  Pointer to the output image
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_reduceNoiseLumaEx(
    morpho_ImageStabilizer3 *stabilizer,
    morpho_ImageDataEx *output_image);

/**
 * Image stabilizer: noise reduction for C image
 *
 * @param[in,out] stabilizer    Pointer to the image stabilizer
 * @param[out]    output_image  Pointer to the output image
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_reduceNoiseChromaEx(
    morpho_ImageStabilizer3 *stabilizer,
    morpho_ImageDataEx *output_image);

/**
 * Image stabilizer: enhance image
 *
 * @param[in,out] stabilizer    Pointer to the image stabilizer
 * @param[out]    output_image  Pointer to the output image
 * @return  Error code(morpho_error.h)
 */
MORPHO_API(int)
morpho_ImageStabilizer3_enhanceImageEx(
    morpho_ImageStabilizer3 *stabilizer,
    morpho_ImageDataEx *output_image);

#endif    /* MORPHO_USE_IMAGE_DATA_PITCH */

/*====================================================================*/

# ifdef __cplusplus
} /* extern "C" */
# endif

/*--------------------------------------------------------------------*/

#endif /* MORPHO_IMAGE_STABILIZER3_INTERNAL_H */

/*====================================================================*/
/* [EOF] */
