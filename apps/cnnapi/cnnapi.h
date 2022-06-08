#ifndef __CNNAPI_H__
#define __CNNAPI_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>
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

inline int round_up_div(int a, int b);
inline uint16_t get_main_value(uint64_t *img_addr, int i, uint8_t vwidth);
inline int8_t get_kernel_value(uint64_t *kernel_addr, int i, uint8_t vwidth);
inline void put_main_value(uint64_t *img_addr, int i, uint8_t vwidth, uint16_t value);

//IO
image_t *RandomInit(uint32_t width, uint32_t height, uint32_t bits, uint8_t order);

kernel_t *RandomInitKernel(uint32_t k, uint32_t bits);

void SetOutput(image_t *output_image);

void SetOutputKernel(kernel_t *output_kernel);

//utils
image_t *Transpose(image_t *input_image);

image_t *MergeImage(image_t *input_image_a, image_t *input_image_b);

//arithmetic
image_t *Convolution(image_t *input_image, kernel_t *input_kernel, int strides);

image_t *MaxPooling(image_t *input_image, int pool_size, int strides);

image_t *AvgPooling(image_t *input_image, int pool_size, int strides);

image_t *Activation(image_t *input_image, char *algorithm, uint16_t zero_point);

image_t *Dense(image_t *input_image, fc_filter_t *fc_filter_array, int units);

#ifdef __cplusplus
}
#endif

#endif
