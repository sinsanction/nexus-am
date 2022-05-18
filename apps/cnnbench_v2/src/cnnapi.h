#ifndef __CNNAPI_H__
#define __CNNAPI_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct image
{
  uint32_t width;
  uint32_t height;

  uint8_t vwidth;
  uint8_t order;

  void *addr;
} image_t;

typedef struct kernel
{
  uint32_t size;
  uint8_t vwidth;

  uint16_t den;

  void *addr;
} kernel_t;

typedef struct fc_filter
{
  uint32_t width;
  uint32_t height;

  uint8_t vwidth;
  uint8_t order;
  uint16_t den;

  void *addr;
} fc_filter_t;

image_t *Transpose(image_t *input_image);

image_t *Convolution(image_t *input_image, kernel_t *input_kernel, int strides);

image_t *MaxPooling(image_t *input_image, int pool_size, int strides);

image_t *AvgPooling(image_t *input_image, int pool_size, int strides);

image_t *Activation(image_t *input_image, char *algorithm, uint16_t zero_point);

image_t *Dense(image_t *input_image, fc_filter_t *fc_filter_array, int units);


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
