#ifndef __CNNAPI_V2_H__
#define __CNNAPI_V2_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// full mixed-precision

typedef struct image_mp
{
  uint32_t width;
  uint32_t height;

  uint8_t *vwidth;
  void *addr;
} image_mp_t;

typedef struct kernel_mp
{
  uint32_t size;

  uint8_t *vwidth;
  uint16_t *den;

  void *addr;
} kernel_mp_t;

typedef struct fc_filter_mp
{
  uint32_t width;
  uint32_t height;

  uint8_t *vwidth;
  uint16_t *den;

  void *addr;
} fc_filter_mp_t;

image_mp_t *Convolution_MP(image_mp_t *input_image, kernel_mp_t *input_kernel, int strides);

image_mp_t *MaxPooling_MP(image_mp_t *input_image, int pool_size, int strides);

image_mp_t *AvgPooling_MP(image_mp_t *input_image, int pool_size, int strides);

image_mp_t *Activation_MP(image_mp_t *input_image, char *algorithm, uint16_t *zero_point);

image_mp_t *Dense_MP(image_mp_t *input_image, fc_filter_mp_t *fc_filter_array, int units);

#ifdef __cplusplus
}
#endif

#endif
